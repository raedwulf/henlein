#include "henlein.h"

int main() {
	int rc;
	volatile int unopt;
	uint64_t start, stop, diff;

	rc = henlein_init();
	if(rc) return rc;

	start = henlein_now();
	unopt = 0;
	while(unopt < (1 << 24))
		unopt++;
	stop = henlein_now();
	diff = henlein_diff(stop, start);

	if(diff == 0) return 1;

	start = henlein_now();
#ifdef HENLEIN_WIN32
	Sleep(1000);
#else
	sleep(1);
#endif
	stop = henlein_now();
	diff = henlein_diff(stop, start);
	if(diff < 1000000000 || diff > 1100000000)
		return -2;

	return 0;
}
