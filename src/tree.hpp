#pragma once

#include <string>
#include <map>
#include <vector>

/**
 * @brief
 * Here's what a tree object file looks like (before Zlib compression).
 * Note that this is slightly different from the actual format used by Git,
 * as it is modified for our use case.
 *
 * ```
 * tree <size>\0
 * <mode> <path>\0<20_byte_sha>
 * <mode> <path>\0<20_byte_sha>
 * ```
 *
 * (The above code block is formatted with newlines for readability, but the actual file doesn't contain newlines)
 *
 * - The file starts with `tree <size>\0`. This is the "object header", similar to what we saw with blob objects.
 *
 * - After the header, there are multiple entries. Each entry is of the form `<mode> <path>\0<sha>`.
 *   - `<mode>` is the mode of the file/directory
 *   - `<path>` is the path of the file/directory
 *   - `\0` is a null byte
 *   - `<20_byte_sha>` is the 20-byte SHA-1 hash of the blob/tree (in hexadecimal format)
 */
class TreeObject
{
public:
	struct EntryData
	{
		std::string mode;
		std::string sha;
		friend constexpr auto operator<=>(const EntryData &lhs, const EntryData &rhs) noexcept
		{
			// Compare the entries based on their SHA-1 hashes
			return lhs.sha <=> rhs.sha;
		}
	};

	/**
	 * @brief Adds a new entry to the tree object or updates an existing entry.
	 *
	 * @param mode The mode of the file/directory
	 * @param path The path of the file/directory. Will be used as a key to look up the entry
	 * @param sha The SHA-1 hash of the file/directory (in hexadecimal format)
	 */
	void add_or_update_entry(const std::string &mode, const std::string &path, const std::string &sha);

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

	std::map<std::string, EntryData> get_entries() const;

	/**
	 * @brief Reads a tree object from the disk.
	 */
	static TreeObject read(const std::string &file_path);

	/**
	 * @brief Writes the tree object to the disk.
	 */
	std::string write() const;

private:
	std::map<std::string, EntryData> entries;
};
