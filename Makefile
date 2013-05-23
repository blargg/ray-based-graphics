CC=g++
FLAGS=-g -fopenmp -O2 -std=c++11
WARNINGS= -Wall -Wno-unused-local-typedefs -Werror

LIBS= -lpng
EIGEN_DIR=/usr/include/eigen3
INCS= -I /usr/include/eigen3
COMPILE=$(CC) $(FLAGS) $(INCS) $(WARNINGS) -c

LINKER=g++
LINKER_FLAGS= -fopenmp
LK=$(LINKER) $(LINKER_FLAGS)

TEST_OPTIONS= -lgtest $(INCS)

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

$(RTEXE) : main.o raytracer.o loader.o camera.o simpleObject.o $(SHAPES_OBJ) shape.o $(MATERIALS_OBJ) easypng.o properties.o perlin.o AreaLight.o film.o ray.o common.o
	$(LK) $(LIBS) -o $@ $^

camera.o : camera.cpp camera.h
	$(COMPILE) $<

film.o : film.cpp film.h easypng.h color.h
	$(COMPILE) $<

main.o : main.cpp
	$(COMPILE) $<

raytracer.o : raytracer.cpp raytracer.h drawable.h easypng.o light.h properties.h ray.h
	$(COMPILE) $<

############### Geometry ####################
shape.o : shape.cpp shape.h
	$(COMPILE) $<

$(SHAPE_DIR)/%.o : $(SHAPE_DIR)/%.cpp $(SHAPE_DIR)/%.h shape.h ray.h
	$(COMPILE) $< -o $@

############### Drawing Objects #################
simpleObject.o : simpleObject.cpp simpleObject.h drawable.h material.h ray.h
	$(COMPILE) $<

bumpy_sphere.o : bumpy_sphere.cpp bumpy_sphere.h drawable.h material.h shapes/sphere.h $(MATH_DEPS)
	$(COMPILE) $<

### Matherials and textures
$(MATERIAL_DIR)/%.o : $(MATERIAL_DIR)/%.cpp $(MATERIAL_DIR)/%.h material.h perlin.h
	$(COMPILE) $< -o $@

perlin.o : perlin.cpp perlin.h
	$(COMPILE) $<

easypng.o : easypng.h easypng.cpp
	$(COMPILE) easypng.cpp

########################## Color and Properties ############################################
properties.o : properties.cpp properties.h color.h
	$(COMPILE) $<

loader.o : loader.cpp loader.h
	$(COMPILE) $<

AreaLight.o : AreaLight.cpp AreaLight.h
	$(COMPILE) $<

ray.o : ray.cpp ray.h
	$(COMPILE) $<

common.o : common.cpp common.h
	$(COMPILE) $<

########################## Test Cases ########################################
tests : Tests/Sphere.test Tests/Plane.test Tests/Triangle.test\
 Tests/Loader.test Tests/AreaLight.test

Tests/Ray.test : Tests/test_ray.cpp ray.o
	g++ -o $@ $^ $(TEST_OPTIONS)

Tests/Sphere.test : Tests/test_sphere.cpp shapes/sphere.o shape.o
	g++ -o $@ $^ $(TEST_OPTIONS)

Tests/Plane.test : Tests/test_plane.cpp shapes/plane.o shape.o
	g++ -o $@ $^ $(TEST_OPTIONS)

Tests/Triangle.test : Tests/test_triangle.cpp shapes/triangle.o shape.o
	g++ -o $@ $^ $(TEST_OPTIONS)

Tests/Loader.test : Tests/test_loader.cpp loader.o shape.o simpleObject.o $(SHAPES) materials/solidColor.o
	g++ -o $@ $^ $(TEST_OPTIONS)

Tests/AreaLight.test : Tests/test_arealight.cpp AreaLight.o
	g++ -o $@ $^ $(TEST_OPTIONS)

############# Makefile utilities ################
.PHONY : clean

clean :
	rm -f *.o $(RTEXE)
	rm -f $(SHAPE_DIR)/*.o $(MATERIAL_DIR)/*.o
