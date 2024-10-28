#include "blob.hpp"
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include "sha.hpp"
#include <string>
#include <unistd.h>
#include <zstr.hpp> // ZLib C++ wrapper. See https://github.com/mateidavid/zstr

// For getopt()
extern int opterr;
extern int optind;
extern int optopt;
extern char *optarg;

int store_file_as_blob(const std::string &path, const std::string &sha1)
{
	std::filesystem::create_directory(".mygit/objects/" + sha1.substr(0, 2));
	zstr::ofstream file(".mygit/objects/" + sha1.substr(0, 2) + "/" + sha1.substr(2), std::ios_base::trunc);
	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file " << ".mygit/objects/" + sha1.substr(0, 2) + "/" + sha1.substr(2) << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream input(path, std::ios::binary);
	if (!input.is_open())
	{
		std::cerr << "Error: Could not open file " << path << std::endl;
		return EXIT_FAILURE;
	}

	auto file_size = std::filesystem::file_size(path);
	file << "blob " << file_size << '\0';
	file << input.rdbuf();
	return EXIT_SUCCESS;
}

int hash_object(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cerr << "Command usage: hash-object [-w] <file>\n";
		return EXIT_FAILURE;
	}

	opterr = 0;
	int c;
	bool write = false;
	while ((c = getopt(argc, argv, "w")) != -1)
	{
		switch (c)
		{
		case 'w':
			write = true;
			break;
		case '?':
			std::cerr << "Unknown option -" << static_cast<char>(optopt) << '\n';
			return EXIT_FAILURE;
		default:
			abort();
		}
	}

	if (optind >= argc)
	{
		std::cerr << "Expected argument after options\n";
		return EXIT_FAILURE;
	}

	std::string path = argv[optind];
	std::string sha1 = get_file_sha1(path);
	std::cout << sha1 << std::endl;

	if (write)
	{
		try
		{
			return store_file_as_blob(path, sha1);
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
	}

	return EXIT_SUCCESS;
}
