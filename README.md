# e172

[![Test Status](https://github.com/burbokop/e172/workflows/Ubuntu-22.04/badge.svg?event=push)](https://github.com/burbokop/e172/actions/workflows/ubuntu-22.04.yml)
[![Test Status](https://github.com/burbokop/e172/workflows/Windows-2022/badge.svg?event=push)](https://github.com/burbokop/e172/actions/workflows/windows-2022.yml)

Library provides tools for game gevelopment and api for video/audio output and keyboard input.</br>
This lib can be called GAME ENGINE.</br>


## adding e172 to your c++ project

1. `git submodule add https://github.com/burbokop/e172.git`</br>
2. in `CMakeLists.txt`:

```
include(${CMAKE_CURRENT_LIST_DIR}/e172/e172.cmake)
target_link_libraries(your_executable_or_lib
    e172
)

```
