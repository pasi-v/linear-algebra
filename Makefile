# Compiler and flags
CXX      := clang++
CPPFLAGS := -Iinclude -Ithird_party -Iapp -MMD -MP
CXXFLAGS := -std=c++11 -Wall -Wextra -Wtype-limits -Wpedantic -O0 -g -fno-omit-frame-pointer
# LDFLAGS  :=    # (add libs here if needed)

BINDIR   := bin
SRCDIR   := src
TESTDIR  := tests
APPDIR   := app
APP_TESTDIR := $(APPDIR)/app_checks

LIB_SRCS  := $(wildcard $(SRCDIR)/*.cpp)
LIB_OBJS  := $(LIB_SRCS:.cpp=.o)
LIB_DEPS := $(LIB_OBJS:.o=.d)

TEST_SRCS := $(wildcard $(TESTDIR)/*.cpp)
TEST_OBJS := $(TEST_SRCS:.cpp=.o)
TEST_DEPS := $(TEST_OBJS:.o=.d)

APP_SRCS  := $(wildcard $(APPDIR)/*.cpp)
APP_OBJS  := $(APP_SRCS:.cpp=.o)
APP_DEPS := $(APP_OBJS:.o=.d)
APP_LIB_OBJS := $(filter-out $(APPDIR)/la_calc.o,$(APP_OBJS))

APP_TEST_SRCS := $(wildcard $(APP_TESTDIR)/*.cpp)
APP_TEST_OBJS := $(APP_TEST_SRCS:.cpp=.o)
APP_TEST_DEPS := $(APP_TEST_OBJS:.o=.d)

TARGET_TEST := $(BINDIR)/tests
TARGET_APP  := $(BINDIR)/la_calc
TARGET_APP_TEST := $(BINDIR)/app_tests

# Default build rule
all: $(TARGET_TEST) $(TARGET_APP) $(TARGET_APP_TEST)

# Ensure bin/ exists but doesn't retrigger links when timestamp changes
$(BINDIR):
	@mkdir -p $@

$(TARGET_TEST): $(LIB_OBJS) $(TEST_OBJS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(TARGET_APP): $(LIB_OBJS) $(APP_OBJS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(TARGET_APP_TEST): $(LIB_OBJS) $(APP_LIB_OBJS) $(APP_TEST_OBJS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

# Run app tests
app_tests: $(TARGET_APP_TEST)
	./$(TARGET_APP_TEST)

# Run tests
test: $(TARGET_TEST)
	./$(TARGET_TEST)

# --- Coverage (LLVM source-based, via the Xcode toolchain) -----------------
# Instrumented objects use a distinct .cov.o suffix so a coverage build never
# clobbers the normal -O0 -g objects (and vice versa). Requires llvm-profdata
# and llvm-cov, reached through `xcrun` on macOS.
COV_FLAGS    := -fprofile-instr-generate -fcoverage-mapping
COV_CXXFLAGS := $(CXXFLAGS) $(COV_FLAGS)
COVDIR       := coverage
COV_IGNORE   := third_party|tests
LLVMCOV      := xcrun llvm-cov
LLVMPROFDATA := xcrun llvm-profdata

LIB_COV_OBJS  := $(LIB_SRCS:.cpp=.cov.o)
TEST_COV_OBJS := $(TEST_SRCS:.cpp=.cov.o)
COV_OBJS      := $(LIB_COV_OBJS) $(TEST_COV_OBJS)
COV_DEPS      := $(COV_OBJS:.o=.d)

TARGET_COV_TEST := $(BINDIR)/tests_cov

# Instrumented compile rule (parallels make's built-in %.o : %.cpp rule).
%.cov.o: %.cpp
	$(CXX) $(COV_CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(TARGET_COV_TEST): $(COV_OBJS) | $(BINDIR)
	$(CXX) $(COV_CXXFLAGS) $(LDFLAGS) -o $@ $^

# Build + run the instrumented tests, then print a per-file coverage report
# for the library (third_party/ and tests/ are excluded from the numbers).
coverage: $(TARGET_COV_TEST)
	@mkdir -p $(COVDIR)
	LLVM_PROFILE_FILE="$(COVDIR)/tests.profraw" ./$(TARGET_COV_TEST)
	$(LLVMPROFDATA) merge -sparse $(COVDIR)/tests.profraw -o $(COVDIR)/tests.profdata
	$(LLVMCOV) report ./$(TARGET_COV_TEST) \
		-instr-profile=$(COVDIR)/tests.profdata \
		-ignore-filename-regex='$(COV_IGNORE)'

# Same data, rendered as a browsable HTML report under coverage/html/.
coverage-html: coverage
	$(LLVMCOV) show ./$(TARGET_COV_TEST) \
		-instr-profile=$(COVDIR)/tests.profdata \
		-ignore-filename-regex='$(COV_IGNORE)' \
		-format=html -output-dir=$(COVDIR)/html
	@echo "Open $(COVDIR)/html/index.html"

# Reformat source files
format:
	clang-format -i $(LIB_SRCS) $(TEST_SRCS) \
		$(wildcard include/la/*.hpp)

# Clean build artifacts
clean:
	rm -rf $(BINDIR) $(LIB_OBJS) $(TEST_OBJS) $(APP_OBJS) \
			$(LIB_DEPS) $(TEST_DEPS) $(APP_DEPS) \
			$(APP_TEST_OBJS) $(APP_TEST_DEPS) \
			$(COV_OBJS) $(COV_DEPS) $(COVDIR)

# Auto-include dependency files (ok if they don't exist yet)
-include $(LIB_DEPS) $(TEST_DEPS) $(APP_DEPS) $(APP_TEST_DEPS) $(COV_DEPS)

.PHONY: all clean format test app_tests coverage coverage-html
