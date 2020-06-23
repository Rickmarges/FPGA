/*
 * Jump-Duck Dino
 *
 * The Jump-Duck Dino project was created as an idea for the course FPGA at NHL Stenden.
 * This program uses multiple LDRs to play the Google Chrome's Dino game.
 *
 * One of the LDRs will check the color of the background
 * while the other two will be used to detect passing cacti.
 * Of the two to detect cacti one is in use with a light background
 * and the other one is used during the dark theme.
 *
 * Authors: Maryn Kaspers & Rick Margés
 * Date: 26-06-2020
 *
 */

#include <stdio.h>
#include <sys/time.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"

#define read_PIO IORD_ALTERA_AVALON_PIO_DATA
#define write_PIO IOWR_ALTERA_AVALON_PIO_DATA

// The initial value for the jump delay
int jumpDelay = 500;

// The two different timestamps to be used
struct timeval currentTime;
struct timeval t0;

// This method creates a binary number (in int format) from a normal int
unsigned int int_to_int(unsigned int k) {
	return (k == 0 || k == 1 ? k : ((k % 2) + 10 * int_to_int(k / 2)));
}

// This method calculates the time (in milliseconds) difference between two different time stamps
float timedifference_msec(struct timeval t0, struct timeval t1) {
	return (t1.tv_sec - t0.tv_sec) * 1000.0f
			+ (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

int main() {
	gettimeofday(&t0, 0);				// Set the begin value for the time
	int jumpCounter = 0;				// Start with zero jumps pending
	int previousDetectionMillis = 0;	// Since there are no previous detections, the timer is zero
	int jumps[5];						// Create an array to save different jumps with times

	while (1) {
		gettimeofday(&currentTime, 0);								// Every time the loop executes get the current time
		int currentMillis = timedifference_msec(t0, currentTime);	// Calculate the time difference in millisecond
		int total = int_to_int(read_PIO(GPI_BASE) % 100);			// Call the method int_to_int to convert an int to a binary number
		int backgroundLDR = total % 10; 							// register background color -- 1st bit
		int foregroundLDRDark = (total / 10) % 10; 					// register cacti dark theme -- 2nd bit
		int foregroundLDRLight = (total / 1000) % 10; 				// register cacti light theme - 4th bit

		// Check if there are jumps pending
		if (jumpCounter > 0) {
			// Loop through the array of stashed jumps
			for (int i = 1; i <= sizeof(jumps)/sizeof(jumps[0]); i++) {
				// If the current time is greater than or equal to
				// the time when a cactus was detected + a jumpdelay
				if (currentMillis >= jumps[i] && jumps[i] > 0) {
					write_PIO(GPO_BASE, 1 << 11); 	// Write to the output pin for the jump
					jumps[i] = 0;					// Set the time to jump back to zero
					if (i == 5) {					// If the jumpcounter equals 5 go back to 0
						jumpCounter = 0;
					}
					break;							// Break the loop so the system won't jump twice
				} else {
					write_PIO(GPO_BASE, 0);			// Set the output pins low to release the jump
				}
			}
		} else {
			write_PIO(GPO_BASE, 0);					// If there are no jumps pending set the output to low
		}

		// Only check for cacti once every tenth of a second so it won't flood pending jumps
		if (currentMillis - previousDetectionMillis >= 150) {
			// Use the 2 LDR's to check for cacti
			// If the background is dark the LDR will return a 1
			// So if the background is 1 it's a dark background and with 0 it's light
			if ((backgroundLDR == 1 && foregroundLDRDark == 0) || (backgroundLDR == 0 && foregroundLDRLight == 1)) {
				previousDetectionMillis = currentMillis;			// Save the time of the last detection
				jumpCounter++;										// Increase the counte of pending jumps
				jumps[jumpCounter] = currentMillis + jumpDelay;		// Save the current time + jump delay in the array
			}
		}

		// A reset fucntion when the right most button is pressed
		if (read_PIO(PUSHBUTTONS_BASE) == 1) {
			gettimeofday(&t0, 0);			// Set the begin time back to the current time
			previousDetectionMillis = 0;	// Reset the previous detection timer
			jumpCounter = 0;				// Reset the jump counter
			jumpDelay = 500;				// Set the jump delay back to it's begin value
		}
	}
	return 0;
}
