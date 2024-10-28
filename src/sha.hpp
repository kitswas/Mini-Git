#pragma once

#include <string>
#include <vector>

/**
 * @brief Takes a file path and returns the SHA1 hash of the file.
 */
std::string get_file_sha1(const std::string &path);

/**
 * @brief Takes a char vector and returns the SHA1 hash of the string.
 */
std::string get_sha1(const std::vector<char> &data);
