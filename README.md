# Toy Raytracer

Based on Peter Shirley's [Ray Tracing in One Weekend](https://github.com/petershirley/raytracinginoneweekend) and [Raytracing the Next Week](https://github.com/petershirley/raytracingthenextweek).

## Setting up a scene

You can setup and render a scene 2 ways -- either by importing the headers from `include/raytracer` or by building the binary and running it with your `.scene` file.

### Scene Files

Scene files are simple text files that describe the scene to be rendered. You can find demos in the `.demos` folder.

### C++ API

You can also build scenes with the C++ API, you can find demos of this in the `api.cpp` file.

### Files

**Includes**:
  * raytracer -- Folder with raytracer header files
  * scene -- Folder with scene builder, takes .scene files and renders them
    - The Builder class creates and runs the Parser class then builds the Program class to render the image
    - The parser adds objects, textures, strings, numbers, etc. in an Environment class instance to build the scene with
  * stbi -- stbi image library
  * cli.h -- CLI header file
