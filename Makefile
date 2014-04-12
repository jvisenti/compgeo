##debug 
CFLAGS = -g
##release
#CFLAGS = -O3 -DNDEBUG

CFLAGS += -Wall

## just use the same compiler flags for cpp
CPPFLAGS = $(CFLAGS)

CC = gcc $(CFLAGS)
CXX = clang++ -std=c++11 -stdlib=libc++ $(CPPFLAGS)

C_SOURCES = MPMesh.c
CXX_SOURCES = main.cpp MPEnvironment2D.cpp MPEnvironment3D.cpp MPDijkstra3D.cpp MPTransform3D.cpp MPModel.cpp MPReader.cpp MPTokenizer.cpp

SRC_PATH = src
OBJ_PATH = obj

C_OBJ_FILES = $(patsubst %.c,$(OBJ_PATH)/%.o,$(C_SOURCES))
CXX_OBJ_FILES = $(patsubst %.cpp,$(OBJ_PATH)/%.o,$(CXX_SOURCES))

PROGS=MotionPlanner

.PHONY: all clean
all: $(PROGS)

$(PROGS): $(C_OBJ_FILES) $(CXX_OBJ_FILES)
	$(CXX) $^ -o $@

## straight c rules
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) -c $< -o $@

## c++ rules
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(CXX) -c $< -o $@

clean:	
	$(RM) $(OBJ_PATH)/*.o $(PROGS)


