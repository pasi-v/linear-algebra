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

# Clean build artifacts
clean:
	rm -f $(TARGET)

.PHONY: all clean

