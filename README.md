Ray Based Graphics
==================

### Project Description
This is about the design of rendering algorithms,
with a focus on photorealism.

### Features
* physically based rendering
* optimization with kd trees
* support for obj files

### Building and Running
Currently, only linux is officially supported. There are no plans to change this
in the future.

Building requires the following libraries
* [libpng](http://www.libpng.org/pub/png/libpng.html)
* [eigen3](http://eigen.tuxfamily.org/index.php?title=Main_Page)
* [googletest](https://code.google.com/p/googletest/) (only needed for tests)

To build the project:
```
make
```

Example to render a scene:
```
./render -p 100 -o output.png scene.obj
```

This will render the scene defined by 'scene.obj' with 100 samples per pixel.
The rendered image will be written to output.png

### Making Scene Files
The scene files that can be loaded are in obj format only. The only alteration
made to the format is that the ambient term has be repurposed as the emissive
term. Obj format can be handled by most 3d modeling software.

### Testing
Testing requires googletest.

To build the tests:
```
make tests
```
This will compile all the tests in the Tests folder.

To build and run all tests:
```
make run_tests
```
