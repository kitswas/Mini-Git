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
	std::string tree_sha;
	std::string message;
	std::string parent;

	CommitObject(const std::string &tree_sha, const std::string &message, const std::string &parent);

	std::string write() const;
};
