

WriteUSB(char* data, int len) {
	retval = usb_bulk_write(usb_handle, 0x1, data, len, 100);
	
	debug(0, data, ret);
	if (ret < 0){
		fprintf(stderr, "[E] Failed WriteUSB. ERR = %i\n", ret);
	}
	
	return ret;
	
	
}

ReadUSB(char* data, int len) {
	int ret;
	retval = usb_bulk_read(usb_handle, 0x82, data, len, 1000);
	
	debug(1, data, ret);
	if (ret < 0){
		fprintf(stderr, "[E] Failed ReadUSB. ERR = %i\n", ret);
	}
	
	return ret;
}

