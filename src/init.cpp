#include <iostream>
#include <filesystem>
#include <fstream>

int init(int argc, [[maybe_unused]] char *argv[])
{
	if (argc != 1)
	{
		std::cerr << "Command usage: init\n";
		return EXIT_FAILURE;
	}
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
	return EXIT_SUCCESS;
}
