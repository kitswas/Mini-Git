#include "commands.hpp"
#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
	// Flush after every std::cout / std::cerr
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;

	if (argc < 2)
	{
		std::cerr << "No command provided.\n";
		return EXIT_FAILURE;
	}

	std::string command = argv[1];
	// We will remove the first argument
	argc--;
	argv++;

	if (command == "init")
	{
		return init(argc, argv);
	}
	else if (!std::filesystem::exists(".mygit"))
	{
		std::cerr << "Not a repository. Run `init` first\n"
				  << "All commands must be run from the root of the repository\n";
		return EXIT_FAILURE;
	}

	if (command == "hash-object")
	{
		return hash_object(argc, argv);
	}
	else if (command == "cat-file")
	{
		return cat_file(argc, argv);
	}
	else if (command == "ls-tree")
	{
		return ls_tree(argc, argv);
	}
	else if (command == "write-tree")
	{
		return write_tree(argc, argv);
	}
	else if (command == "add")
	{
		return add(argc, argv);
	}
	else if (command == "commit")
	{
		return commit(argc, argv);
	}
	else
	{
		std::cerr << "Unknown command " << command << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
