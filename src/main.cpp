#include <iostream>

#include "cpu/cpu.h"

int main() {
    CPU cpu = CPU();
    while (true) {
        cpu.step();
    }
    return 0;
}