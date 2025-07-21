CXX = clang++
CXXFLAGS = -std=c++20 -O2 -Isrc -Wall -Wextra -Wno-bitwise-op-parentheses
CXXMACRODEF = -DTERMINALCHESS_INCLUDE_CONFIGREADER -DDEV_HELPERS_LATER_REMOVE

SRC := ${shell find src/ -name '*.cpp'}
OBJ := ${patsubst src/%.cpp,build/%.o,$(SRC)}

all: ${OBJ}
	@mkdir -p ./bin
	@${CXX} ${CXXFLAGS} ${CXXMACRODEF} ${OBJ} -o ./bin/main

build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	@${CXX} ${CXXFLAGS} ${CXXMACRODEF} -c $< -o $@

.PHONY: all clean test run
clean: 
	@echo '    cleaning build/ and bin/ directories...'
	@rm -r build/ bin/ 2>/dev/null || true

test:
	@${CXX} ${CXXFLAGS} test/test.cpp -o bin/test
	@./bin/test

run: all
	@./bin/main
