# 1. Variables
CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
TARGET   = target/executable

# Directories
SRC_DIR  = src
OBJ_DIR  = obj

# Files (Paths updated to include directories)
SRCS     = $(wildcard $(SRC_DIR)/*.cpp)
OBJS     = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# 2. Phony Targets
.PHONY: all clean run

# 3. Default Target
all: $(TARGET)

# 4. Linking Rule
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 5. Compilation Rule (Compiles src/*.cpp into obj/*.o)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# 6. Utility Rules
run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

