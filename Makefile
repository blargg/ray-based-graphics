#TODO Use the system provided makefile variables for the compiler

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

$(RTEXE) : main.o raytracer.o simpleObject.o sphere.o shape.o solidColor.o easypng.o
	$(LK) $(LIBS) -o $@ $^

#TODO really, i think main.o only really depends on the .h files rather than the .o files.
main.o : main.cpp raytracer.o drawable.h simpleObject.o sphere.o solidColor.o easypng.h light.h $(MATH_DEPS)
	$(COMPILE) $<

raytracer.o : raytracer.cpp raytracer.h drawable.h easypng.o light.h structs.h $(MATH_DEPS)
	$(COMPILE) $<

#TODO old stuff
#$(RTEXE) : raytrace.o simpleObject.o shape.o sphere.o solidColor.o easypng.o checkerBoardTexture.o
#	$(LK) $(LIBS) -o $@ $^
#
#raytrace.o : raytrace.cpp easypng.o sphere.o drawable.h structs.h $(MATH_DEPS)
#	$(COMPILE) raytrace.cpp

############### Geometry ####################
shape.o : shape.cpp shape.h $(MATH_DEPS)
	$(COMPILE) $<

sphere.o : sphere.cpp sphere.h shape.o $(MATH_DEPS)
	$(COMPILE) $<

############### Drawing Objects #################
simpleObject.o : simpleObject.cpp simpleObject.h drawable.h material.h $(MATH_DEPS)
	$(COMPILE) $<

solidColor.o : solidColor.cpp solidColor.h material.h $(MATH_DEPS)
	$(COMPILE) $<

checkerBoardTexture.o : checkerBoardTexture.cpp checkerBoardTexture.h material.h
	$(COMPILE) $<

easypng.o : easypng.h easypng.cpp
	$(COMPILE) easypng.cpp

material.h : structs.h $(MATH_DEPS)
	touch $@

drawable.h : structs.h
	touch $@

############# Makefile utilities ################
.PHONY : clean

clean :
	rm -f *.o $(RTEXE)
