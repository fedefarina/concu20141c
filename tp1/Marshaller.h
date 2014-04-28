#ifndef MARSHALLER_H
#define MARSHALLER_H
#include "Auto.h"
#include <iostream>
#include <string>
#include <iostream>
#include <vector>
#include "Utils.h"

class Marshaller {

private:
    void split(std::vector<string> &tokens, std::string text, char sep) {
        int start = 0, end = 0;
        while ((end = text.find(sep, start)) != std::string::npos) {
            tokens.push_back(text.substr(start, end - start));
            text=text.substr(end+1, text.length());
        }
    }

public:
    Marshaller(){}
    ~Marshaller() {}
    std::string toString(Auto unAuto){
        std::string buffer;
        Utils<float> utils;
        buffer.append(utils.tostr(unAuto.getCapacidad()));
        buffer.append("_");
        return buffer;
    }

    Auto fromString(const std::string autoString){
        Auto unAuto;
        std::vector<string> tokens;
        split(tokens,autoString,'_');
        Utils<float> utils;
        float number=utils.fromString(tokens.at(0));
        unAuto.setCapacidad(number);
        return unAuto;
    }
};



#endif // MARSHALLER_H
