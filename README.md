# Description

This package is just a collection of a few bits and bobs used in various projects of mine.

## mysql

This is the main feature of the library. It is a wrapper around `libmysqlclient` that - in my opinion - has a far simpler syntax. And presumably far fewer features, although of course you still have access to the underlying `libmysqlclient` functions.

It uses `asciify` (another part of this project) to format the strings sent to the MySQL server.

### Usage Examples

    #include <compsky/mysql/query.hpp>
    
    MYSQL_RES* RES;
    MYSQL_ROW ROW;
    
    compsky::mysql::init("/path/to/file.cfg");
    
    compsky::mysql::query(&RES, "SELECT id, name FROM ",  table,  " WHERE id < ",  100);
    
    uint64_t id;
    char* name;
    while(compsky::mysql::assign_next_row(RES, &ROW, &id, &name))
        std::cout << +id << ": " << name << std::endl;
    
    compsky::mysql::exit();

# asciify

## Benchmarks

Using [libfmt's benchmark](https://github.com/fmtlib/format-benchmark) patched to include `compsky::asciify`, I get the following results on my machine (Intel Ubuntu 18.04):

| real | user | sys |
| - | - | - |
| printf | 1.07 | 1.06 |
| iostreams | 1.76 | 1.76 |
| format | 1.01 | 1.01 |
| fmt::prepare | 0.93 | 0.86 |
| tinyformat | 2.17 | 2.16 |
| boost | 6.19 | 6.18 |
| compsky::asciify | 0.32 | 0.31 |
| folly | N/A | N/A |
| stb_sprintf | 0.68 | 0.68 |

This benchmark is just to satisfy curiosity. I don't think `compsky::asciify` is particularly useful to anyone - if you look at the output, you can see that the floats are not printed exactly, with `1.2339999999` instead of `1.2340000000` and (more worryingly) `3.12` instead of `+3.13`. Perhaps this is something that can be fixed, but I would expect many other flaws exist, and suggest that `compsky::asciify` is only useful where both a *nearly* correct answer is acceptable and performance is critical and you have a relatively new compiler (i.e. almost never).

You can test it yourself using [the patch](3rdparty/patches/format-benchmark/0001-Added-compsky-asciify.patch) and applying it in the `format-benchmark` directory with `git am < /PATH/TO/0001-Added-compsky-asciify.patch`).

# Building

## Dependencies

* C++17 standard compliant compiler

Packages required for GUI:

* libqt5

Packages required for MySQL:

* libmysqlclient

## Building

### Unix

Navigate to this project's root directory and run:

    mkdir build
    cd build
    cmake ..
    sudo cmake install

### Windows

The recommended way of building for Windows is using `MXE` on a Unix system. I have not successfully build it with Visual Studio Code 2015 on my Windows machine, though I spent a day trying and got close.

### MXE

You must use MXE, as the standard MinGW tools on Ubuntu do not include things such as libmysqlclient. If you haven't installed it already, allocate an hour or so (and ~3GB) for it to download and build all dependencies.
    
    cd ..
    git clone https://github.com/mxe/mxe
    cd mxe
    make MXE_TARGETS=x86_64-w64-mingw32.static
    make qtbase libmysqlclient

Ensure MXE's `/usr/bin` is in your `PATH`:

    export PATH="$PATH:$PWD/usr/bin"

Now build libcompsky:

    cd ..
    git clone https://github.com/compsky/libcompsky
    cd libcompsky
    mkdir build
    cd build
    x86_64-w64-mingw32.static-cmake ..
    make
    sudo make install

#### Native

##### VS Code

###### Actually Building It

Right click on the `Command Prompt for VS` and run as admin.

    mkdir build
    cd build
    cmake --config Release -G "Visual Studio 15 2017 Win64" ..
    cmake --build . --config Release --target INSTALL

###### If things go wrong

The expected output includes lots of warnings - `'fopen' is unsafe`, `conversion from 'size_t' to 'unsigned long'`, `format string requires an argument of 'unsigned long'` etc., which can all be safely ignored.

If `mysql.h` cannot be found, run the `cmake --config Release ...` command again but with `-DWIN_MYSQL_DIR=<value>`, replacing `<value>` with the path to your MySQL server directory, replacing the backslashes with forward slashes - the default is `C:/Program Files/MySQL/MySQL Server 8.0`.

If you get the `library machine type 'x64' conflicts with target machine type 'x86'` warning, you forgot to specify `Win64` in the generator `-G` option before.

The generator must be set to 64 bit otherwise VC will force a x86 build (even if the x64 command prompt is used), and that causes undefined reference (linking) errors.

Note that `--config Release` must be used because `-DCMAKE_BUILD_TYPE` is ignored. Debug builds are not possible in VC builds on Windows because VC believes it can only have the same debug level as libmysqlclient (which is Release).

The directories must be explicitly stated because the find_package command does not find them, even if the FindMySQL.cmake file from the CMake community wiki is copied into the CMake Modules folder. 

###### Rebuilding Libraries

Despite forcing `lib` files to be built - compsky_mysql could not compile if we didn't - these `lib` files are deleted when CMake has finished building, and are not installed. Hence our libraries are unusable by other programs.

The `lib` files need to be rebuilt. 
