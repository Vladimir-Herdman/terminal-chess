#include <iostream>
#include <string>
#include "ConfigMap.h"

void background_color(std::string &value) { std::cout << "Test worked" << '\n'; }
void foreground_color(std::string& value) { std::cout << "Test worked" << '\n'; }

ConfigMap config_map = {
    {"test", background_color},
    {"test2", foreground_color},
};
