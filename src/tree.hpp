#pragma once

#include <string>
#include <set>
#include <vector>

/**
 * @brief
 * Here's what a tree object file looks like (before Zlib compression):
 *
 * ```
 * tree <size>\0
 * <mode> <name>\0<20_byte_sha>
 * <mode> <name>\0<20_byte_sha>
 * ```
 *
 * (The above code block is formatted with newlines for readability, but the actual file doesn't contain newlines)
 *
 * - The file starts with `tree <size>\0`. This is the "object header", similar to what we saw with blob objects.
 *
 * - After the header, there are multiple entries. Each entry is of the form `<mode> <name>\0<sha>`.
 *   - `<mode>` is the mode of the file/directory
 *   - `<name>` is the name of the file/directory
 *   - `\0` is a null byte
 *   - `<20_byte_sha>` is the 20-byte SHA-1 hash of the blob/tree (this is not in hexadecimal format)
 */
class TreeObject
{
public:
	struct Entry
	{
		const std::string mode;
		const std::string name;
		const std::string sha;
		friend constexpr auto operator<=>(const Entry &lhs, const Entry &rhs) noexcept
		{
			// Compare the entries based on their names, then SHA-1 hashes
			return lhs.name <=> rhs.name == 0 ? lhs.sha <=> rhs.sha : lhs.name <=> rhs.name;
		}
	};

	/**
	 * @brief Adds a new entry to the tree object.
	 *
	 * @param mode The mode of the file/directory
	 * @param name The name of the file/directory
	 * @param sha The SHA-1 hash of the file/directory (not in hexadecimal format)
	 */
	void add_entry(const std::string &mode, const std::string &name, const std::string &sha);

	/**
	 * @brief Creates a tree object from the given path.
	 *
	 * @param path The path to the directory whose tree object is to be created
	 * @return The tree object
	 */
	static TreeObject create_tree(const std::string &path);

	/**
	 * @brief Returns the tree as a vector of characters.
	 * This cannot be a string because the data contains null bytes.
	 */
	std::vector<char> get_data() const;

	std::set<Entry> get_entries() const;

	/**
	 * @brief Reads a tree object from the disk.
	 */
	static TreeObject read(const std::string &file_path);

	/**
	 * @brief Writes the tree object to the disk.
	 */
	std::string write() const;

private:
	std::set<Entry> entries;
};
