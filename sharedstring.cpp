#include "sharedstring.h"
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <utility>

std::map<const char*,SharedString::StringValue*,SharedString::CStringComp> SharedString::stringval_map;

SharedString::StringValue* SharedString::stringval_of(const char* c_str) {
    StringValue* ret = nullptr;
    std::map<const char*,StringValue*>::iterator iter;
    iter = stringval_map.find(c_str);

    if (iter != stringval_map.end()) {
        ret = iter->second;
    }
    else {
        ret = new StringValue(c_str);
        stringval_map[ret->_c_array] = ret;
    }
    ret->_count++;
    return ret;
}


SharedString::SharedString(const char* c_str) : _value{stringval_of(c_str)} {}

SharedString::SharedString(const SharedString& other)
                  : _value{other._value}
{
    _value->_count++;
}

SharedString::StringValue::StringValue(const char* c_str)
                        : _count{0}
                        , _array_size{(unsigned)strlen(c_str)}
{
    _c_array = new char[_array_size+1];
    strcpy(_c_array,c_str);
}

SharedString::~SharedString() {
    if ( --(_value->_count) == 0) {
        std::map<const char*,StringValue*>::iterator iter;
        iter = stringval_map.find(_value->_c_array);
        stringval_map.erase(iter);
        delete _value;
    }
}

SharedString::StringValue::~StringValue(){
    delete[] _c_array;
}

SharedString& SharedString::operator=(const SharedString& other) {
    if (this != &other) {
        if (--(_value->_count) == 0) {
            std::map<const char*,StringValue*>::iterator iter;
            iter = stringval_map.find(_value->_c_array);
            stringval_map.erase(iter);
            delete _value;
        }
        _value = other._value;
        _value->_count++;
    }
    return *this;
}

SharedString& SharedString::operator=(const char* c_str) {
    return *this = SharedString(c_str);
}

char SharedString::operator[](unsigned int index) const {
    return _value->_c_array[index];
}

SharedString::StringProxy SharedString::operator[](unsigned int index) {
    return StringProxy(this,index);
}

SharedString SharedString::operator+(const SharedString& other) const {
    unsigned int newsize = this->get_size() + other.get_size() + 1;
    char* temp = new char[newsize];
    strcpy(temp,this->c_str());
    strcat(temp,other.c_str());
    SharedString ret(temp);
    delete[] temp;
    return ret;
}

SharedString SharedString::operator+(const char* c_str) const {
    unsigned int newsize = this->get_size() + strlen(c_str) + 1;
    char* temp = new char[newsize];
    strcpy(temp,this->c_str());
    strcat(temp,c_str);
    SharedString ret(temp);
    delete[] temp;
    return ret;
}

SharedString SharedString::operator+(char ch) const {
    unsigned int newsize = this->get_size() + 1;
    char* temp = new char[newsize+1];
    strcpy(temp,this->c_str());
    temp[newsize-1] = ch;
    temp[newsize] = '\0';
    SharedString ret(temp);
    delete[] temp;
    return ret;
}

SharedString& SharedString::operator+=(const SharedString& other) {
    return *this = *this + other;
}

SharedString& SharedString::operator+=(const char* c_str) {
    return *this = *this + c_str;
}

SharedString& SharedString::operator+=(char ch) {
    return *this = *this + ch;
}

std::ostream& operator<<(std::ostream& os, const SharedString& str) {
    os << str.c_str();
    return os;
}

std::istream& operator>>(std::istream& is, SharedString& str) {
    char input[100];
    is.getline(input, 100, '\n');
    str = SharedString(input);
    return is;
}
