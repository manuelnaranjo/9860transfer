SendPositive(char *subtype) {
	buffer[0] = 0x01;
	buffer[1] = subtype;
	buffer[3] = '0';
	buffer[4] = "??";	// FIXME
	
	return WriteUSB(buffer,6);
	
}

SendNegative(char *subtype) {
	struct Packet o;
	o.raw_type = 0x15;
	o.raw_subtype = *subtype;
	o.raw_datafollows = '0';
	o.raw_checksum = "??"; // CREATE CHECKSUM ROUTINE AND FIX THIS PROPERLY