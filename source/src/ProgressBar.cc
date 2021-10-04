// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot and M.Fechner
 * DATE: from 2009 (C++98 version) to 2015 (C++11 version)
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//

#include "ProgressBar.hh"

#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace tools
{
using std::cout;
using std::snprintf;

void display_bar_cxx98_notime(
    char* disp, size_t n, const uint64_t whereami, const uint64_t tot, const bool kbs) {
    int progress = static_cast<int>(
        100. * static_cast<double>(whereami) / static_cast<double>(tot));
    char* p = disp;
    struct timeval tv;
    static double previous_time = 0., previous = 0.;
    snprintf(disp, n, "processed: %2d%%  ", progress);
    move_to_end(p, disp);
    *p++ = '[';
    for (int k = 0; k < 20; k++) {
        if (k < progress / 5) {
            *p++ = '=';
        } else if (k == progress / 5) {
            *p++ = '>';
        } else {
            *p++ = ' ';
        }
    }
    *p++ = ']';
    *p++ = '\0';
    gettimeofday(&tv, 0);
    char buffer[detail_bar::kSmallSize];
    double speed =
        (static_cast<double>(whereami) - previous * static_cast<double>(tot))
        / static_cast<double>(tv.tv_sec + 1.0e-6 * tv.tv_usec - previous_time) / 1024.;
    if (kbs) {
        snprintf(buffer, detail_bar::kSmallSize, "  %.1f kB/s ", speed);
    } else {
        snprintf(buffer, detail_bar::kSmallSize, "  %.1f evts/s ", speed * 1024.);
    }
    strncat(disp, buffer, detail_bar::kSmallSize);
    cout << "\r";
    cout << disp;
    cout.flush();
    previous = static_cast<double>(whereami) / static_cast<double>(tot);
    previous_time = static_cast<double>(tv.tv_sec + 1.0e-6 * tv.tv_usec);
}

////////////////////////////////////////////////////////////////

void display_bar_cxx98(
    char* disp,
    size_t n,
    const uint64_t whereami,
    const uint64_t tot,
    const double init_time,
    const bool kbs) {
    int progress = static_cast<int>(
        100. * static_cast<double>(whereami) / static_cast<double>(tot));
    char* p = disp;
    struct timeval tv;
    static double previous_time = 0., previous = 0.;
    snprintf(disp, n, "processed: %2d%%  ", progress);
    move_to_end(p, disp);
    *p++ = '[';
    for (int k = 0; k < 20; k++) {
        if (k < progress / 5) {
            *p++ = '=';
        } else if (k == progress / 5) {
            *p++ = '>';
        } else {
            *p++ = ' ';
        }
    }
    *p++ = ']';
    *p++ = '\0';
    gettimeofday(&tv, 0);
    char buffer[detail_bar::kSmallSize];
    double speed =
        (static_cast<double>(whereami) - previous * static_cast<double>(tot))
        / static_cast<double>(tv.tv_sec + 1.0e-6 * tv.tv_usec - previous_time) / 1024.;
    double average = static_cast<double>(whereami)
                     / static_cast<double>(tv.tv_sec + 1.0e-6 * tv.tv_usec - init_time)
                     / 1024.;
    if (kbs) {
        snprintf(
            buffer, detail_bar::kSmallSize, "  %.1f kB/s -> %.1f kB/s", speed, average);
    } else {
        snprintf(
            buffer,
            detail_bar::kSmallSize,
            "  %.1f evts/s -> %.1f evts/s",
            speed * 1024.,
            average * 1024.);
    }
    strncat(disp, buffer, detail_bar::kSmallSize);
    cout << "\r";
    cout << disp;
    cout.flush();
    previous = static_cast<double>(whereami) / static_cast<double>(tot);
    previous_time = static_cast<double>(tv.tv_sec + 1.0e-6 * tv.tv_usec);
}

////////////////////////////////////////////////////////////////

void display_end_bar(char* disp, size_t n) {
    snprintf(disp, n, "processed: 100%% [====================]");
    cout << "\r";
    cout << disp;
    cout.flush();
    cout << "\n";
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

ProgressBar::ProgressBar(double _NbIteration):
    NbIteration(_NbIteration), small_buffer(), large_buffer() {
    init = Clock::now();
    previous = init;
}

////////////////////////////////////////////////////////////////

void ProgressBar::Display(double current_iter) {
    now = Clock::now();
    if (now - previous < delta_t) {
        return;
    }

    char* p = large_buffer;
    int progress = static_cast<int>(100. * current_iter / NbIteration);
    snprintf(large_buffer, detail_bar::kLargeSize, "processed: %2d%%  ", progress);
    move_to_end(p, large_buffer);

    *p++ = '[';
    for (int k = 0; k < 20; k++) {
        if (k < progress / 5) {
            *p++ = '=';
        } else if (k == progress / 5) {
            *p++ = '>';
        } else {
            *p++ = ' ';
        }
    }
    *p++ = ']';
    *p++ = '\0';

    double speed = (current_iter - previous_iter)
                   / static_cast<double>((now - previous).count()) * ClockPeriod;
    double average =
        current_iter / static_cast<double>((now - init).count()) * ClockPeriod;

    snprintf(
        small_buffer,
        detail_bar::kSmallSize,
        "  %.1f evts/s -> %.1f evts/s",
        speed,
        average);
    strncat(large_buffer, small_buffer, detail_bar::kSmallSize);

    cout << "\r" << large_buffer;
    cout.flush();

    previous_iter = current_iter;
    previous = now;
}

////////////////////////////////////////////////////////////////

void ProgressBar::DisplayEnd() {
    cout << "\rprocessed: 100%% [====================]\n";
}

}  // namespace tools
