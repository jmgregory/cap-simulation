#ifndef JMG_EXCEPTION_H
#define JMG_EXCEPTION_H

#include <exception>
#include <string>

using std::string;

class Exception : public std::exception
{
public:
  Exception(const string & what) : _what(what) {}
  Exception(const char * what) : _what(what) {}

  const char* what() const throw()
  {
    return _what.c_str();
  }

  ~Exception() throw() {}

private:
  string _what;
};

#endif
