#include <stdio.h>
#include <string.h>

int main() {
	char datasize[4] = "FFFF";
	printf("%i\n", strtol(datasize, NULL, 16));
	return 0;
}