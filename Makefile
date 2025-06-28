CXX = clang++
CXXFLAGS = -std=c++20

# Extremely cool .cpp to .o setup and recipes here for easier src additions
# Great resource over Makefiles: https://makefiletutorial.com/#getting-started
SRC := ${shell find src/ -name '*.cpp'}
OBJ := ${patsubst src/%.cpp,build/%.o,$(SRC)}

all: ${OBJ}
	@mkdir -p ./bin
	@${CXX} ${CXXFLAGS} ${OBJ} -o ./bin/main

build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	@${CXX} ${CXXFLAGS} -c $< -o $@

# Here, we have litte make scripts we can use
# 	.PHONY says, hey, this 'file' is always out of date, so always execute
# 	what we have written in here
.PHONY: all clean test run
clean: 
	@echo '    cleaning build/ and bin/ directories...'
	@rm -r build/ bin/

test:
	@${CXX} ${CXXFLAGS} test/test.cpp -o bin/test
	@./bin/test

run: all
	@./bin/main

# Notes
# 	@ Before a command makes it silent, so you won't see what's ran
# 	$@ -> refers to item before : in rule, so above build/main
# 		would be a $@
# 	$< -> refers to first item after : in rule, so above src/main.cpp
# 		would be one
