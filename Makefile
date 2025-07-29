CXX = clang++
CXXFLAGS = -std=c++20 -O2 -Isrc -Wall -Wextra -Wno-bitwise-op-parentheses
CXXMACRODEF = -DTERMINALCHESS_INCLUDE_CONFIGREADER -DDEV_HELPERS_LATER_REMOVE

SRC := $(shell find src/ -name '*.cpp')
OBJ := $(patsubst src/%.cpp,build/main/%.o,$(SRC))

TESTCXXFLAGS := -std=c++20 -O2 -Isrc -Itest -Wall -Wextra -Wno-bitwise-op-parentheses

TESTSRC := $(shell find test/ -name '*.cpp')
TESTOBJ := $(patsubst test/%.cpp,build/test/%.o,$(TESTSRC))

all: $(OBJ)
	@mkdir -p ./bin
	@$(CXX) $(CXXFLAGS) $(CXXMACRODEF) $(OBJ) -o ./bin/main

build/main/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(CXXMACRODEF) -c $< -o $@

test: $(TESTOBJ)
	@mkdir -p ./bin
	@$(CXX) $(TESTCXXFLAGS) $(TESTOBJ) -o bin/test
	@echo '    compiled into bin/test'

build/test/%.o: test/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(TESTCXXFLAGS) $(CXXMACRODEF) -c $< -o $@

.PHONY: all clean run runtest
clean: 
	@echo '    cleaning build/ and bin/ directories...'
	@rm -r build/ bin/ 2>/dev/null || true

run: all
	@./bin/main

runtest: test
	@./bin/test
