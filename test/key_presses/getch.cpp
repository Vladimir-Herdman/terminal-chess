#include "key_presses/getch.hpp"

#if defined(__unix__) || defined(__APPLE__)
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#else
#include <conio.h>
#endif

char getch() {
#if defined(__unix__) || defined(__APPLE__)
    char buf = 0;
    struct termios old = {};
    if (tcgetattr(STDIN_FILENO, &old) < 0) perror("tcsetattr()");

    struct termios new_term = old;
    new_term.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_term) < 0) perror("tcsetattr ICANON");

    if (read(STDIN_FILENO, &buf, 1) < 0) perror("read()");

    tcsetattr(STDIN_FILENO, TCSANOW, &old); // restore old settings
    return buf;
#else
    return _getch();
#endif
}
