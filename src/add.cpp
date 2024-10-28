#include "blob.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "tree.hpp"
#include <zstr.hpp> // ZLib C++ wrapper. See https://github.com/mateidavid/zstr

int add(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cerr << "Command usage: add <path1> <path2> ...\n";
		return EXIT_FAILURE;
	}

	for (int i = 1; i < argc; i++)
	{
		std::filesystem::path path = argv[i];
		if (!std::filesystem::exists(path))
		{
			std::cerr << "Path " << path << " does not exist\n";
			return EXIT_FAILURE;
		}
	}

	TreeObject tree;
	if (std::filesystem::exists(".mygit/index"))
	{
		tree = TreeObject::read(".mygit/index");
	}

	for (int i = 1; i < argc; i++)
	{
		std::filesystem::path path = argv[i];

		if (std::filesystem::is_directory(path))
		{
			for (const auto &entry : std::filesystem::recursive_directory_iterator(path))
			{
				if (entry.is_regular_file())
				{
					std::string sha = get_file_sha1(entry.path());
					store_file_as_blob(entry.path(), sha);
					tree.add_entry("100644", entry.path(), sha);
				}
				else if (entry.is_symlink())
				{
					std::string sha = get_file_sha1(entry.path());
					store_file_as_blob(entry.path(), sha);
					tree.add_entry("120000", entry.path(), sha);
				}
			}
		}
		else
		{
			std::string sha = get_file_sha1(path);
			store_file_as_blob(path, sha);
			tree.add_entry("100644", path, sha);
		}
	}

	std::ofstream index_file(".mygit/index", std::ios::trunc);
	if (!index_file.is_open())
	{
		std::cerr << "Failed to open index file\n";
		return EXIT_FAILURE;
	}
	index_file << tree.get_data().data();

	return EXIT_SUCCESS;
}
