#include <iostream>
#include <filesystem>
#include <fstream>
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
	else
	{
		std::cerr << "Unknown command " << command << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
