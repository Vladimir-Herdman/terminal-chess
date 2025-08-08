// CHAR CASTING
#include <iostream>
#include <thread>
int char_casting () {
    using namespace std;
    char let = 'a';   
    cout << let << endl;
    cout << (char)(let+1) << endl;
    cout << static_cast<char>(let-1) << endl;
    return 0;
}

// KEY PRESSES
#include "key_presses/getch.hpp"
#include <chrono>
#include <thread>
#include <unistd.h>
int key_presses() {
    using namespace std;
    char key;
    while (true) {
        key = getch();
        std::cout << "you pressed:" << key << '\n'; //<CR> 10 on mac = TODO: test windows and linux
        this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    return 0;
}

void left_bracket(){
    std::cout << "left bracket\n";
}
// FUNCTION LOOKUP TABLES
int lookups() {
    //struct style lookup can use metadata with command
    struct command_info {
        void (*func)();
    };
    const struct command_info command_info_lookup[] = {
        ['{'] = {left_bracket}
    };
    std::cout << sizeof(command_info_lookup) << '\n';
    command_info_lookup['{'].func();

    // array style is just barebones function calling like our config map
    void (*command_info_array_lookup[])() = {
        ['{'] = left_bracket
    };
    command_info_array_lookup['{']();
    return 0;
}

int main() {
    std::cout << (512 | 262144);
}
