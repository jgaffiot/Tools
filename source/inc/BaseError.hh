// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot
 * DATE: 2015-03-17
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//

#ifndef TOOLS_BASE_ERROR_HH
#define TOOLS_BASE_ERROR_HH 1

#include <exception>
#include <fstream>
#include <string>

#include "StringUtils.hh"

namespace tools
{
class BaseError: public std::exception {
public:
    virtual ~BaseError() noexcept {}

    explicit BaseError(const std::string& arg): std::exception(), message(arg) {}
    explicit BaseError(const std::string& arg, std::ofstream& write):
        std::exception(), message(arg) {
        write << message << std::endl;
    }

    template<typename... Args>
    BaseError(const std::string& arg, Args... args): std::exception() {
        message = cat(arg, args...);
    }

    inline const std::string& swhat() const noexcept { return message; }
    inline const char* what() const noexcept override { return message.c_str(); }

private:
    std::string message;
};

}  // namespace tools

#endif  // TOOLS_BASE_ERROR_HH
