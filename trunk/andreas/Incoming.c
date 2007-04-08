int GetPacket(struct Packet *i) {
	char subtype[2];	// Used for conversion
	char datasize[4];	
	struct Packet i;	// Create packet struct to play with
	ReadUSB(i->raw_type,1);
	ReadUSB(i->raw_subtype,2);
	
	subtype = strtol(i->raw_subtype, NULL, 16);
	
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
	ReadUSB(i->raw_datafollows, 1);
	if (i->raw_datafollows != '0')		// Invalid 
		return ERR_PACKETFORMAT;
	ReadUSB(i->raw_checksum, 2);
	
	// CONNCHECK SHOULD NOT HAPPEN ON THIS SIDE? reply anyway
	SendPositive(PT_POSITIVE_NORMAL);
	
	
}

int HandlePositive(char subtype) {
	if (!expectingpositive) {
		fprintf(stderr, "\n[W] GOT UNEXPECTED POSITIVE\n");
		return ERR_UNEXPECTED;
	}
	ReadUSB(i->raw_datafollows,1);
	switch(subtype) {
		case ST_POSITIVE_OK:
			ReadUSB(i->raw_checksum, 2);
		case ST_POSITIVE_OVERWRITE:
			ReadUSB(i->raw_checksum, 2);
		case ST_POSITIVE_INFO:
			ReadUSB(i->raw_datasize, 4);
			datasize = (short int)strtoi(i->raw_datasize, NULL, 16);
			ReadUSB(i->raw_data, datasize);
			ReadUSB(i->raw_checksum, 2);
		default:
	}
	return 0;	// gotta return something more useful
}

// Net written yet
int HandleNegative(char subtype);
int HandleAbort(char subtype);
int HandleUnknown();