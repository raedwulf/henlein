/* See LICENSE file for copyright and license details. */

#include <stdio.h>
#include <inttypes.h>
#ifndef _WIN32
#include <unistd.h>
#endif

#include "henlein.h"

int
main()
{
	int rc, support;
	volatile int unopt;
	uint64_t start, stop, diff, cache_tsc, cache_mnow;
	int64_t cycles;

	rc = henlein_init();
	if (rc) return rc;

	start = henlein_now();
	unopt = 0;
	while (unopt < (1 << 24))
		unopt++;
	stop = henlein_now();
	diff = henlein_diff(stop, start);

	printf("start: %" PRIu64 " stop: %" PRIu64 " diff: %" PRIu64 "\n",
		start, stop, diff);

	if (!diff) return 1;

	start = henlein_now();
#ifdef HENLEIN_WIN32
	Sleep(1000);
#else
	sleep(1);
#endif
	stop = henlein_now();
	diff = henlein_diff(stop, start);

	printf("start: %" PRIu64 " stop: %" PRIu64 " diff: %" PRIu64 "\n",
		start, stop, diff);

	if (diff < 1000000000 || diff > 1100000000)
		return -2;

	start = henlein_unow();
#ifdef HENLEIN_WIN32
	Sleep(1000);
#else
	sleep(1);
#endif
	stop = henlein_unow();
	diff = henlein_diff(stop, start);

	printf("ustart: %" PRIu64 " ustop: %" PRIu64 " diff: %" PRIu64 "\n",
		start, stop, diff);

	if (diff < 1000000 || diff > 1100000)
		return -2;

	start = henlein_mnow();
#ifdef HENLEIN_WIN32
	Sleep(1000);
#else
	sleep(1);
#endif
	stop = henlein_mnow();
	diff = henlein_diff(stop, start);

	printf("mstart: %" PRIu64 " mstop: %" PRIu64 " diff: %" PRIu64 "\n",
		start, stop, diff);

	if (diff < 1000 || diff > 1100)
		return -2;

	support = henlein_tsc_support();
	switch (support) {
	case HL_INVARIANT_TSC:
		printf("tsc support: invariant tsc\n"); break;
	case HL_STABLE_TSC:
		printf("tsc support: stable tsc\n"); break;
	case HL_UNSTABLE_TSC:
		printf("tsc support: unstable tsc\n"); break;
	default:
		printf("tsc support: not supported\n");
		return 0;
	}

	cycles = henlein_tsc_measure(1) >> 2;
	printf("tsc measure (0.5 ms): %" PRIi64 "\n", cycles);

	cache_tsc = 0;
	cache_mnow = 0;
	start = henlein_cmnow(cycles, &cache_tsc, &cache_mnow);
#ifdef HENLEIN_WIN32
	Sleep(2);
#else
	usleep(2000);
#endif
	stop = henlein_cmnow(cycles, &cache_tsc, &cache_mnow);
	diff = henlein_diff(stop, start);

	printf("cmstart: %" PRIu64 " cmstop: %" PRIu64 " diff: %" PRIu64 "\n",
		start, stop, diff);

	if (diff < 1 || diff > 10)
		return -2;

	start = henlein_cmnow(cycles, &cache_tsc, &cache_mnow);
#ifdef HENLEIN_WIN32
	Sleep(1000);
#else
	sleep(1);
#endif
	stop = henlein_cmnow(cycles, &cache_tsc, &cache_mnow);
	diff = henlein_diff(stop, start);

	printf("cmstart: %" PRIu64 " cmstop: %" PRIu64 " diff: %" PRIu64 "\n",
		start, stop, diff);

	if (diff < 1000 || diff > 1100)
		return -2;

	return 0;
}
