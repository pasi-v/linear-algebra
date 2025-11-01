# Compiler and flags
CXX      := clang++
CPPFLAGS := -Iinclude -Ithird_party -MMD -MP
CXXFLAGS := -std=c++11 -Wall -Wextra -Wtype-limits -Wpedantic -O0 -g -fno-omit-frame-pointer
# LDFLAGS  :=    # (add libs here if needed)

BINDIR   := bin
SRCDIR   := src
TESTDIR  := tests

SRC  := $(wildcard $(SRCDIR)/*.cpp)
OBJ  := $(SRC:.cpp=.o)
DEPS := $(OBJ:.o=.d)

TEST_SRC := $(wildcard $(TESTDIR)/*.cpp)
TEST_OBJ := $(TEST_SRC:.cpp=.o)
TEST_DEPS := $(TEST_OBJ:.o=.d)

TARGET_TEST := $(BINDIR)/tests

# Default build rule
all: $(TARGET_TEST)

# Ensure bin/ exists but doesn't retrigger links when timestamp changes
$(BINDIR):
	@mkdir -p $@

$(TARGET_TEST): $(OBJ) $(TEST_OBJ) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

# Run tests
test: $(TARGET_TEST)
	./$(TARGET_TEST)

# Reformat source files
format:
	clang-format -i $(SRC) $(TEST_SRC) \
		$(wildcard include/la/*.hpp)

# Clean build artifacts
clean:
	rm -rf $(BINDIR) $(OBJ) $(TEST_OBJ) $(DEPS) $(TEST_DEPS)

# Auto-include dependency files (ok if they don't exist yet)
-include $(DEPS) $(TEST_DEPS)

.PHONY: all clean format test
