#include "commit.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <zstr.hpp> // ZLib C++ wrapper. See https://github.com/mateidavid/zstr

int log(int argc, [[maybe_unused]] char *argv[])
{
	if (argc != 1)
	{
		std::cerr << "Command usage: log\n";
		return EXIT_FAILURE;
	}
	try
	{
		std::ifstream head_file(".mygit/HEAD");
		std::string head;
		head_file >> head;
		head_file.close();

		std::string commit_sha = head;
		while (!commit_sha.empty())
		{
			const std::string commit_path = ".mygit/objects/" + commit_sha.substr(0, 2) + "/" + commit_sha.substr(2);
			zstr::ifstream input(commit_path, std::ios::binary);
			if (!input.is_open())
			{
				std::cerr << "Failed to open commit object file " << commit_path << " for reading\n";
				exit(EXIT_FAILURE);
			}
			std::string parent;
			std::string line;
			while (std::getline(input, line))
			{
				if (line.starts_with("parent"))
				{
					parent = line.substr(7);
					break;
				}
			}
			std::cout << "commit " << commit_sha << '\n';
			if (!parent.empty())
			{
				std::cout << "parent " << parent << '\n';
			}
			std::cout << input.rdbuf() << '\n';

			commit_sha = parent;
		}
	}
	catch (const std::filesystem::filesystem_error &e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
