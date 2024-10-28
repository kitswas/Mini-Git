#include "blob.hpp"
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include "sha.hpp"
#include "tree.hpp"
#include <zstr.hpp> // ZLib C++ wrapper. See https://github.com/mateidavid/zstr

void TreeObject::add_or_update_entry(const std::string &mode, const std::string &path, const std::string &sha)
{
	entries.insert_or_assign(path, EntryData{mode, sha});
}

TreeObject TreeObject::create_tree(const std::string &path)
{
	TreeObject tree;
	for (const auto &entry : std::filesystem::directory_iterator(path))
	{
		if (entry.path().filename() == ".mygit")
		{
			continue;
		}
		if (entry.is_directory())
		{
			TreeObject subtree = create_tree(entry.path());
			std::string sha = subtree.write();
			tree.add_or_update_entry("040000", entry.path().filename(), sha);
		}
		else if (entry.is_regular_file())
		{
			std::string sha = get_file_sha1(entry.path());
			store_file_as_blob(entry.path(), sha);
			tree.add_or_update_entry("100644", entry.path().filename(), sha);
		}
		else if (entry.is_symlink())
		{
			std::string sha = get_file_sha1(entry.path());
			store_file_as_blob(entry.path(), sha);
			tree.add_or_update_entry("120000", entry.path().filename(), sha);
		}
	}
	return tree;
}

std::vector<char> TreeObject::get_data() const
{
	std::vector<char> result;
	uintmax_t size = 0;
	for (const auto &[path, entry] : entries)
	{
		size += entry.mode.size() + 1 + path.size() + 1 + entry.sha.size() + 1;
	}
	result.reserve(size);
	std::string header = std::format("tree {}", size);
	result.insert(result.begin(), header.begin(), header.end());
	result.push_back(' ');
	for (const auto &[path, entry] : entries)
	{
		result.insert(result.end(), entry.mode.begin(), entry.mode.end());
		result.push_back(' ');
		result.insert(result.end(), path.begin(), path.end());
		result.push_back('\t');
		result.insert(result.end(), entry.sha.begin(), entry.sha.end());
		result.push_back('\n');
	}
	return result;
}

std::map<std::string, TreeObject::EntryData> TreeObject::get_entries() const
{
	return entries;
}

TreeObject TreeObject::read(const std::string &file_path)
{
	zstr::ifstream input(file_path, std::ios::binary);
	if (!input.is_open())
	{
		std::cerr << "Failed to open tree object file " << file_path << " for reading\n";
		exit(EXIT_FAILURE);
	}

	std::string type;
	input >> type;
	uintmax_t blob_size;
	input >> blob_size;

	if (type == "tree")
	{
		TreeObject tree;

		std::string mode;
		std::string name;
		std::string sha;

		while (input >> mode >> name >> sha)
		{
			tree.add_or_update_entry(mode, name, sha);
		}
		input.close();
		return tree;
	}
	else
	{
		input.close();
		throw std::invalid_argument("Not a tree object");
	}
}

std::string TreeObject::write() const
{
	std::vector<char> treeData = this->get_data();
	std::string treeSha = get_sha1(treeData);

	std::filesystem::create_directory(".mygit/objects/" + treeSha.substr(0, 2));
	zstr::ofstream treeFile(".mygit/objects/" + treeSha.substr(0, 2) + "/" + treeSha.substr(2), std::ios_base::trunc);
	if (!treeFile.is_open())
	{
		std::cerr << "Failed to open tree object file " << treeSha << " for writing\n";
		exit(EXIT_FAILURE);
	}
	treeFile.write(treeData.data(), treeData.size());
	treeFile.close();

	return treeSha;
}
