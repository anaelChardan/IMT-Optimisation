#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // memset
#include <stdint.h> // uintxx_t types
#ifdef WIN32
#include <windows.h> //
#else
#include <sys/time.h> // gettimeofday
#endif

#include "time_measure.h"


int main(int argc, const char * argv[])
{
	int status = 0;
	process_data_t pdata;
	uint32_t loop_counter = LOOPS;
	double execution_duration = 0.0;
#ifdef WIN32
	FILETIME temp_time = {0,0};
	ULARGE_INTEGER start, stop;
#else
	struct timeval start, stop;
#endif
	
	status = init_process_data(&pdata, argc, argv);
	if( !status ) {
#ifdef WIN32
		GetSystemTimeAsFileTime(&temp_time);
		start.HighPart = temp_time.dwHighDateTime;
		start.LowPart = temp_time.dwLowDateTime;
#else
		gettimeofday(&start, NULL);
#endif
		while( !status && loop_counter-- ) {
			status = pdata.data_processing_function(&pdata);
		}
		if( !status ) {
#ifdef WIN32
			GetSystemTimeAsFileTime(&temp_time);
			stop.HighPart = temp_time.dwHighDateTime;
			stop.LowPart = temp_time.dwLowDateTime;
			execution_duration = ((double)((stop.QuadPart - start.QuadPart) / 10000)) / 1000.;
#else
			gettimeofday(&stop, NULL);
			execution_duration = ((double)(1000 * (stop.tv_sec - start.tv_sec) + ((stop.tv_usec - start.tv_usec) / 1000))) / 1000.;
#endif
			printf("operation totals : %04f seconds (ie %04f seconds per iteration)\n", execution_duration, execution_duration / ((double) LOOPS));
		} else printf("data processing failed\n");
		dispose_process_data(&pdata);
	} else printf("process data initialization failed\n");
	
#ifdef WIN32
	system("pause");
#endif
	
	return status;
}

