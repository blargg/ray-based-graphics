CC=g++
FLAGS=-g -fopenmp
WARNINGS= -Wall -Werror
LIBS= -lpng
COMPILE=$(CC) $(FLAGS) $(WARNINGS) -c

LINKER=g++
LINKER_FLAGS= -fopenmp
LK=$(LINKER) $(LINKER_FLAGS)

MATH_DEPS=point.cpp point.h vectre.cpp vectre.h ray.cpp ray.h

RTEXE=render

SHAPE_DIR=shapes
SHAPES=$(wildcard $(SHAPE_DIR)/*.cpp)
SHAPES_OBJ=$(SHAPES:.cpp=.o)

MATERIAL_DIR=materials
MATERIALS_SRC=$(wildcard $(MATERIAL_DIR)/*.cpp)
MATERIALS_OBJ=$(MATERIALS_SRC:.cpp=.o)

# allows you to switch which is compiled with 'make' with no args
first : $(RTEXE)

$(RTEXE) : main.o raytracer.o loader.o camera.o bumpy_sphere.o simpleObject.o $(SHAPES_OBJ) shape.o $(MATERIALS_OBJ) easypng.o properties.o perlin.o func.o AreaLight.o bounding_shape.o film.o
	$(LK) $(LIBS) -o $@ $^

camera.o : camera.cpp camera.h
	$(COMPILE) $<

film.o : film.cpp film.h easypng.h color.h
	$(COMPILE) $<

main.o : main.cpp
	$(COMPILE) $<

raytracer.o : raytracer.cpp raytracer.h drawable.h easypng.o light.h properties.h $(MATH_DEPS)
	$(COMPILE) $<

############### Geometry ####################
shape.o : shape.cpp shape.h $(MATH_DEPS)
	$(COMPILE) $<

$(SHAPE_DIR)/%.o : $(SHAPE_DIR)/%.cpp $(SHAPE_DIR)%.h shape.h $(MATH_DEPS)
	$(COMPILE) $< -o $@

############### Drawing Objects #################
simpleObject.o : simpleObject.cpp simpleObject.h drawable.h material.h $(MATH_DEPS)
	$(COMPILE) $<

bumpy_sphere.o : bumpy_sphere.cpp bumpy_sphere.h drawable.h material.h shapes/sphere.h $(MATH_DEPS)
	$(COMPILE) $<

bounding_shape.o : bounding_shape.cpp bounding_shape.h drawable.h material.h $(MATH_DEPS)
	$(COMPILE) $<

drawable.h : properties.h
	touch $@

	### Matherials and textures
$(MATERIAL_DIR)/%.o : $(MATERIAL_DIR)/%.cpp $(MATERIAL_DIR)/%.h material.h perlin.h $(MATH_DEPS)
	$(COMPILE) $< -o $@

perlin.o : perlin.cpp perlin.h $(MATH_DEPS)
	$(COMPILE) $<

material.h : properties.h $(MATH_DEPS)
	touch $@

easypng.o : easypng.h easypng.cpp
	$(COMPILE) easypng.cpp

########################## Color and Properties ############################################
properties.o : properties.cpp properties.h color.h
	$(COMPILE) $<

loader.o : loader.cpp loader.h
	$(COMPILE) $<

AreaLight.o : AreaLight.cpp AreaLight.h
	$(COMPILE) $<

func.o : func.cpp func.h
	$(COMPILE) $<
########################## Test Cases ########################################
tests : Tests/Sphere.test Tests/Plane.test Tests/Triangle.test\
 Tests/Loader.test Tests/AreaLight.test Tests/BoundingShape.test

Tests/Sphere.test : Tests/test_sphere.cpp shapes/sphere.o shape.o
	g++ -o $@ $^ -lgtest

Tests/Plane.test : Tests/test_plane.cpp shapes/plane.o shape.o
	g++ -o $@ $^ -lgtest

Tests/Triangle.test : Tests/test_triangle.cpp shapes/triangle.o shape.o
	g++ -o $@ $^ -lgtest

Tests/Loader.test : Tests/test_loader.cpp loader.o shape.o simpleObject.o $(SHAPES) materials/solidColor.o bounding_shape.o
	g++ -o $@ $^ -lgtest

Tests/AreaLight.test : Tests/test_arealight.cpp AreaLight.o func.o
	g++ -o $@ $^ -lgtest

Tests/BoundingShape.test : Tests/test_bounding_shape.cpp simpleObject.o materials/solidColor.o bounding_shape.o shape.o shapes/sphere.o
	g++ -o $@ $^ -lgtest

############# Makefile utilities ################
.PHONY : clean

clean :
	rm -f *.o $(RTEXE)
	rm -f $(SHAPE_DIR)/*.o $(MATERIAL_DIR)/*.o
