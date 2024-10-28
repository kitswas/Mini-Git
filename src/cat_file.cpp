#include <iostream>
#include <string>
#include "types.hpp"
#include <zstr.hpp> // ZLib C++ wrapper. See https://github.com/mateidavid/zstr

int cat_file(int argc, char *argv[])
{
	if (argc < 3)
	{
		std::cerr << "Command usage: cat-file -p <hash>\n";
		return EXIT_FAILURE;
	}

	opterr = 0;
	int c;
	bool f_print = false;
	bool f_size = false;
	bool f_type = false;
	int option_count = 0;
	while ((c = getopt(argc, argv, "pst")) != -1)
	{
		option_count++;
		switch (c)
		{
		case 'p':
			f_print = true;
			break;
		case 's':
			f_size = true;
			break;
		case 't':
			f_type = true;
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
	if (option_count > 1)
	{
		std::cerr << "Only one option is allowed\n";
		return EXIT_FAILURE;
	}

	const std::string value = argv[2];

	const std::string dir_name = value.substr(0, 2);
	const std::string blob_sha = value.substr(2);

	std::string obj_path = ".mygit/objects/" + dir_name + "/" + blob_sha;

	try
	{
		zstr::ifstream input(obj_path, std::ofstream::binary);

		if (!input.is_open())
		{
			std::cerr << "Failed to open object file.\n";
			return EXIT_FAILURE;
		}

		std::string type;
		input >> type;
		uintmax_t blob_size;
		input >> blob_size;

		if (f_print)
		{
			if (type == "blob" || type == "commit")
			{
				std::cout << input.rdbuf();
			}
			else if (type == "tree")
			{
				std::string mode;
				std::string path;
				std::string sha;

				while (input >> mode >> path >> sha)
				{
					std::cout << mode << ' ' << type_map.at(mode) << ' ' << sha << '\t' << path << '\n';
				}
			}
		}
		else if (f_size)
		{
			std::cout << blob_size << '\n';
		}
		else if (f_type)
		{
			std::cout << type << '\n';
		}
		else
		{
			std::cerr << "No option provided\n";
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
