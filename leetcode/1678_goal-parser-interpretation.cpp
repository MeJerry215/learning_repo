#include "common.h"


class Solution {
public:
    string interpret(string command) {
        int i = 0, j = 0;
        while(i < command.size()) {
            if (command[i] == 'G') {
                command[j++] = command[i++];
            } else {
                if (command[i + 1] == ')') {
                    command[j++] = 'o';
                    i += 2;
                } else {
                    command[j] = 'a';
                    command[j + 1] = 'l';
                    j += 2;
                    i += 4;
                }
            }
        }
        command.resize(i);
        return command
    }
};