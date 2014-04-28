#include "Logger.h"

LockFile Logger::lf((char*)"logger");

Logger::Logger() {

}

Logger::~Logger() {

}

void Logger::debug(pid_t pid, const string& msg) {

    cout<<msg<<endl;
    stringstream ss;
    ss << pid;
    string str = ss.str();
    str += ": ";

    Logger::lf.tomarLock();

    Logger::lf.escribir((char*) str.c_str(), str.size());
    Logger::lf.escribir((char*) msg.c_str(), msg.size());

    Logger::lf.liberarLock();
}
