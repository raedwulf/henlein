/* See LICENSE file for copyright and license details. */

#include "henlein.h"

#ifdef HENLEIN_WIN32
LARGE_INTEGER henlein_frequency;
#elif defined(HENLEIN_OSX)
mach_timebase_info_data_t henlein_timebase_info;
#endif

/* initialise the timers */
int
henlein_init()
{
#ifdef HENLEIN_WIN32
	QueryPerformanceFrequency(&henlein_frequency);
#elif defined(HENLEIN_OSX)
	mach_timebase_info(&henlein_timebase_info);
#endif
#ifdef HENLEIN_CLOCK
	/* to detect a faulty clock_gettime, we test twice
	   for the error code */
	uint64_t t = henlein_now();
	if (t == 0xFFFFFFFFFFFFFFFFULL && t == henlein_now())
		return -1;
#endif
	return 0;
}
