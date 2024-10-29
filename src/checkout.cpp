#include "commit.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include "tree.hpp"
#include <zstr.hpp> // ZLib C++ wrapper. See https://github.com/mateidavid/zstr

long restore_tree(const std::string &tree_sha)
{
	long restored = 0;
	try
	{
		TreeObject tree = TreeObject::read(".mygit/objects/" + tree_sha.substr(0, 2) + "/" + tree_sha.substr(2));
		for (const auto &[path, entry] : tree.get_entries())
		{
			if (entry.mode == "040000")
			{
				std::filesystem::create_directory(path);
				restored += restore_tree(entry.sha);
			}
			else
			{
				std::string blob_path = ".mygit/objects/" + entry.sha.substr(0, 2) + "/" + entry.sha.substr(2);
				zstr::ifstream input(blob_path, std::ios::binary);
				if (!input.is_open())
				{
					std::cerr << "Failed to open blob object file " << blob_path << " for reading\n";
					exit(EXIT_FAILURE);
				}
				std::ofstream output(path, std::ios::binary | std::ios::trunc);
				output << input.rdbuf();
				output.close();
				++restored;
			}
		}
		return restored;
	}
	catch (const std::filesystem::filesystem_error &e)
	{
		std::cerr << e.what() << '\n';
		return -1;
	}
}

int checkout(int argc, [[maybe_unused]] char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Command usage: checkout <commit_sha>\n";
		return EXIT_FAILURE;
	}

	std::string commit_sha = argv[1];
	if (commit_sha.size() != 40)
	{
		std::cerr << "Invalid commit SHA\n";
		return EXIT_FAILURE;
	}

	const std::string commit_path = ".mygit/objects/" + commit_sha.substr(0, 2) + "/" + commit_sha.substr(2);
	if (!std::filesystem::exists(commit_path))
	{
		std::cerr << "Commit " << commit_sha << " does not exist\n";
		return EXIT_FAILURE;
	}

	try
	{
		zstr::ifstream input(commit_path, std::ios::binary);
		if (!input.is_open())
		{
			std::cerr << "Failed to open commit object file " << commit_path << " for reading\n";
			exit(EXIT_FAILURE);
		}
		std::string tree_sha;
		std::string line;
		while (std::getline(input, line))
		{
			if (line.starts_with("tree"))
			{
				tree_sha = line.substr(5);
				break;
			}
		}
		input.close();

		if (tree_sha.empty())
		{
			std::cerr << "No tree found in commit " << commit_sha << '\n';
			return EXIT_FAILURE;
		}

		auto restored = restore_tree(tree_sha);
		if (restored == -1)
		{
			return EXIT_FAILURE;
		}
		std::cout << "Restored " << restored << " files\n";

		std::ofstream head_file(".mygit/HEAD", std::ios::trunc);
		head_file << commit_sha;
		head_file.close();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
