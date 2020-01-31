# unicode-connect-four
A C Implementation of connect four using unicode characters for the display

## Contents
1. [Installation](#Installation)
2. [Notes](#Notes)

## [Installation](#Installation)
To install you need to have `make` installed, so on your respective linux distro, make sure you have installed `build-essential` package using your respective package manager.

Once installed run `make` or `make all` or `make main` to build the program. To run the program simply execute the `main` executable created from running `make`.


## [Notes](#Notes)
This program makes extensive use of unicode to see how it works, first look at [main.c](./main.c) and then look at [utf8.c](./utf8.c) and [utf8.h](./utf8.h) for implementation details. I used the notes and source code from this website to help me:

[https://www.cprogramming.com/tutorial/unicode.html](https://www.cprogramming.com/tutorial/unicode.html)

If you have an trouble or need help feel free to open an issue.
