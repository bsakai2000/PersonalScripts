#include "xbacklight.h"

// A barebones xbacklight clone using this API
int main(int argc, char* argv[])
{
	// Run initialization
	backlight_setup();

	switch(argc)
	{
		// If we have no args, print general information
		case 1:
			printf("Min: %ld\nMax: %ld\nCurrent: %ld\nPercentage: %f%%\n",
					backlight_min(), backlight_max(), backlight_get(),
					100 * ((double) backlight_get() - backlight_min()) / (backlight_max() - backlight_min()));
			return 1;
		// If we have one arg, set the backlight to that number
		case 2:
			backlight_set(atoi(argv[1]));
			break;
		// If we have a wrong number of args, print usage information
		default:
			printf("Usage: %s <backlight number>\n", argv[0]);
			break;
	}
	return 0;
}
