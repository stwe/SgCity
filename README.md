# SgCity

Tile based 3D terrain maked with OpenGL.

The individual tiles can be raised and lowered with the mouse, creating a
look and feel of old city building simulations from the 90s.

<img src="https://github.com/stwe/SgCity/blob/main/resources/devlog/2022-01-01.png" width="500" height="400" />

## Requirements

- [GLFW](https://www.glfw.org/)
- [GLEW](http://glew.sourceforge.net/)
- [GLM](https://github.com/g-truc/glm)
- [spdlog](https://github.com/gabime/spdlog)
- [Dear ImGui](https://github.com/ocornut/imgui)
- C++ 17 Compiler

## Platforms

- Linux

## Build instructions

First, [install](https://docs.conan.io/en/latest/installation.html) the Conan package manager locally.

If you are using GCC compiler >= 5.1, Conan will set the `compiler.libcxx` to the old ABI for backwards compatibility. You can change this with the following commands:

```bash
$ conan profile new default --detect  # Generates default profile detecting GCC and sets old ABI
$ conan profile update settings.compiler.libcxx=libstdc++11 default  # Sets libcxx to C++11 ABI
```

As this project relies on multiple 3rd-Party Libs, I created a `conanfile.txt` with all the requirements.

```txt
[requires]
glfw/3.3.5
glew/2.2.0
glm/0.9.9.8
spdlog/1.9.2
imgui/1.86

[generators]
cmake
```

Complete the installation of requirements for the project running:

```bash
$ conan install conanfile.txt -s build_type=Debug --build missing
```

or

```bash
$ conan install conanfile.txt -s build_type=Release --build missing
```

**If the following error occurs: `ERROR: opengl/system: Error in package_info() method, line 80`, then `pkg-config` must be installed first.**

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
$ cd src/bin
$ ./SgCity
```

## License

SgCity is licensed under the GPL-2.0 License, see [LICENSE](https://github.com/stwe/SgCity/blob/main/LICENSE) for more information.
