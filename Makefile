# Compiler and flags
CXX      := clang++
CXXFLAGS := -std=c++11 -Wall -Wextra -g

# Source and target
SRC      := test_main.cpp vector.cpp vector2d.cpp plane3d.cpp test_vector3d.cpp test_math_utils.cpp
HEADERS  := math_utils.h vector3d.h
TARGET   := tests

# Default build rule
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Run tests
test: $(TARGET)
	./$(TARGET)

# Reformat source files
format:
	clang-format -i $(SRC) $(HEADERS)

# Clean build artifacts
clean:
	rm -f $(TARGET)

.PHONY: all clean format test
