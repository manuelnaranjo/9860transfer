#include <stdio.h>
#include <string.h>

#define PT_POSITIVE_NORMAL 0x00

#define ERR_UNEXPECTED -10
#define ERR_PACKETFORMAT -11
#define ERR_CHECKSUM -12

bool	expectingcommand = 0,
	expectingdata = 0,
	expectingpositive = 0,
	expectingswitch = 0;
	
int ret = 0;
struct Packet {
	char raw_type;
	char raw_subtype[2];
	char raw_datafollows;
	char raw_datasize[4];
	char raw_data[520];	// possibly the biggest packet size ??
	char raw_checksum[2];
}
char buffer[530];