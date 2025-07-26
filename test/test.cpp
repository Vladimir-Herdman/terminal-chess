// CHAR CASTING
#include <iostream>
int char_casting () {
    using namespace std;
    char let = 'a';   
    cout << let << endl;
    cout << (char)(let+1) << endl;
    cout << static_cast<char>(let-1) << endl;
    return 0;
}

// KEY PRESSES
#include <stdio.h>
#include "key_presses/getch.hpp"
#include <unistd.h>
int main() {
    char key;
    while (true) {
        key = getch();
        printf("you pressed:%c\n", key); //<CR> 10 on mac = TODO: test windows and linux
        usleep(1000000);
    }
    return 0;
}
