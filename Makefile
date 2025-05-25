# Compiler and flags
CXX      := clang++
CXXFLAGS := -std=c++11 -Wall -Wextra -g

# Source and target
SRC      := test_main.cpp vector.cpp vector2d.cpp
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
	clang-format -i *.cpp *.hpp

# Clean build artifacts
clean:
	rm -f $(TARGET)

.PHONY: all clean format test
