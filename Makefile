# Compiler and flags
CXX      := clang++
CXXFLAGS := -Iinclude -std=c++11 -Wall -Wextra -Wtype-limits -O0 -g -fno-omit-frame-pointer -MMD -MP

# Source and target
SRC      := test_main.cpp vector.cpp vector2d.cpp plane3d.cpp \
test_vector3d.cpp test_math_utils.cpp matrix.cpp parity.cpp
OBJ := $(SRC:.cpp=.o)
DEPS := $(OBJ:.o=.d)
TARGET   := tests

# Default build rule
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

# Run tests
test: $(TARGET)
	./$(TARGET)

# Reformat source files
format:
	clang-format -i $(SRC) $(shell find include -name '*.hpp')

# Clean build artifacts
clean:
	rm -f $(TARGET) *.o

.PHONY: all clean format test
