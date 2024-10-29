#include "commit.hpp"
#include <filesystem>
#include <format>
#include "sha.hpp"
#include <zstr.hpp> // ZLib C++ wrapper. See https://github.com/mateidavid/zstr

CommitObject::CommitObject(const std::string &tree_sha, const std::string &message, const std::string &parent) : tree_sha(tree_sha), message(message), parent(parent)
{
	timestamp = std::chrono::system_clock::now();
}

std::string CommitObject::write() const
{
	std::string commitData = "commit 0\n";
	commitData += "tree " + tree_sha + '\n';
	if (!parent.empty())
	{
		commitData += "parent " + parent + '\n';
	}
	commitData += std::format("author {} <{}> {:%Y-%m-%d %H:%M} UTC\n",
							  author.name,
							  author.email,
							  timestamp);
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

std::string get_head()
{
	std::ifstream head_file(".mygit/HEAD");
	std::string head;
	head_file >> head;
	head_file.close();
	return head;
}

void write_head(const std::string &commit_sha)
{
	std::ofstream head_file(".mygit/HEAD", std::ios::trunc);
	head_file << commit_sha << "\n";
	head_file.close();
}

int commit(int argc, char *argv[])
{
	std::string message;
	if (argc == 3 && std::string(argv[1]) == "-m")
	{
		message = argv[2];
	}

	if (std::filesystem::exists(".mygit/index") == false)
	{
		std::cerr << "Did you forget to run `add`?\n";
		return EXIT_FAILURE;
	}

	try
	{
		std::string parent = get_head();
		TreeObject tree = TreeObject::read(".mygit/index");
		const std::string tree_sha = tree.write();
		CommitObject commit(tree_sha, message, parent);
		std::string commit_sha = commit.write();
		std::cout << "Committed with SHA-1: " << commit_sha << '\n';
		write_head(commit_sha);
		std::filesystem::remove(".mygit/index");
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
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
