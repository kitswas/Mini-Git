#include <iostream>
#include <string>
#include <zstr.hpp> // ZLib C++ wrapper. See https://github.com/mateidavid/zstr

int cat_file(const std::string &value)
{
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

		const std::string object_content =
			object_str.substr(object_str.find('\0') + 1);
		std::cout << object_content;
	}
	catch (...)
	{
		std::cerr << "Failed to open object file.\n";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
