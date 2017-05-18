// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot
 * DATE: 2015-03-17
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//

#ifndef EXCEPTION_HH
#define EXCEPTION_HH 1

#include <String.hh>

#include <exception>
#include <string>
#include <fstream>
#include <sstream>

namespace tools {

class except : public std::exception {
public:
  virtual ~except() noexcept {}

  explicit except(const std::string& arg): std::exception(), message(arg) {}
  explicit except(const std::string& arg, std::ofstream& write): std::exception(), message(arg) {
    write << message << std::endl;
  }

  template <typename ... Args>
  except(Args ... args): std::exception() { message = scat(args...); }

  inline virtual const std::string& swhat() const noexcept { return message; }
  inline virtual const char* what() const noexcept { return message.c_str(); }

private:
  std::string message;
};

} // namespace tools

#endif // EXCEPTION_HH
