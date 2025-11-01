# Compiler and flags
CXX      := clang++
CXXFLAGS := -Iinclude -std=c++11 -Wall -Wextra -Wtype-limits -O0 -g -fno-omit-frame-pointer -MMD -MP

BINDIR   := bin
SRCDIR   := src
SRC      := $(wildcard $(SRCDIR)/*.cpp) test_main.cpp \
test_vector3d.cpp test_math_utils.cpp
OBJ := $(SRC:.cpp=.o)
DEPS := $(OBJ:.o=.d)
TARGET_TEST := $(BINDIR)/tests

# Default build rule
all: $(TARGET_TEST)

$(TARGET_TEST): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

# Run tests
test: $(TARGET_TEST)
	./$(TARGET_TEST)

# Reformat source files
format:
	clang-format -i $(SRC) $(shell find include -name '*.hpp')

# Clean build artifacts
clean:
	rm -rf $(BINDIR) $(SRCDIR)/*.o

.PHONY: all clean format test
