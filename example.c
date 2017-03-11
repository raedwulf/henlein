/* See LICENSE file for copyright and license details. */

#include "henlein.h"
#include <stdio.h>
#include <inttypes.h>

int
main()
{
	int rc;
	volatile int unopt;
	uint64_t start, stop, diff;

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

	return 0;
}
