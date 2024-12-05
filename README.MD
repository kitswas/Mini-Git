# Mini Git

A baby version of Git.  
This project was created for an assignment in a graduate course on Operating Systems.  
For educational purposes.

> [!CAUTION]
> Warranty NOT included. Use at your own risk.

## Setting up your workspace

This project has a [.editorconfig file](https://editorconfig.org/) to enforce project level coding standards.  
CLion has built-in support,
VSCode requires [a plugin](https://marketplace.visualstudio.com/items?itemName=EditorConfig.EditorConfig).

## Installing dependencies

```sh
sudo apt-get install zlib1g-dev
```

## How to run

> [!IMPORTANT]
> Requires a POSIX compliant system.

This project requires [CMake](https://cmake.org/) to build.
Your IDE (VSCode or CLion) should automatically detect the CMakeLists.txt file and build the project.
Install extensions for CMake support if prompted.  
If you are using the command line, you can run the following commands:

```bash
cmake -B build
cmake --build build --config Release
./build/mygit
```

### Supported Commands

1. `./mygit init`
    - Initializes a new repository by creating a directory called .mygit.
2. `./mygit hash-object [-w] test.txt`
    - This command calculates the SHA-1 hash of a file, compresses it, and stores it as a blob object in the repository when the -w option is used. If -w is not included, it simply prints the file's SHA-1 hash.
3. `./mygit cat-file <flag> <file_sha>`
    - This command reads and displays the content or metadata of a file stored as an object using its SHA-1 hash.
    - Flags:
      - `-p` prints the content of the file.
      - `-t` prints the type of the object.
      - `-s` prints the size of the object.
4. `./mygit write-tree`
    - This command writes the current directory structure to a newly-created tree object.
5. `./mygit ls-tree [--name-only] <tree_sha>`
    - This command reads and displays the content of a tree object using its SHA-1 hash.
    - The `--name-only` flag prints only the names of the files in the tree.
6. `./mygit add <path>`
    - This command adds files and directories to the staging area.
7. `./mygit commit -m "commit message"`
    - This command creates a new commit object from the current staging area and writes it to the repository.
8. `./mygit log`
    - This command displays the commit history of the repository. Branches are not supported. So, the history is linear.
9. `./mygit checkout <commit_sha>`
    - This command checks out a specific commit, restoring the state of the project as it was at that
commit.

## Generating and Viewing Documentation

This project uses [Doxygen](https://www.doxygen.nl/index.html) to generate documentation.  
If Doxygen is available on your system,  
You can generate the documentation by running the `doc` CMake target.

```bash
cmake -B build
cmake --build build --target doc
```

This repository also has an automated workflow to generate documentatation via Github Actions.  

The generated documentation can be viewed at [/docs](./docs/index.html).

```bash
pushd docs ; python3 -m http.server 9999; popd # if you have python installed and want to use a server
```

```bash
open docs/index.html # or open the file from the OS file manager, js will not work
```

A good starting point to explore the codebase is the [file listing page](./docs/files.html).
([files.html](files.html) if you are viewing this in a browser)
