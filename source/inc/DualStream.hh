// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot
 * DATE: 2015-03-17
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//

#ifndef DUALSTREAM_HH
#define DUALSTREAM_HH 1

#include <iostream>
#include <fstream>

namespace tools {

class DualStream
{
public:
  DualStream(std::ostream& os = std::cout): screen_out(os) {}
  DualStream(const char* output_file_name, std::ostream& os = std::cout);
  DualStream(const std::string& output_file_name, std::ostream& os = std::cout);
  ~DualStream() { file_stream.close(); }

  bool Open(const char* output_file_name);
  inline bool Open(const std::string& output_file_name) { return Open(output_file_name.data()); }
  inline void Close() { file_stream.close(); }

  std::ofstream& GetStream() { return file_stream; }
  std::ofstream& Str()       { return file_stream; }
  std::ofstream& write()     { return file_stream; }

  template<class T>
  friend DualStream& operator<< (DualStream& os, const T& val);
  DualStream& operator<< (std::ostream& (*pfun)(std::ostream&));

private:
  std::ofstream file_stream;
  std::ostream& screen_out;
};

template<class T>
inline DualStream& operator<< (DualStream& os, const T& val)
{
  os.file_stream << val;
  std::cout << val;
  return os;
}

inline DualStream& DualStream::operator<< (std::ostream& (*pfun)(std::ostream&))
{
  pfun(file_stream);
  pfun(std::cout);
  return *this;
}

////////////////////////////////////////////////////////////////

class DualStreamBuf
{
public:
  DualStreamBuf(std::filebuf* buf, std::ostream& os = std::cout): file_buf(buf), screen_out(os) {}
  ~DualStreamBuf() {}

  std::ostream& GetStream() { return file_buf; }
  std::ostream& Str() { return file_buf; }
  std::ostream& write() { return file_buf; }

  template<class T>
  friend DualStreamBuf& operator<< (DualStreamBuf& os, const T& val);
  DualStreamBuf& operator<< (std::ostream& (*pfun)(std::ostream&));

private:
  std::ostream file_buf;
  std::ostream& screen_out;
};

template<class T>
inline DualStreamBuf& operator<< (DualStreamBuf& os, const T& val)
{
  os.file_buf << val;
  os.screen_out << val;
  return os;
}

inline DualStreamBuf& DualStreamBuf::operator<< (std::ostream& (*pfun)(std::ostream&))
{
  pfun(file_buf);
  pfun(screen_out);
  return *this;
}

} // namespace tools

#endif
