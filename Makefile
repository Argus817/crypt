# 1. Variables
CXX         = g++
CXXFLAGS    = -Wall -Wextra -std=c++23 -O2
TARGET_EXEC = executable

# Directories
SRC_DIR    = src
OBJ_DIR    = obj
TARGET_DIR = target

# Files (Updated to find all nested .cpp files)
SRCS   = $(shell find $(SRC_DIR) -name "*.cpp")
OBJS   = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET = $(TARGET_DIR)/$(TARGET_EXEC)

ARGS ?= 

# 2. Phony Targets
.PHONY: all clean run

# 3. Default Target
all: $(TARGET)

# 4. Linking Rule
$(TARGET): $(OBJS) | $(TARGET_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 5. Compilation Rule (Compiles src/**/*.cpp into obj/**/*.o)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create target directory if it doesn't exist
$(TARGET_DIR):
	mkdir -p $(TARGET_DIR)

# 6. Utility Rules
run: $(TARGET)
	./$(TARGET) $(ARGS)

clean:
	rm -rf $(OBJ_DIR) $(TARGET_DIR)
