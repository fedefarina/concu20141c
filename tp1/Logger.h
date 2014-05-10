#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <sstream>
#include "LockFile.h"

using namespace std;

class Logger {

private:
    static LockFile lf;
    static bool isDebugMode;

public:
    Logger();
    ~Logger();

    static void setDebugMode(bool isDebugMode);
    static void debug(pid_t pid, const string& msg);
};

#endif // LOGGER_H
