# 42sh
POSIX standarts compliant shell implementation in C language.\
\
Implementation of several commands :
- if, else, elif, fi
- while / until, do, done
- true / false
- echo
- cd
- builtin commands

## Installation
To download our project:
```bash
wget https://github.com/TRKirua/42sh/archive/refs/heads/main.zip -O "42sh.zip" &&
unzip ./"42sh.zip" &&
rm ./"42sh.zip"
```

To compile our project:
```bash
meson setup builddir
```

Then:
```bash
ninja -C builddir
```

## Usage
To run our project, you have several options:\
\
To launch in interactive mode, just do:
```bash
./builddir/42sh
```

To launch by using a file:
```bash
./builddir/42sh <your_file>
```

Or directly using stdin with a file:
```bash
./builddir/42sh < <your_file>
```

You can also do:
```bash
./builddir/42sh -c "<your_command(s)>"
```

## Clean project
To clean the project:
```bash
rm -r builddir
```

## Credits :
 * [Enes Ekici](https://github.com/TRKirua)
 * [Emon Barberis](https://github.com/EmonBar)
 * [Samy Abou-al-Tout](https://github.com/locovamos)
 * [Jean-Marc Twagirakristu](https://github.com/Watcher2025)
