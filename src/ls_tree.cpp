#include <iostream>
#include <string>
#include "types.hpp"
#include <zstr.hpp> // ZLib C++ wrapper. See https://github.com/mateidavid/zstr

const std::unordered_map<std::string, std::string> type_map = {
	{"100644", "blob"},
	{"040000", "tree"},
	{"120000", "symlink"},
};

int ls_tree(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cerr << "Command usage: ls-tree [--name-only] <tree_sha>\n";
		return EXIT_FAILURE;
	}

	bool name_only = false;
	if (argc == 3)
	{
		if (std::string(argv[1]) == "--name-only")
		{
			name_only = true;
		}
		else
		{
			std::cerr << "Invalid arguments\n";
			return EXIT_FAILURE;
		}
	}

	const std::string value = argv[name_only ? 2 : 1];

	const std::string dir_name = value.substr(0, 2);
	const std::string blob_sha = value.substr(2);

	std::string path = ".mygit/objects/" + dir_name + "/" + blob_sha;

	try
	{
		zstr::ifstream input(path, std::ofstream::binary);

		if (!input.is_open())
		{
			std::cerr << "Failed to open object file.\n";
			return EXIT_FAILURE;
		}

		std::string type;
		input >> type;
		uintmax_t blob_size;
		input >> blob_size;

		if (type == "tree")
		{
			std::string mode;
			std::string name;
			std::string sha;

			while (input >> mode >> name >> sha)
			{
				if (name_only)
				{
					std::cout << name << '\n';
				}
				else
				{
					std::cout << mode << ' ' << type_map.at(mode) << ' ' << sha << '\t' << name << '\n';
				}
			}
		}
		else
		{
			std::cerr << "Not a tree object\n";
			return EXIT_FAILURE;
		}
		input.close();
	}
	catch (...)
	{
		std::cerr << "Failed to open object file.\n";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
