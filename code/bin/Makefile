MYHOME = $(WORK)/code

# Compiler
CXX = g++

# Compiler Flags
CXXFLAGS = -DNDEBUG -O3 -std=c++17 -Wall -pedantic -fopenmp

# Directories
SRC_DIR = $(MYHOME)/c
INCLUDE_DIR = $(MYHOME)/include
OBJ_DIR = $(MYHOME)/obj
BIN_DIR = $(MYHOME)/bin

# Source and Object Files
SOURCES = $(SRC_DIR)/*.cpp
# Source and Object Files
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/*.cpp))

# Executable Name
EXECUTABLE = $(BIN_DIR)/uniformsched

# Build Rule
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

# Clean Rule
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.PHONY: all clean

