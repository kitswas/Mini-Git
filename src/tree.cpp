#include "blob.hpp"
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <openssl/evp.h>
#include "tree.hpp"
#include <zstr.hpp> // ZLib C++ wrapper. See https://github.com/mateidavid/zstr

/**
 * @brief Takes a string and returns the SHA1 hash of the string.
 */
std::string get_sha1(const std::vector<char> &data)
{
	EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
	if (mdctx == nullptr)
	{
		std::cerr << "Error: Could not create EVP_MD_CTX\n";
		exit(EXIT_FAILURE);
	}

	if (EVP_DigestInit(mdctx, EVP_sha1()) != 1)
	{
		std::cerr << "Error: Could not initialize digest\n";
		exit(EXIT_FAILURE);
	}

	if (EVP_DigestUpdate(mdctx, data.data(), data.size()) != 1)
	{
		std::cerr << "Error: Could not update digest\n";
		exit(EXIT_FAILURE);
	}

	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int hash_len;
	if (EVP_DigestFinal(mdctx, hash, &hash_len) != 1)
	{
		std::cerr << "Error: Could not finalize digest\n";
		exit(EXIT_FAILURE);
	}

	std::string result;
	for (unsigned int i = 0; i < hash_len; i++)
	{
		result += std::format("{:02x}", hash[i]);
	}

	EVP_MD_CTX_free(mdctx);
	return result;
}

void TreeObject::add_entry(const std::string &mode, const std::string &name, const std::string &sha)
{
	entries.emplace_back(mode, name, sha);
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
			std::clog << "Subtree " << entry.path().filename() << " has SHA-1 " << sha << '\n';
			tree.add_entry("40000", entry.path().filename(), sha);
		}
		else if (entry.is_regular_file())
		{
			std::string sha = get_file_sha1(entry.path());
			store_file_as_blob(entry.path(), sha);
			tree.add_entry("100644", entry.path().filename(), sha);
		}
		else if (entry.is_symlink())
		{
			std::string sha = get_file_sha1(entry.path());
			store_file_as_blob(entry.path(), sha);
			tree.add_entry("120000", entry.path().filename(), sha);
		}
	}
	return tree;
}

std::vector<char> TreeObject::get_data() const
{
	std::vector<char> result;
	uintmax_t size = 0;
	for (const auto &entry : entries)
	{
		size += entry.mode.size() + 1 + entry.name.size() + 1 + entry.sha.size() + 1;
	}
	result.reserve(size);
	std::string header = std::format("tree {}\0", size);
	result.insert(result.begin(), header.begin(), header.end());
	result.push_back('\0');
	for (const auto &entry : entries)
	{
		result.insert(result.end(), entry.mode.begin(), entry.mode.end());
		result.push_back(' ');
		result.insert(result.end(), entry.name.begin(), entry.name.end());
		result.push_back('\0');
		result.insert(result.end(), entry.sha.begin(), entry.sha.end());
		result.push_back('\n');
	}
	return result;
}

std::string TreeObject::write() const
{
	std::vector<char> treeData = this->get_data();
	std::string treeSha = get_sha1(treeData);

	std::filesystem::create_directory(".mygit/objects/" + treeSha.substr(0, 2));
	zstr::ofstream treeFile(".mygit/objects/" + treeSha.substr(0, 2) + "/" + treeSha.substr(2));
	if (!treeFile.is_open())
	{
		std::cerr << "Failed to open tree object file " << treeSha << " for writing\n";
		exit(EXIT_FAILURE);
	}
	treeFile.write(treeData.data(), treeData.size());
	treeFile.close();

	return treeSha;
}
