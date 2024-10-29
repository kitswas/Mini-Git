#pragma once

#include <string>

/**
 * @brief Stores a file as a blob object in the repository.
 *
 * @details
 * Each Git Blob is stored as a separate file in the .git/objects directory. The file contains a header and the contents of the blob object, compressed using Zlib.
 *
 * The format of a blob object file looks like this (after Zlib decompression):
 *
 * ```
 *   blob <size>\0<content>
 * ```
 *
 *   - `<size>` is the size of the content (in bytes)
 *   - `\0` is a null byte
 *   - `<content>` is the actual content of the file
 *
 * For example, if the contents of a file are hello world, the blob object file would look like this (after Zlib decompression):
 * ```
 *   blob 11\0hello world
 * ```
 *
 * @param path Path to the file
 * @param sha1 Precomputed SHA-1 hash of the file contents (to avoid recomputing it)
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
int store_file_as_blob(const std::string &path, const std::string &sha1);
