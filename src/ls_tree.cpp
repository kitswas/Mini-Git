#include <iostream>
#include <string>
#include "tree.hpp"
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

	std::string obj_path;
	if (value == "index")
	{
		obj_path = ".mygit/index";
	}
	else if (value.size() == 40)
	{
		const std::string dir_name = value.substr(0, 2);
		const std::string blob_sha = value.substr(2);
		obj_path = ".mygit/objects/" + dir_name + "/" + blob_sha;
	}
	else
	{
		std::cerr << "Invalid SHA-1 value\n";
		return EXIT_FAILURE;
	}

	try
	{
		TreeObject tree = TreeObject::read(obj_path);
		for (const auto &[path, entry] : tree.get_entries())
		{
			if (name_only)
			{
				std::cout << path << '\n';
			}
			else
			{
				std::cout << type_map.at(entry.mode) << ' ' << entry.sha << '\t' << path << '\n';
			}
		}
	}
	catch (...)
	{
		std::cerr << "Failed to open object file.\n";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
