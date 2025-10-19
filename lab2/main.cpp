#include <iostream>
#include "TerminalEmulator.h"
#include "Command.h"
int main() {
    TerminalEmulator term;
    while (true) {
        term.Repeat();
    }
    return 0;
}