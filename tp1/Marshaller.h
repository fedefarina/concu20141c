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
    void split(std::vector<string> &tokens, std::string text, std::string sep) {

        std::string::size_type end = 0;
        if((end=text.find(sep))!= std::string::npos){
            while ( end != std::string::npos) {
                tokens.push_back(text.substr(0, end));
                text=text.substr(end, text.length());
                end=text.find(sep);
            }
        }else{
            tokens.push_back(text);
        }
    }

public:
    Marshaller(){}
    ~Marshaller() {}

    std::string toString(Auto unAuto){
        std::string buffer;
        Utils<float> utils;
        buffer.append(utils.tostr(unAuto.getCapacidad()));
        return buffer;
    }

    Auto fromString(const std::string autoString){
        Auto unAuto;
        std::vector<string> tokens;
        split(tokens,autoString,"_");
        Utils<float> utils;
        float number=utils.fromString(tokens.at(0));
        unAuto.setCapacidad(number);
        return unAuto;
    }
};



#endif // MARSHALLER_H
