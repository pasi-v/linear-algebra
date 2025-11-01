# Compiler and flags
CXX      := clang++
CXXFLAGS := -Iinclude -std=c++11 -Wall -Wextra -Wtype-limits -O0 -g -fno-omit-frame-pointer -MMD -MP

BINDIR   := bin
SRCDIR   := src
SRC      := $(wildcard $(SRCDIR)/*.cpp)
OBJ := $(SRC:.cpp=.o)
DEPS := $(OBJ:.o=.d)
TESTDIR  := tests
TEST_SRC := $(wildcard $(TESTDIR)/*.cpp)
TEST_OBJ := $(TEST_SRC:.cpp=.o)
TEST_DEPS := $(TEST_OBJ:.o=.d)
TARGET_TEST := $(BINDIR)/tests

# Default build rule
all: $(TARGET_TEST)

$(TARGET_TEST): $(OBJ) $(TEST_OBJ)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(TEST_OBJ)

# Run tests
test: $(TARGET_TEST)
	./$(TARGET_TEST)

# Reformat source files
format:
	clang-format -i $(SRC) $(TEST_SRC) $(shell find include -name '*.hpp')

# Clean build artifacts
clean:
	rm -rf $(BINDIR) $(OBJ) $(TEST_OBJ) $(DEPS) $(TEST_DEPS)

.PHONY: all clean format test
