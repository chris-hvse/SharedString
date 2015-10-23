#ifndef SHAREDSTRING_H
#define SHAREDSTRING_H
#include <iostream>
#include <map>
#include <cstring>

class SharedString {
private:
    class StringValue {
    public:
        char* _c_array;
        unsigned int _count;
        unsigned int _array_size;

        StringValue(const char*);
        ~StringValue();
    };

    class StringProxy {
    public:
        SharedString* _this_string;
        unsigned int _index;

        StringProxy(SharedString* this_string, unsigned int index)
                : _this_string{this_string}
                , _index{index}
                {}

        StringProxy(const StringProxy& other)
                : _this_string{other._this_string}
                , _index{other._index}
                {}

        char operator=(char c) {
            unsigned int size = _this_string->get_size() + 1;
            char* temp = new char[size];
            strcpy(temp,_this_string->_value->_c_array);
            temp[_index] = c;
            *_this_string = SharedString(temp);
            delete[] temp;
            return c;
        }

        char operator=(const StringProxy& other) {
            char indexed_char = other._this_string->_value->_c_array[other._index];
            *this = indexed_char;
            return indexed_char;
        }

        operator char() {
             return _this_string->_value->_c_array[_index];
        }
    };

    struct CStringComp {
        bool operator()(const char* s1, const char* s2) const {return std::strcmp(s1,s2) < 0;}
    };
    static std::map<const char*,StringValue*,CStringComp> stringval_map;

    static StringValue* stringval_of(const char*);

    StringValue* _value;
public:
    SharedString(const char* c_str = "");
    SharedString(const SharedString&);
    ~SharedString();
    const char* c_str() const { return _value->_c_array; }
    unsigned int get_size() const { return _value->_array_size; };
    unsigned int get_count() const { return _value->_count; }
    SharedString& operator=(const SharedString&);
    SharedString& operator=(const char*);
    StringProxy operator[](unsigned int);
    char operator[](unsigned int) const;
    SharedString operator+(const SharedString&) const;
    SharedString operator+(const char*) const;
    SharedString operator+(char) const;
    SharedString& operator+=(const SharedString&);
    SharedString& operator+=(const char*);
    SharedString& operator+=(char);
};

std::ostream& operator<<(std::ostream&, const SharedString&);
std::istream& operator>>(std::istream&, SharedString&);
#endif
