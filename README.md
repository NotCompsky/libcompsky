# Description

This package is just a collection of a few bits and bobs used in various projects of mine.

# Installing

## Ubuntu and other Debian-derived systems

    regexp="https://github\.com/NotCompsky/libcompsky/releases/download/[0-9]\.[0-9]\.[0-9]/libcompsky-[0-9]+\.[0-9]+\.[0-9]+-$(dpkg --print-architecture)\.deb"
    url=$(curl -s https://api.github.com/repos/NotCompsky/libcompsky/releases/latest  |  egrep "$regexp" | sed 's%.*"\(https://.*\)"%\1%g')
    curl "$url" -o /tmp/libcompsky.deb
    sudo apt install /tmp/libcompsky.deb

Users of other Debian-based distributions may have to modify the dependency package names before the packages will install (you could use `equivs` if you don't want to modify the `deb` itself). Send a bug report with your platform and a suggestion for the alternative dependency.

If installation still fails for some reason, see [installing on Ubuntu](INSTALLING_UBUNTU.md) (and also make a bug report).

## Other

You should compile from source. But if there's enough interest, I'll distribute Windows installers too.

# mysql

This is the main feature of the library. It is a wrapper around `libmysqlclient` that - in my opinion - has a far simpler syntax. And presumably far fewer features, although of course you still have access to the underlying `libmysqlclient` functions.

It uses `asciify` (another part of this project) to format the strings sent to the MySQL server.

## Dependencies

* libmysqlclient

## Usage Examples

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


`init` and `exit` are a pair, and entirely optional - you can just connect directly via `libmysqlclient`. However, if used, the config file must have exactly the correct syntax.

For convenience, `compsky_mysql_create_config` provides a way to make very simple programs to initialise the database, its tables, and its config file for you, for a given project:

    #include <compsky/mysql/create_config.hpp>
    
    int main(){
        compsky::mysql::create_config(
            #include "init.sql"
            , "PROJECT_MYSQL_CFG_PATH"
        );
        return 0;
    }

Here `init.sql` is a file next to the source file that only contains the SQL commands to create tables, in a raw string.

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

# security

Contains the `memzero_secure` function based on the work of `Zhaomo Yang` that he released into the Public Domain. It is used to wipe `mysql` authentification details when `compsky::mysql::exit()` is called.

# Building

## Unix

    git clone https://github.com/NotCompsky/libcompsky
    cd libcompsky
    mkdir build
    cd build
    cmake ..
    sudo cmake install

## Windows

### Cross Compiling from Linux

You must use MXE, as the standard MinGW tools on Ubuntu do not include things such as libmysqlclient. If you haven't installed it already, allocate an hour or so (and ~3GB) for it to download and build all dependencies.

    git clone https://github.com/mxe/mxe
    cd mxe
    make MXE_TARGETS=x86_64-w64-mingw32.static
    make libmysqlclient

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
