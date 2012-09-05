#TODO Use the system provided makefile variables for the compiler
#TODO Figure out how to clean this up.

CC=g++
FLAGS=-g
WARNINGS= -Wall -Werror
LIBS= -lpng
COMPILE=$(CC) $(FLAGS) $(WARNINGS) -c
#COMPILE=$(CC) $(FLAGS) $(WARNINGS) $(LIBS) -c #TODO don't know why LIBS was included. Removing

LK=g++

# These classes are templated and are dependencies of any class that uses them
# This variable is for convenience and readability
MATH_DEPS=point.cpp point.h vectre.cpp vectre.h ray.cpp ray.h

RTEXE=render

# allows you to switch which is compiled with 'make' with no args
first : $(RTEXE)

$(RTEXE) : main.o raytracer.o simpleObject.o sphere.o plane.o shape.o solidColor.o easypng.o properties.o turbulent.o perlin.o
	$(LK) $(LIBS) -o $@ $^

#TODO really, i think main.o only really depends on the .h files rather than the .o files.
main.o : main.cpp raytracer.o drawable.h simpleObject.o sphere.o solidColor.o properties.h easypng.h light.h turbulent.h $(MATH_DEPS)
	$(COMPILE) $<

raytracer.o : raytracer.cpp raytracer.h drawable.h easypng.o light.h properties.h $(MATH_DEPS)
	$(COMPILE) $<

############### Geometry ####################
shape.o : shape.cpp shape.h $(MATH_DEPS)
	$(COMPILE) $<

sphere.o : sphere.cpp sphere.h shape.o $(MATH_DEPS)
	$(COMPILE) $<

plane.o : plane.cpp plane.h shape.h $(MATH_DEPS)
	$(COMPILE) $<

############### Drawing Objects #################
simpleObject.o : simpleObject.cpp simpleObject.h drawable.h material.h $(MATH_DEPS)
	$(COMPILE) $<

drawable.h : properties.h
	touch $@

	### Matherials and textures
solidColor.o : solidColor.cpp solidColor.h material.h $(MATH_DEPS)
	$(COMPILE) $<

turbulent.o : turbulent.cpp turbulent.h perlin.h material.h $(MATH_DEPS)
	$(COMPILE) $<

perlin.o : perlin.cpp perlin.h $(MATH_DEPS)
	$(COMPILE) $<

checkerBoardTexture.o : checkerBoardTexture.cpp checkerBoardTexture.h material.h
	$(COMPILE) $<

material.h : properties.h $(MATH_DEPS)
	touch $@

easypng.o : easypng.h easypng.cpp
	$(COMPILE) easypng.cpp

########################## Color and Properties ############################################
properties.o : properties.cpp properties.h color.h
	$(COMPILE) $<

#properties.h : color.h
	#touch $@

############# Makefile utilities ################
.PHONY : clean

clean :
	rm -f *.o $(RTEXE)
