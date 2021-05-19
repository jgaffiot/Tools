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

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace tools;
using namespace std;

void display_bar_cxx98_notime(
    char* disp, const unsigned long whereami, const unsigned long tot, const bool kbs) {
    int progress = int(100. * double(whereami) / double(tot));
    char* p = disp;
    struct timeval tv;
    static double previous_time = 0., previous = 0.;
    std::sprintf(disp, "processed: %2d%%  ", progress);
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
    char buffer[40];
    double speed = (double(whereami) - previous * double(tot))
                   / double(tv.tv_sec + 1.0e-6 * tv.tv_usec - previous_time) / 1024.;
    if (kbs) {
        std::sprintf(buffer, "  %.1f kB/s ", speed);
    } else {
        std::sprintf(buffer, "  %.1f evts/s ", speed * 1024.);
    }
    strncat(disp, buffer, 40);
    std::cout << "\r";
    std::cout << disp;
    std::cout.flush();
    previous = double(whereami) / double(tot);
    previous_time = double(tv.tv_sec + 1.0e-6 * tv.tv_usec);
}

////////////////////////////////////////////////////////////////

void display_bar_cxx98(
    char* disp,
    const unsigned long whereami,
    const unsigned long tot,
    const double init_time,
    const bool kbs) {
    int progress = int(100. * double(whereami) / double(tot));
    char* p = disp;
    struct timeval tv;
    static double previous_time = 0., previous = 0.;
    std::sprintf(disp, "processed: %2d%%  ", progress);
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
    char buffer[40];
    double speed = (double(whereami) - previous * double(tot))
                   / double(tv.tv_sec + 1.0e-6 * tv.tv_usec - previous_time) / 1024.;
    double average =
        double(whereami) / double(tv.tv_sec + 1.0e-6 * tv.tv_usec - init_time) / 1024.;
    if (kbs) {
        std::sprintf(buffer, "  %.1f kB/s -> %.1f kB/s", speed, average);
    } else {
        std::sprintf(
            buffer, "  %.1f evts/s -> %.1f evts/s", speed * 1024., average * 1024.);
    }
    strncat(disp, buffer, 40);
    std::cout << "\r";
    std::cout << disp;
    std::cout.flush();
    previous = double(whereami) / double(tot);
    previous_time = double(tv.tv_sec + 1.0e-6 * tv.tv_usec);
}

////////////////////////////////////////////////////////////////

void display_end_bar(char* disp) {
    sprintf(disp, "processed: 100%% [====================]");
    std::cout << "\r";
    std::cout << disp;
    std::cout.flush();
    std::cout << "\n";
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

ProgressBar::ProgressBar(double _NbIteration): NbIteration(_NbIteration) {
    init = PBC::now();
    previous = init;
}

////////////////////////////////////////////////////////////////

void ProgressBar::Display(double current_iter) {
    now = PBC::now();
    if (now - previous < delta_t) {
        return;
    }

    char* p = large_buffer;
    int progress = int(100. * current_iter / NbIteration);
    sprintf(large_buffer, "processed: %2d%%  ", progress);
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

    double speed =
        (current_iter - previous_iter) / double((now - previous).count()) * ClockPeriod;
    double average = current_iter / double((now - init).count()) * ClockPeriod;

    sprintf(small_buffer, "  %.1f evts/s -> %.1f evts/s", speed, average);
    strncat(large_buffer, small_buffer, 40);

    cout << "\r" << large_buffer;
    cout.flush();

    previous_iter = current_iter;
    previous = now;
}

////////////////////////////////////////////////////////////////

void ProgressBar::DisplayEnd() {
    cout << "\rprocessed: 100%% [====================]\n";
}
