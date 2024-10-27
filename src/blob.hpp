#pragma once

#include <string>

std::string get_file_sha1(const std::string &path);
int store_file_as_blob(const std::string &path, const std::string &sha1);
