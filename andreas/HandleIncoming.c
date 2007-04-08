#include <stdio.h>
#include <string.h>

#define PT_POSITIVE_NORMAL 0x00

#define ERR_UNEXPECTED -10
#define ERR_PACKETFORMAT -11
#define ERR_CHECKSUM -12

bool 	expectingcommand = 0,
	expectingdata = 0,
	expectingpositive = 0,
	expectingswitch = 0;
	
int ret = 0;

char raw_type;
char raw_subtype[2]
char raw_datafollows;
char raw_datasize[4];
char raw_data[520];	// possibly the biggest packet size ??
char raw_checksum[2];

char subtype;
short int datasize;

char buffer[530];

int GetPacket() {
	ReadUSB(raw_type,1);
	ReadUSB(raw_subtype,2);
	
	subtype = strtol(raw_subtype, NULL, 16);
	
	switch(type) {
		case T_COMMAND:	ret = HandleCommand(subtype); break;
		case T_DATA:		ret = HandleData(subtype); break;
		case T_SWITCH:		ret = HandleSwitch(subtype); break;
		case T_CONNCHECK:	ret = HandleConncheck(subtype); break;
		case T_POSITIVE:	ret = HandlePositive(subtype); break;
		case T_NEGATIVE:	ret = HandleNegative(subtype); break;
		case T_ABORT:		ret = HandleAbort(subtype); break;
		default:		ret = HandleUnknown();
	}
	if (ret < 0) {
		printf("\n[!] Negative retval in Listen(): RET = %i\n", ret);
	}
	return ret;
}

// These are not written yet
int HandleCommand(char subtype);
int HandleData(char subtype);
int HandleSwitch(char subtype);

int HandleConncheck(char subtype) {
	ReadUSB(raw_datafollows, 1);
	if (raw_datafollows != '0')		// Invalid 
		return ERR_PACKETFORMAT;
	ReadUSB(raw_checksum, 2);
	
	// CONNCHECK SHOULD NOT HAPPEN ON THIS SIDE? reply anyway
	SendPositive(PT_POSITIVE_NORMAL);
	
	
}

int HandlePositive(char subtype) {
	if (!expectingpositive) {
		fprintf(stderr, "\n[W] GOT UNEXPECTED POSITIVE\n");
		return ERR_UNEXPECTED;
	}
	ReadUSB(raw_datafollows,1);
	switch(subtype) {
		case ST_POSITIVE_OK:
			ReadUSB(raw_checksum, 2);
		case ST_POSITIVE_OVERWRITE:
			ReadUSB(raw_checksum, 2);
		case ST_POSITIVE_INFO:
			ReadUSB(raw_datasize, 4);
			datasize = (short int)strtoi(raw_datasize, NULL, 16);
			ReadUSB(raw_data, datasize);
			ReadUSB(raw_checksum, 2);
		default:
	}
	return 0;
}

// Net written yet
int HandleNegative(char subtype);
int HandleAbort(char subtype);
int HandleUnknown();