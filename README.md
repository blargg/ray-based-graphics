ray-based-rendering
===================

Project Overview
-----------------
This is a simple ray-tracing project. It focuses on clean and easy to program
code rather than speed. Currently, only ray-tracing is being developed, but I
expect to transition into a path-tracer later.

Compiling
----------
Requires that you have gcc (for g++) and libpng on your system.
In the project directory run:

	make
	./render

to make and run the program. The renderer works on a hard coded list, so to 
edit the scene you must alter main.cpp and recompile the project.

Documentation
-------------
The code is documented in a Doxygen format. Use doxygen to generate the
documentation. It can be used as a reference for the code.

To make the documentation, first check that doxygen is installed then run the following.
(This may also require latex)

	doxygen Doxyfile

Todo's are included in the code. Search for 'TODO'
