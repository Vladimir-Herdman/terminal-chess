CXX = clang++
CXXFLAGS = -std=c++20

build/main: src/main.cpp
	@${CXX} ${CXXFLAGS} $< -o $@
	@./$@

# Here, we have litte make scripts we can use
# 	.PHONY says, hey, this isn't a file so don't check, treat it like a
# 	command and just do what's in here
.PHONY: test
test:
	@${CXX} ${CXXFLAGS} test/test.cpp -o build/test
	@./build/test
	@./build/test

# Notes
# 	@ Before a command makes it silent, so you won't see what's ran
# 	$@ -> refers to item before : in rule, so above build/main
# 		would be a $@
# 	$< -> refers to first item after : in rule, so above src/main.cpp
# 		would be one
