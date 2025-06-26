CXX = clang++
CXXFLAGS = -std=c++20
OBJ = build/main.o build/ConfigReader.o build/ConfigData.o build/ConfigMap.o

build/main: build/ ${OBJ}
	@${CXX} ${CXXFLAGS} ${OBJ} -o $@
	@./$@

build/:
	@mkdir -p build

build/main.o: src/main.cpp
	@clang++ -c src/main.cpp -o build/main.o

build/ConfigReader.o: src/ConfigReader.cpp
	@clang++ -c src/ConfigReader.cpp -o build/ConfigReader.o

build/ConfigData.o: src/ConfigData.cpp
	@clang++ -c src/ConfigData.cpp -o build/ConfigData.o

build/ConfigMap.o: src/ConfigMap.cpp
	@clang++ -c src/ConfigMap.cpp -o build/ConfigMap.o

# Here, we have litte make scripts we can use
# 	.PHONY says, hey, this 'file' is always out of date, so always execute
# 	what we have written in here
.PHONY: clean test run
clean: 
	@for i in build/*; do echo "  removing file $$i"; rm $$i; done

test:
	@${CXX} ${CXXFLAGS} test/test.cpp -o build/test
	@./build/test

run: build/ build/main
	@./build/main

# Notes
# 	@ Before a command makes it silent, so you won't see what's ran
# 	$@ -> refers to item before : in rule, so above build/main
# 		would be a $@
# 	$< -> refers to first item after : in rule, so above src/main.cpp
# 		would be one
