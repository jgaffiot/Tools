// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot and M.Fechner
 * DATE: from 2009 (C++98 version) to 2015 (C++11 version)
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//

#ifndef PROGRESS_BAR_HH
#define PROGRESS_BAR_HH

#include <chrono>
#include <ratio>

namespace tools
{
////////////////////////////////////////////////////////////////

// C++ 98 version, with functions, time has to be managed by system dependant libraries
// stolen from wget code
/* Use move_to_end (p,s) to get p to point the end of the string (the
   terminating \0).  This is faster than s+=strlen(s), but some people
   are confused when they see strchr (s, '\0') in the code.  */
#define move_to_end(ptr, str) ptr = strchr(str, '\0');

namespace detail_bar
{
constexpr size_t kLargeSize = 80;
constexpr size_t kSmallSize = 40;
}  // namespace detail_bar

void display_bar_cxx98_notime(
    char* disp,
    size_t n,
    const uint64_t whereami,
    const uint64_t tot,
    const bool kbs = true);
void display_bar_cxx98(
    char* disp,
    size_t n,
    const uint64_t whereami,
    const uint64_t tot,
    const double init_time,
    const bool kbs = true);
void display_end_bar(char* disp, size_t n);

////////////////////////////////////////////////////////////////

// C++ 11 version, a class using std::chrono
class ProgressBar {
public:
    explicit ProgressBar(double _NbIteration);

    void Display(double current_iter);
    void DisplayEnd();

private:
    typedef std::chrono::high_resolution_clock PBC;  // Progress Bar Clock
    typedef PBC::time_point PBTP;  // Progress Bar Time Point

    double NbIteration;
    double previous_iter = 0.;
    char small_buffer[detail_bar::kSmallSize];
    char large_buffer[detail_bar::kLargeSize];
    PBTP init, now, previous;

    constexpr static double ClockPeriod =
        static_cast<double>(PBC::period::den) / static_cast<double>(PBC::period::num);
    constexpr static std::chrono::duration<PBC::rep, std::chrono::milliseconds::period>
        delta_t{500};
};

}  // namespace tools

#endif  // end PROGRESS_BAR_HH
