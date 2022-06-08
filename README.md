A simple SQL like database management system
* C++
* wxWidgets 3.0.5

## Getting Started

1. clone the repo

```bash
git clone https://github.com/khaled-hassen/database.git
cd database
```
2. setup the MSVC2019 compiler + wxWidgets
2.1 Download  [the windows binaries](https://www.wxwidgets.org/downloads/): header files, development files(x86_64) and release dlls(x86_64)
2.2 Create a new folder named libs (eg: in C:/ directory) that will contain all wxWidgets files and add a new wdWidgets folder inside libs/
2.3 extract the development files into libs/wxWidgets/lib
2.4 extract the release dlls into libs/wxWidgets/lib
2.5 extract the header files into libs/wxWidgets/include
2.6 add a new environment variable named WX_WID with the value of the path of wxWidgets (eg: C:/libs/wxWidgets)
