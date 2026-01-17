# Compiler and flags
CXX      := clang++
CPPFLAGS := -Iinclude -Ithird_party -MMD -MP
CXXFLAGS := -std=c++11 -Wall -Wextra -Wtype-limits -Wpedantic -O0 -g -fno-omit-frame-pointer
# LDFLAGS  :=    # (add libs here if needed)

BINDIR   := bin
SRCDIR   := src
TESTDIR  := tests
APPDIR   := app

LIB_SRCS  := $(wildcard $(SRCDIR)/*.cpp)
LIB_OBJS  := $(LIB_SRCS:.cpp=.o)
LIB_DEPS := $(LIB_OBJS:.o=.d)

TEST_SRCS := $(wildcard $(TESTDIR)/*.cpp)
TEST_OBJS := $(TEST_SRCS:.cpp=.o)
TEST_DEPS := $(TEST_OBJS:.o=.d)

APP_SRCS  := $(wildcard $(APPDIR)/*.cpp)
APP_OBJS  := $(APP_SRCS:.cpp=.o)
APP_DEPS := $(APP_OBJS:.o=.d)

TARGET_TEST := $(BINDIR)/tests
TARGET_APP  := $(BINDIR)/la_calc

# Default build rule
all: $(TARGET_TEST)

# Ensure bin/ exists but doesn't retrigger links when timestamp changes
$(BINDIR):
	@mkdir -p $@

$(TARGET_TEST): $(LIB_OBJS) $(TEST_OBJS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(TARGET_APP): $(LIB_OBJS) $(APP_OBJS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

# Run tests
test: $(TARGET_TEST)
	./$(TARGET_TEST)

# Reformat source files
format:
	clang-format -i $(LIB_SRCS) $(TEST_SRCS) \
		$(wildcard include/la/*.hpp)

# Clean build artifacts
clean:
	rm -rf $(BINDIR) $(LIB_OBJS) $(TEST_OBJS) $(APP_OBJS) \
			$(LIB_DEPS) $(TEST_DEPS) $(APP_DEPS)

# Auto-include dependency files (ok if they don't exist yet)
-include $(LIB_DEPS) $(TEST_DEPS) $(APP_DEPS)

.PHONY: all clean format test
