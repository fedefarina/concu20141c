#ifndef UTILS_H
#define UTILS_H
#include "string"

template <class T> class Utils {
public:
    Utils();
    ~Utils();
    std::string toString(const T& t);
    T fromString(const std::string& s);

};

template <class T> Utils<T> :: Utils (){}
template <class T> Utils<T> :: ~Utils (){}

template <class T> std::string Utils<T>::toString(const T& t){
    ostringstream os;
    os<<t;
    return os.str();
}

template <class T> T Utils<T> ::fromString(const std::string& s){
    std::istringstream stream (s);
    T t;
    stream >> t;
    return t;
}
#endif // UTILS_H
