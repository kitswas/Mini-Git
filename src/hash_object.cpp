#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <openssl/evp.h>
#include <string>
#include <unistd.h>

// For getopt()
extern int opterr;
extern int optind;
extern int optopt;
extern char *optarg;

/**
 * @brief Takes a file path and returns the SHA1 hash of the file.
 */
std::string get_sha1(const std::string &path)
{
	std::ifstream file(path, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file " << path << std::endl;
		exit(EXIT_FAILURE);
	}

	EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
	if (mdctx == nullptr)
	{
		std::cerr << "Error: Could not create EVP_MD_CTX\n";
		exit(EXIT_FAILURE);
	}

	if (EVP_DigestInit(mdctx, EVP_sha1()) != 1)
	{
		std::cerr << "Error: Could not initialize digest\n";
		exit(EXIT_FAILURE);
	}

	const size_t buf_size = 1024;
	char buffer[buf_size];
	while (file.good())
	{
		file.read(buffer, buf_size);
		if ((file.gcount() > 0) && (EVP_DigestUpdate(mdctx, buffer, file.gcount()) != 1))
		{
			std::cerr << "Error: Could not update digest\n";
			exit(EXIT_FAILURE);
		}
	}

	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int hash_len;
	if (EVP_DigestFinal(mdctx, hash, &hash_len) != 1)
	{
		std::cerr << "Error: Could not finalize digest\n";
		exit(EXIT_FAILURE);
	}

	EVP_MD_CTX_free(mdctx);

	std::string result;
	for (unsigned int i = 0; i < hash_len; i++)
	{
		result += std::format("{:02x}", hash[i]);
	}

	return result;
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
	std::string sha1 = get_sha1(path);
	std::cout << sha1 << std::endl;

	if (write)
	{
		try
		{
			std::filesystem::create_directory(".mygit/objects/" + sha1.substr(0, 2));
		}
		catch (const std::filesystem::filesystem_error &e)
		{
			if (e.code() == std::errc::no_such_file_or_directory)
			{
				std::cerr << "Did you forget run `init`?\n";
			}
			else
			{
				std::cerr << e.what() << std::endl;
			}
			return EXIT_FAILURE;
		}
		std::ofstream file(".mygit/objects/" + sha1.substr(0, 2) + "/" + sha1.substr(2));
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
	}

	return EXIT_SUCCESS;
}
