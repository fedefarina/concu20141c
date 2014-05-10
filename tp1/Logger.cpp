#include "Logger.h"

/**
 * @brief Logger::lf
 *  Encargada de escribir los eventos de la aplicacion
 */

LockFile Logger::lf((char*)"logger");
bool Logger::isDebugMode;

Logger::Logger() {
}

Logger::~Logger() {
}

void Logger::setDebugMode(bool debugMode){
    Logger::isDebugMode=debugMode;
}

void Logger::debug(pid_t pid, const string& msg) {

    if(!Logger::isDebugMode)
        return;

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
