#include "commands.hpp"
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
	else if (command == "hash-object")
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
	else
	{
		std::cerr << "Unknown command " << command << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
