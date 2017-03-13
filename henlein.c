/* See LICENSE file for copyright and license details. */

#if defined(__APPLE__) || defined(__FreeBSD__)
#include <sys/types.h>
#include <sys/sysctl.h>
#elif defined(_GNU_SOURCE)
#include <sys/sysinfo.h>
#endif

#ifndef _WIN32
#include <unistd.h>
#endif

#if defined(__GNUC__)
#include <cpuid.h>
#elif defined(_MSC_VER)
#include <intrin.h>
#endif

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

static int
nprocs()
{
	int count = -1;
#if defined(__APPLE__) || defined(__FreeBSD__)
	size_t size = sizeof(count);
	if (sysctlbyname("hw.ncpu", &count, &size, NULL, 0))
		return -1;
#elif defined(_SC_NPROCESSORS_ONLN)
	count = sysconf(_SC_NPROCESSORS_ONLN);
	count = (count > 0) ? count : -1;
#elif defined(_GNU_SOURCE)
	count = get_nprocs();
#elif defined(_WIN32)
	SYSTEM_INFO info;
	memset(info, 0, sizeof(info));
	GetSystemInfo(&info);
	count = info.dwNumberOfProcessors;
#endif
	return count;
}

int
henlein_tsc_support()
{
#if defined(__x86_64__) || defined(__i386__) ||\
    defined(_M_X64) || defined(_M_IX86)
	enum vendorid {
		intel,
		amd,
		other
	};

	uint32_t eax, ebx, ecx, edx;
	uint32_t family, model;
	int vendor;

#ifdef __GNUC__
#define CPUID(i,a,b,c,d) __cpuid(i,a,b,c,d)
#elif _MSC_VER
#define CPUID(i,a,b,c,d)\
	do {\
		unsigned int reg[4];\
		__cpuid(r, i);\
		a = r[0]; b = r[1]; c = r[2]; d = r[3];\
	} while (0);
#else
#define CPUID(i,a,b,c,d)\
	asm volatile("cpuid"\
		: "=a" (a), "=b" (b), "=c" (c), "=d" (d)\
		: "a" (i))
#endif

	CPUID(0x00000000, eax, ebx, ecx, edx);

	if (ebx == 0x756e6547 && ecx == 0x6c65746e && edx == 0x49656e69)
		vendor = intel;
	else if (ebx == 0x68747541 && ecx == 0x444d4163 && edx == 0x69746e65)
		vendor = amd;
	else
		vendor = other;

	/* tsc invariant flag */
	if (eax >= 0x00000007) {
		CPUID(0x00000007, eax, ebx, ecx, edx);
		if (edx & 0x00000100)
			return HL_INVARIANT_TSC;
	}

	/* get family */
	CPUID(0x00000001, eax, ebx, ecx, edx);
	family = ((eax & 0x00000f00) >> 8) | ((eax & 0x0ff00000) >> 20);
	model = ((eax & 0x000000f0) >> 4) | ((eax & 0x000f0000) >> 12);

	/* check intel version */
	switch (vendor) {
	case intel:
		if (family == 0x6 && model >= 0xe)
			return HL_INVARIANT_TSC;
		if (family == 0xf && model >= 0x3)
			return HL_INVARIANT_TSC;
		/* early pentiums */
		if (family == 0x5)
			return HL_STABLE_TSC;
		/* pentium iii & m */
		if (family == 0xf || family == 0x6) {
			/* only support single core */
			if (nprocs() != 1)
				return HL_UNSTABLE_TSC;
			/* check if enhanced speed-step is available */
			if (ecx & 0x80)
				return HL_UNSTABLE_TSC;
			else
				return HL_STABLE_TSC;
		}
		if (family >= 0x6)
			return HL_UNSTABLE_TSC;
		break;
	case amd:
		if (eax == 0x60fb2)
			return HL_INVARIANT_TSC;
		if (family >= 0x10)
			return HL_INVARIANT_TSC;
		break;
	}
#endif
	return  HL_NOT_SUPPORTED;
}

int64_t
henlein_tsc_measure(int ms)
{
#if defined(__x86_64__) || defined(__i386__) ||\
    defined(_M_X64) || defined(_M_IX86)
	uint64_t start_rdtsc = HENLEIN_RDTSC();
#ifndef _WIN32
	usleep(ms * 1000);
#else
	Sleep(ms);
#endif
	uint64_t stop_rdtsc = HENLEIN_RDTSC();
	int64_t diff_rdtsc = stop_rdtsc - start_rdtsc;
	if(diff_rdtsc < 0) diff_rdtsc = -diff_rdtsc;
	return diff_rdtsc;
#else
	return 0LL;
#endif
}
