# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

CXX = g++-11 -std=c++20
CXXFLAGS = -Wall -O -MMD -I/usr/X11/include
LIBFLAGS = -L/opt/homebrew/lib -lX11
SOURCES = $(wildcard $(SRC_DIR)/*.cc)   # list of all .cc files in the current directory
OBJECTS = $(patsubst $(SRC_DIR)/%.cc,$(BUILD_DIR)/%.o,$(SOURCES))  # .o files depend upon .cc files with same names
DEPENDS = ${OBJECTS:.o=.d}   # .d file is list of dependencies for corresponding .cc file
-include ${DEPENDS}

EXEC=$(BIN_DIR)/chess

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc 
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(LIBFLAGS) -I$(SRC_DIR)

# First target in the makefile is the default target.
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC)  $(LIBFLAGS)

.PHONY: clean
clean:
	rm  -f $(OBJECTS) $(DEPENDS) $(EXEC)

# Ensure that the build and bin directories exist
$(shell mkdir -p $(BUILD_DIR) $(BIN_DIR))

# Default target
all: $(EXECUTABLE)

