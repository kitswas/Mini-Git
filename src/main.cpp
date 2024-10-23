#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <zstr.hpp> // ZLib C++ wrapper. See https://github.com/mateidavid/zstr

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

	if (std::string command = argv[1]; command == "init")
	{
		try
		{
			std::filesystem::create_directory(".mygit");
			std::filesystem::create_directory(".mygit/objects");
			std::filesystem::create_directory(".mygit/refs");

			if (std::ofstream headFile(".mygit/HEAD"); headFile.is_open())
			{
				headFile << "ref: refs/heads/main\n";
				headFile.close();
			}
			else
			{
				std::cerr << "Failed to create .mygit/HEAD file.\n";
				return EXIT_FAILURE;
			}

			std::cout << "Initialized git directory\n";
		}
		catch (const std::filesystem::filesystem_error &e)
		{
			std::cerr << e.what() << '\n';
			return EXIT_FAILURE;
		}
	}
	else if (command == "cat-file")
	{
		if (argc < 4)
		{
			std::cerr << "Invalid arguments, `-p <blob_sha>` required.\n";
			return EXIT_FAILURE;
		}

		if (const std::string flag = argv[2]; flag != "-p")
		{
			std::cerr << "Invalid flag for cat-file, `-p` expected\n";
			return EXIT_FAILURE;
		}

		const std::string value = argv[3];
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

			std::string object_str{std::istreambuf_iterator<char>(input),
								   std::istreambuf_iterator<char>()};

			input.close();

			const std::string object_content = object_str.substr(object_str.find('\0') + 1);
			std::cout << object_content;
		}
		catch (...)
		{
			std::cerr << "Failed to open object file.\n";
			return EXIT_FAILURE;
		}
	}
	else
	{
		std::cerr << "Unknown command " << command << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
