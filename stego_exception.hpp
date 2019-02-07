#ifndef STEGO_EXCEPTION_H
#define STEGO_EXCEPTION_H

#include <exception>

class Stego_exception: public std::exception
{
    char* msg;
public:
    Stego_exception();
    Stego_exception(char* msg)
    {
        this->msg = msg;
    }
    virtual const char* what() const throw()
    {
        return msg;
    }
};

#endif // STEGO_EXCEPTION_H
