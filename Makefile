# Compiler and flags
CXX      := clang++
CXXFLAGS := -std=c++11 -Wall -Wextra -g

# Source and target
SRC      := vector.cpp
TARGET   := vector_test

# Default build rule
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Clean build artifacts
clean:
	rm -f $(TARGET)

.PHONY: all clean
objects = adnd.o
