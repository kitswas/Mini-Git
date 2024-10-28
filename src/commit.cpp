#include "commit.hpp"
#include <filesystem>
#include <format>
#include "sha.hpp"
#include <zstr.hpp> // ZLib C++ wrapper. See https://github.com/mateidavid/zstr

CommitObject::CommitObject(const TreeObject &tree, const std::string &message, std::string parent = "")
	: tree(tree),
	  message(message),
	  parent(parent)
{
	timestamp = std::chrono::system_clock::now();
}

std::string CommitObject::write() const
{
	std::string commitData;
	commitData += "tree " + tree_sha + '\n';
	if (!parent.empty())
	{
		commitData += "parent " + parent + '\n';
	}
	commitData += "author " + author.name + " <" + author.email + "> " + std::format("{} +0000", std::chrono::duration_cast<std::chrono::seconds>(timestamp.time_since_epoch()).count()) + '\n';
	commitData += "committer " + author.name + " <" + author.email + "> " + std::format("{} +0000", std::chrono::duration_cast<std::chrono::seconds>(timestamp.time_since_epoch()).count()) + '\n';
	commitData += '\n' + message + '\n';

	std::string commitSha = get_sha1(std::vector<char>(commitData.begin(), commitData.end()));
	std::filesystem::create_directory(".mygit/objects/" + commitSha.substr(0, 2));
	zstr::ofstream commitFile(".mygit/objects/" + commitSha.substr(0, 2) + "/" + commitSha.substr(2), std::ios_base::trunc);
	if (!commitFile.is_open())
	{
		std::cerr << "Failed to open commit object file " << commitSha << " for writing\n";
		exit(EXIT_FAILURE);
	}
	commitFile.write(commitData.data(), commitData.size());
	commitFile.close();

	return commitSha;
}
