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
