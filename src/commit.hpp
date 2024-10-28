#pragma once

#include <string>
#include <chrono>
#include "tree.hpp"

struct AuthorInfo
{
	const std::string name = "S";
	const std::string email = "not@telling.you";
};

struct CommitObject
{
	AuthorInfo author;
	std::chrono::system_clock::time_point timestamp;
	TreeObject tree;
	std::string tree_sha;
	std::string message;
	std::string parent;

	CommitObject(const TreeObject &tree, const std::string &message, std::string parent);

	std::string write() const;
};
