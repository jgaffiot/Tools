// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot
 * DATE: 2015-03-17
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//

#ifndef EXCEPTION_HH
#define EXCEPTION_HH 1

#include <exception>
#include <fstream>
#include <sstream>
#include <string>

#include "String.hh"

namespace tools
{
class Error: public std::exception {
public:
    virtual ~Error() noexcept {}

    explicit Error(const std::string& arg): std::exception(), message(arg) {}
    explicit Error(const std::string& arg, std::ofstream& write):
        std::exception(), message(arg) {
        write << message << std::endl;
    }

    template<typename... Args>
    Error(const std::string& arg, Args... args): std::exception() {
        message = cat(arg, args...);
    }

    inline virtual const std::string& swhat() const noexcept { return message; }
    inline virtual const char* what() const noexcept { return message.c_str(); }

private:
    std::string message;
};

}  // namespace tools

#endif  // EXCEPTION_HH
