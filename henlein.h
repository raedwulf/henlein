/*
 * Copyright (c) 2016 Tai Chi Minh Ralph Eastwood
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef HENLEIN_INCLUDED_H
#define HENLEIN_INCLUDED_H

#if defined(__unix__) || defined(__unix) || defined(unix)
#define HENLEIN_UNIX
#endif

#if defined(__MACH__) && defined(__APPLE__)
#define HENLEIN_OSX
#endif

#if defined(__hpux) || defined(hpux)
#define HENLEIN_HPUX
#endif

#if defined(__sun__) || defined(__sun) || defined(sun)
#if defined(__SVR4) || defined(__svr4__)
#define HENLEIN_SOLARIS
#else
#define HENLEIN_SUNOS
#endif
#endif

#include <stdint.h>

#ifdef _MSC_VER
#define inline __inline
#endif

#ifdef _WIN32
#define HENLEIN_WIN32
#include <windows.h>
extern LARGE_INTEGER henlein_frequency;
#endif

#ifdef HENLEIN_UNIX
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#if defined(_POSIX_VERSION) && defined(_POSIX_TIMERS) && _POSIX_TIMERS > 0
#define HENLEIN_POSIX
#if defined(CLOCK_MONOTONIC_PRECISE)
#define HENLEIN_CLOCK CLOCK_MONOTONIC_PRECISE
#elif defined(CLOCK_HIGHRES)
#define HENLEIN_CLOCK CLOCK_HIGHRES
#elif defined(CLOCK_MONOTONIC)
#define HENLEIN_CLOCK CLOCK_MONOTONIC
#elif defined(CLOCK_REALTIME)
#define HENLEIN_CLOCK CLOCK_REALTIME
#else
#warning "no high resolution clockid_t present"
#endif
#if defined(CLOCK_MONOTONIC_FAST)
#define HENLEIN_CLOCK_MS CLOCK_MONOTONIC_FAST
#elif defined(CLOCK_MONOTONIC_COARSE)
#define HENLEIN_CLOCK_MS CLOCK_MONOTONIC_COARSE
#else
#define HENLEIN_CLOCK_MS HENLEIN_CLOCK
#endif
#endif
#endif

#ifdef HENLEIN_OSX
#include <mach/mach.h>
#include <mach/mach_time.h>
extern mach_timebase_info_data_t henlein_timebase_info;
#endif

int henlein_init();

#if defined(HENLEIN_WIN32)
#define HENLEIN_IMPLEMENTATION(scale)\
	LARGE_INTEGER value;\
	QueryPerformanceCounter(&value);\
	return ((uint64_t)value.QuadPart * (1000000000ULL/(scale))) /\
		henlein_frequency.QuadPart;
#elif defined(HENLEIN_OSX)
#define HENLEIN_IMPLEMENTATION(scale)\
	return (mach_absolute_time() * henlein_timebase_info.numer) /\
		henlein_timebase_info.denom / (scale);
#elif defined(HENLEIN_SUNOS) || defined(HENLEIN_HPUX)
#define HENLEIN_IMPLEMENTATION(scale)\
	return gethrtime() / (scale);
#elif defined(HENLEIN_POSIX)
#ifdef HENLEIN_CLOCK
#if scale == 1000000ULL
#define HENLEIN_CLOCK_BEST HENLEIN_CLOCK_MS
#else
#define HENLEIN_CLOCK_BEST HENLEIN_CLOCK
#endif
#define HENLEIN_IMPLEMENTATION(scale)\
	struct timespec ts;\
	if (clock_gettime(HENLEIN_CLOCK_BEST, &ts) != -1)\
		return ((uint64_t)ts.tv_sec * (1000000000ULL/(scale))) + (ts.tv_nsec/(scale));\
	else\
		return 0xFFFFFFFFFFFFFFFFULL;
#else
	/* non-highres fallback */
#define HENLEIN_IMPLEMENTATION(scale)\
	struct timeval tm;\
	gettimeofday(&tm, NULL);\
	return ((uint64_t)tm.tv_sec * (1000000000ULL/(scale))) + ((tm.tv_usec * 1000ULL)/(scale));
#endif
#else
#error "no high-resolution timer implementation"
#endif

/* returns a high-precision OS-specific time value in nanoseconds */
static inline uint64_t henlein_now() {
	HENLEIN_IMPLEMENTATION(1ULL);
}

/* returns a high-precision OS-specific time value in microseconds */
static inline uint64_t henlein_unow() {
	HENLEIN_IMPLEMENTATION(1000ULL);
}

/* returns a high-precision OS-specific time value in milliseconds */
static inline uint64_t henlein_mnow() {
	HENLEIN_IMPLEMENTATION(1000000ULL);
}

/* returns t2 - t1; assumes that t2 > t1 */
static inline uint64_t henlein_diff(uint64_t t2, uint64_t t1) {
	return t2 < t1 ? (UINT64_MAX - t1) - t2 + 1 : t2 - t1;
}

/* aliases */
#ifndef HENLEIN_NO_SHORT_ALIASES
#define hlinit henlein_init
#define hlnow  henlein_now
#define hlnnow henlein_now
#define hlunow henlein_unow
#define hlmnow henlein_mnow
#define hldiff henlein_diff
#endif

#endif
