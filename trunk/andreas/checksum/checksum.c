/*
 * The algo seems to be correct, but it does not work on all packets,
 * Intended usage is to run it, enter the contents of the packet
 * (excluding first and the two last bytes, you can hack that to work
 * better), and press enter. My compiler warns about using gets, so
 * that could be fixed, not critical though ;)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	char buffer[256];
	int length;
	int i;
	char sum;

rescanf:
	gets(buffer);
	fflush(stdin);
	printf("Read in: %s\n", buffer);
	length = strlen(buffer);
	sum = 0;
	for (i = 0; i < length; i++) {
		sum += buffer[i];
	}
	printf("Checksum: %.02x\n", (~sum)+1);
	goto rescanf;


	return 0;
}
