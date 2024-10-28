#include <format>
#include <fstream>
#include <iostream>
#include <openssl/evp.h>
#include <string>
#include <vector>

std::string get_file_sha1(const std::string &path)
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

std::string get_sha1(const std::vector<char> &data)
{
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

	if (EVP_DigestUpdate(mdctx, data.data(), data.size()) != 1)
	{
		std::cerr << "Error: Could not update digest\n";
		exit(EXIT_FAILURE);
	}

	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int hash_len;
	if (EVP_DigestFinal(mdctx, hash, &hash_len) != 1)
	{
		std::cerr << "Error: Could not finalize digest\n";
		exit(EXIT_FAILURE);
	}

	std::string result;
	for (unsigned int i = 0; i < hash_len; i++)
	{
		result += std::format("{:02x}", hash[i]);
	}

	EVP_MD_CTX_free(mdctx);
	return result;
}
