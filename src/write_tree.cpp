#include <filesystem>
#include <iostream>
#include "tree.hpp"

int write_tree(int argc, [[maybe_unused]] char *argv[])
{
	if (argc != 1)
	{
		std::cerr << "Command usage: write-tree\n";
		return EXIT_FAILURE;
	}

	try
	{
		TreeObject tree = TreeObject::create_tree(std::filesystem::current_path());
		std::cout << tree.write() << '\n';
	}
	catch (const std::filesystem::filesystem_error &e)
	{
		if (e.code() == std::errc::no_such_file_or_directory)
		{
			std::cerr << "Did you forget to run `init`?\n";
		}
		else
		{
			std::cerr << e.what() << std::endl;
		}
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
