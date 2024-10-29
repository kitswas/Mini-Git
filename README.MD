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
