// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot
 * DATE: 2015-03-17
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//

#include <DualStream.hh>
using namespace tools;

#include <String.hh>

#include <string>
#include <stdexcept>
using namespace std;

DualStream::DualStream(const char* output_file_name, std::ostream& os): screen_out(os)
{
  if ( not Open(output_file_name) ) {
    throw invalid_argument(cat("DualStream::DualStream: Can not open file: ",output_file_name) );
  }
}

DualStream::DualStream(const string& output_file_name, std::ostream& os): screen_out(os)
{
  if ( not Open(output_file_name.data()) ) {
    throw invalid_argument(cat("DualStream::DualStream: Can not open file: ",output_file_name) );
  }
}

bool DualStream::Open(const char* output_file_name)
{
  if (file_stream.is_open()) {
    cerr << "*Warning DualStream::DualStream: closing current output stream\n";
    file_stream.close();
  }
  file_stream.open(output_file_name, ios::out);
  if ( file_stream.is_open() and file_stream.good() ) { return true; }
  return false;
}

#ifdef DUAL_TEST
int main()
{
  ofstream ofs("dual.txt");
  DualStreamBuf bout(ofs.rdbuf());
  DualStreamBuf berr(ofs.rdbuf(),std::cerr);
  bout << "pwatout\n";
  berr << "pwaterr\n";
}
#endif
