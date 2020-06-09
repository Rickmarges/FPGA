/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"

#define read_PIO IORD_ALTERA_AVALON_PIO_DATA
#define write_PIO IOWR_ALTERA_AVALON_PIO_DATA

int jumpDelay = 100;

struct timeval currentTime;
//struct timeval previousDetectionMillis;
struct timeval t0;

unsigned int int_to_int(unsigned int k) {
	return (k == 0 || k == 1 ? k : ((k % 2) + 10 * int_to_int(k / 2)));
}

float timedifference_msec(struct timeval t0, struct timeval t1) {
	return (t1.tv_sec - t0.tv_sec) * 1000.0f
			+ (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

int main() {
	gettimeofday(&t0, 0);
	int jumpCounter = 0;
	int previousDetectionMillis = 0;
	int jumps[5];
	while (1) {
		gettimeofday(&currentTime, 0);
		int currentMillis = timedifference_msec(t0, currentTime);
		int total = int_to_int(read_PIO(GPI_BASE) % 100);
		int foregroundLDR = total % 10; // register cacti
		int backgroundLDR = (total / 10) % 10; // register background color
		// printf("Foreground LDR: %d -- Background LDR: %d -- Total: %d \n", foregroundLDR, backgroundLDR, total);
		if (jumpCounter > 0) {
			//printf("Counter: %d -- Value: %d\n", jumpCounter, jumps[jumpCounter]);
			for (int i = 1; i <= sizeof(jumps)/sizeof(jumps[0]); i++) {
				if (currentMillis >= jumps[i] && jumps[i] > 0) {
					write_PIO(GPO_BASE, 1 << 11); // jump
					printf("Jumps: %d -- detecttime: %d -- currentTime: %d\n", jumpCounter, jumps[i], currentMillis);
					jumps[i] = 0;
					if (i == 5) {
						jumpCounter = 0;
					}
					break;
				} else {
					write_PIO(GPO_BASE, 0);
				}
			}
		}
		// Only check for cacti once every tenth of a second so it won't flood pending jumps
		if (currentMillis - previousDetectionMillis >= 100) {
			// Use the 2 LDR's to check for cacti
			// The LDR which check the foreground has to be 1.4 times higher or lower than the background
			// This value is a threshold which will be exceded by the passing cacti
			if (foregroundLDR != backgroundLDR) {
				previousDetectionMillis = currentMillis;
				jumpCounter++;
				jumps[jumpCounter] = previousDetectionMillis + jumpDelay;
				printf("jumpcounter: %d -- millis: %d\n", jumpCounter, previousDetectionMillis);
			}
		}
	}
	return 0;
}
