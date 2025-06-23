CXX = clang++
CXXFLAGS = -std=c++20
OBJ = build/ConfigReader.o

build/main: src/main.cpp build/ ${OBJ}
	@${CXX} ${CXXFLAGS} $< ${OBJ} -o $@
	@./$@

build/:
	@mkdir -p build

build/ConfigReader.o: src/ConfigReader.cpp
	@clang++ -c src/ConfigReader.cpp -o build/ConfigReader.o

# Here, we have litte make scripts we can use
# 	.PHONY says, hey, this 'file' is always out of date, so always execute
# 	what we have written in here
.PHONY: clean test
clean: 
	@for i in build/*; do echo "  removing file $$i"; rm $$i; done

test:
	@${CXX} ${CXXFLAGS} test/test.cpp -o build/test
	@./build/test

# Notes
# 	@ Before a command makes it silent, so you won't see what's ran
# 	$@ -> refers to item before : in rule, so above build/main
# 		would be a $@
# 	$< -> refers to first item after : in rule, so above src/main.cpp
# 		would be one
