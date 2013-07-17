CC=g++
FLAGS=-g -D NDEBUG -fopenmp -std=c++11 -O2
WARNINGS= -Wall -Wno-unused-local-typedefs -Werror

LIBS= -lpng
EIGEN_DIR=/usr/include/eigen3
INCS= -I $(EIGEN_DIR)
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

.PHONY: debug $(RTEXE)

$(RTEXE) : main.o raytracer.o obj_loader.o mtl_loader.o camera.o simpleObject.o $(SHAPES_OBJ) shape.o $(MATERIALS_OBJ) easypng.o properties.o perlin.o AreaLight.o film.o ray.o common.o
	$(LK) $(LIBS) -o $@ $^

debug : LINKER_FLAGS += -ltcmalloc -lprofiler
debug : FLAGS += -U NDEBUG
debug : $(RTEXE)

camera.o : camera.cpp camera.h
	$(COMPILE) $<

film.o : film.cpp film.h easypng.h color.h
	$(COMPILE) $<

main.o : main.cpp obj_loader.h common.h film.h
	$(COMPILE) $<

raytracer.o : raytracer.cpp raytracer.h drawable.h easypng.o light.h properties.h ray.h common.h
	$(COMPILE) $<

############### Geometry ####################
shape.o : shape.cpp shape.h
	$(COMPILE) $<

$(SHAPE_DIR)/%.o : $(SHAPE_DIR)/%.cpp $(SHAPE_DIR)/%.h shape.h ray.h
	$(COMPILE) $< -o $@

############### Drawing Objects #################
drawable.h : aabb.h
	touch $@

simpleObject.o : simpleObject.cpp simpleObject.h drawable.h material.h ray.h
	$(COMPILE) $<

### Matherials and textures
$(MATERIAL_DIR)/%.o : $(MATERIAL_DIR)/%.cpp $(MATERIAL_DIR)/%.h material.h perlin.h
	$(COMPILE) $< -o $@

perlin.o : perlin.cpp perlin.h
	$(COMPILE) $<

easypng.o : easypng.h easypng.cpp
	$(COMPILE) easypng.cpp

########################## Color and Properties ###############################
properties.o : properties.cpp properties.h color.h
	$(COMPILE) $<

obj_loader.o : obj_loader.cpp obj_loader.h mtl_loader.h shapes/triangle.h simpleObject.h
	$(COMPILE) $<

mtl_loader.o : mtl_loader.cpp mtl_loader.h properties.h materials/solidColor.h simpleObject.h
	$(COMPILE) $<

AreaLight.o : AreaLight.cpp AreaLight.h
	$(COMPILE) $<

ray.o : ray.cpp ray.h
	$(COMPILE) $<

kdtree.o : kdtree.cpp kdtree.h aabb.h ray.h drawable.h
	$(COMPILE) $<

common.o : common.cpp common.h
	$(COMPILE) $<

########################## Test Cases ########################################
TEST_EXES=Tests/Sphere.test Tests/Triangle.test\
		  Tests/ObjLoader.test Tests/AreaLight.test Tests/Common.test\
		  Tests/MtlLoader.test Tests/KDTree.test

run_tests : tests
	$(foreach test, $(TEST_EXES), ./$(test) ;)

tests : $(TEST_EXES)

Tests/Common.test : Tests/test_common.cpp common.o
	g++ -o $@ $^ $(TEST_OPTIONS)

Tests/Ray.test : Tests/test_ray.cpp ray.o
	g++ -o $@ $^ $(TEST_OPTIONS)

Tests/Sphere.test : Tests/test_sphere.cpp shapes/sphere.o shape.o
	g++ -o $@ $^ $(TEST_OPTIONS)

Tests/Triangle.test : Tests/test_triangle.cpp shapes/triangle.o shape.o common.o
	g++ -o $@ $^ $(TEST_OPTIONS)

Tests/ObjLoader.test : Tests/test_obj_loader.cpp obj_loader.o mtl_loader.o materials/solidColor.o shapes/triangle.o simpleObject.o common.o shape.o shapes/sphere.o
	g++ -o $@ $^ $(TEST_OPTIONS)

Tests/MtlLoader.test : Tests/test_mtl_loader.cpp mtl_loader.o materials/solidColor.o shapes/triangle.o simpleObject.o common.o shape.o shapes/sphere.o
	g++ -o $@ $^ $(TEST_OPTIONS)

Tests/AreaLight.test : Tests/test_arealight.cpp AreaLight.o common.o
	g++ -o $@ $^ $(TEST_OPTIONS)

Tests/KDTree.test : Tests/test_kdtree.cpp kdtree.o obj_loader.o mtl_loader.o materials/solidColor.o shapes/triangle.o simpleObject.o common.o shape.o shapes/sphere.o
	g++ -o $@ $^ $(TEST_OPTIONS)

############# Makefile utilities ################
.PHONY : clean

clean :
	rm -f *.o $(RTEXE)
	rm -f $(SHAPE_DIR)/*.o $(MATERIAL_DIR)/*.o
