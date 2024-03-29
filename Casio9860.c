/*
 * Casio fx-9860 Protocol - Program to connect to Casio fx-9860
 * from user space using libusb
 *
 * Copyright (C) 2007
 *	Manuel Naranjo (naranjo.manuel@gmail.com)
 *	Andreas Bertheussen (andreasmarcel@gmail.com)
 * Copyright (C) 2004
 *	Greg Kroah-Hartman (greg@kroah.com)
 *
 * This program is free software; you can
 * redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the
 * License.
 *
 */
#include <stdio.h>
#include <string.h>

#include "Casio9860.h"

#define C9860_VENDOR_ID   0x07CF
#define C9860_PRODUCT_ID  0x6101

#define LEN_LINE	16

#define __DUMP__

/*
 * Private methods
 */
void debug(int input, char* array, int len){
#ifdef __DEBUG__
#endif //__DEBUG__
#ifdef __DUMP__
	int i;
	unsigned char temp;
	int j, line = 0;
	
	
	if (input==0)
		fprintf(stderr, ">> ");
	else
		fprintf(stderr, "<< ");
	
	for (i = 0 ; i < len ; i++){
		temp = (unsigned char) array[i];
		
		if (i % LEN_LINE == 0 && i != 0){
			fprintf(stderr, "\t");
			for (j = line; j < line + LEN_LINE; j++){
				char u = (unsigned char) array[j];
				if (u > 31)
					fprintf(stderr, "%c", u);
				else
					fprintf(stderr, ".");
			}
			
			line = i;
			fprintf(stderr,"\n");
			if (input==0)
				fprintf(stderr, ">> ");
			else
				fprintf(stderr, "<< ");			
		}
		
		fprintf(stderr,"%02X ", temp);
		
	}
	
	if (i % LEN_LINE != 0)	
		for (j = 0 ; j < (int)(LEN_LINE-(i % LEN_LINE)); j++)
			fprintf(stderr,"   ");
	
	fprintf(stderr, "\t");
	for (j = line; j < len; j++){
		temp = (short unsigned int) array[j];
		if (temp > 31)
			fprintf(stderr, "%c", temp);
		else
			fprintf(stderr, ".");			
	}
	
	fprintf(stderr,"\n\n");
#endif //__DUMP__
}

struct usb_device *device_init(void)
{
	struct usb_bus *usb_bus;
	struct usb_device *dev;
	usb_init();
	usb_find_busses();
	usb_find_devices();
	for (usb_bus = usb_busses;
		usb_bus;
		usb_bus = usb_bus->next) {
		for (dev = usb_bus->devices; dev; dev = dev->next) {
			if ((dev->descriptor.idVendor == C9860_VENDOR_ID) && (dev->descriptor.idProduct == C9860_PRODUCT_ID))
				return dev;
		}
	}    
	return NULL;
}

/**
 * Public Methods
 */
int init_9860(struct usb_dev_handle *usb_handle){
	char* out;
	int retval;
	out = calloc(0x12, sizeof(char));
	    
	retval = usb_control_msg(usb_handle, 0x80, 0x6, 0x100, 0, out, 0x12, 200);
	debug(1, out, retval);
	       
	if (retval < 0){
		fprintf(stderr, "Not able to send first message\n");
		return retval;
	}
	       
	out = (char *)realloc(out, (size_t) (0x29 * sizeof(char)));
	     
	retval = usb_control_msg(usb_handle, 0x80, 0x6, 0x200, 0, out, 0x29, 250);
	debug(1, out, retval);

	if (retval < 0){
		fprintf(stderr, "Not able to send second message\n");
		return retval;
	}

	out = (char *)realloc(out, (size_t) (0x1 * sizeof(char)));

	retval = usb_control_msg(usb_handle, 0x41, 0x1, 0x0, 0, out, 0x0, 250);
	debug(1, out, retval);

	if (retval < 0){
		fprintf(stderr, "Not able to send third message\n");
			fprintf(stderr, "Not able to send third message\n");
		return retval;
	}

	return 0;
}

int main(int argc, char **argv){
	int retval = 1;
	char* out;
	char* temp;
	struct usb_device *usb_dev;
	struct usb_dev_handle *usb_handle;

	usb_dev = device_init();

	if (usb_dev == NULL) {
		fprintf(stderr, "Device not found\n");        
		goto exit;
	}

	usb_handle = usb_open(usb_dev);
	if (usb_handle == NULL) {
		fprintf(stderr, "Not able to claim the USB device\n");
		goto exit_close;
	}

	retval = usb_claim_interface(usb_handle, 0);
	if (retval < 0) {
		fprintf(stderr, "Not able to claim USB Interface\n");
		goto exit_unclaim;
	}
	       
	retval = init_9860(usb_handle);

	if (retval < 0){
		fprintf(stderr, "Couldn't initilizate device\n");
		goto exit_unclaim;
	}

	out = calloc(0x40, sizeof(char));
	
	// Connection verification, start of communication
	out[0] =0x05; out[1]=0x30; out[2]=0x30; out[3]=0x30; out[4]=0x37; out[5]=0x30;  
	retval = sendPackage(usb_handle,out,6);
	if (retval < 0)		
		goto exit_unclaim;

	retval = readPackage(usb_handle,out,6);
	if (retval < 0)		
		goto exit_unclaim;
	
 	// Request device identification
 	out[0] = 0x01; out[1] = 0x30; out[2] = 0x31; out[3]=0x30; out[4] = 0x36; out[5] = 0x46;
 	retval = sendPackage(usb_handle,out,6);
	if (retval < 0)			
		goto exit_unclaim;

	usleep(50*1000);
	
	temp = malloc (sizeof(char) * 0x40 * 3);
	
	retval = readPackage(usb_handle,out,0x40);
 	if (retval < 0)
 		goto exit_unclaim;
 		
 	memcpy(temp, out, retval);
 	 	
 	usleep(50*1000);	
 	retval = readPackage(usb_handle,out,0x40);
 	if (retval < 0)
 		goto exit_unclaim;
 	
 	memcpy(temp + 0x40, out, 0x40);
 	usleep(50*1000);
 	retval = readPackage(usb_handle,out,0x2e);
 	if (retval < 0)
 		goto exit_unclaim;

	memcpy(temp + 0x80, out, retval);
	parseHeader(temp, 0x80 + retval);
	
	// Request all settings
	out[0] = 0x01; out[1] = 0x33; out[2] = 0x33; out[3] = 0x30; out[4] = 0x36; out[5] = 0x41;
	retval = sendPackage(usb_handle,out, 6); 		
	if (retval < 0)
		goto exit_unclaim;

	retval = readPackage(usb_handle,out, 6);
	if (retval < 0)
		goto exit_unclaim;

	// Change leadership to calculator
	out[0]=0x03; out[1]=0x30; out[2]=0x30; out[3]=0x30; out[4]=0x37; out[5]=0x30;
	retval = sendPackage(usb_handle,out,6);
	if (retval < 0)
		goto exit_unclaim;
	
get_settings: 
	retval = readBuffer(usb_handle, out);
	if (retval < 0)
		goto exit_unclaim;
		
	if (out[0] == 0x01) {	// if incoming is data, ack it.
		out[0] = 0x06; out[1]=0x30; out[2]=0x30; out[3]=0x30; out[4] = 0x37; out[5]=0x30;
		retval = sendPackage(usb_handle,out, 6);
		if (retval < 0)
			goto exit_unclaim;
		 
		usleep(100*1000);
		goto get_settings; 
	}
	
	// Get RAM image
	out[0]=0x01; out[1]=0x32; out[2]=0x39; out[3]=0x30; out[4]=0x36; out[5]=0x35;
	retval = sendPackage(usb_handle,out, 6);
	if (retval < 0)
		goto exit_unclaim;

	retval = readPackage(usb_handle,out, 6);
	if (retval < 0)
		goto exit_unclaim;
		
	// let calc take control
	out[0]=0x03; out[1]=0x30; out[2]=0x30; out[3]=0x30; out[4]=0x37; out[5]=0x30;
	retval = sendPackage(usb_handle,out, 4);
	if (retval < 0)
		goto exit_unclaim;

get_ram:
	retval = 1;
	while(retval > 0) 	
		retval = readBuffer(usb_handle, out);
		
	if (retval < 0 || out[0]==0x03)	// goes on until 'direction change' packet is received
		goto exit_unclaim;
	
	out[0]=0x06; out[1]=0x30; out[2]=0x30; out[3]=0x30; out[4]=0x37; out[5]=0x30;
	retval = sendPackage(usb_handle,out, 6);
	if (retval < 0)
		goto exit_unclaim;
	
	//usleep(100*1000);
	goto get_ram;
	

exit_unclaim:
	usb_release_interface(usb_handle, 0);
exit_close:	
    usb_close(usb_handle);
	free(usb_dev);
exit:
	exit(retval);
    return retval;
}

/**
 * This method write n chars
 */
int sendPackage(struct usb_dev_handle *usb_handle, char* out, int len){
	int retval;
	retval = usb_bulk_write(usb_handle, 0x1, out, len, 100);
	
	debug(0, out, retval);
	if (retval < 0){
		fprintf(stderr, "Couldn't write package\n");
		fprintf(stderr, "ERR Num: %i", retval);
	}
	
	return retval;
}

/**
 * This method reads the hole buffer.
 */
int readBuffer(struct usb_dev_handle *usb_handle, char* out){
	return readPackage(usb_handle, out, 0x40);
}

/**
 * This method reads n chars
 */
int readPackage(struct usb_dev_handle *usb_handle, char* out, int len){
	int retval;
	retval = usb_bulk_read(usb_handle, 0x82, out, len, 1000);
	
	debug(1, out, retval);
	if (retval < 0){
		fprintf(stderr, "Couldn't read\n");
		fprintf(stderr, "ERR Num: %i", retval);
	}
	
	return retval;
}

void parseHeader(char* input, int len){
	char * temp;
	int i;	
	unsigned char t;
	
	if (len == 0)
		return;
		
	switch (input[0]){
		case 0x6:{
			temp = (char *) malloc(sizeof(char) * 16);		
			memcpy(temp, input + 16, 16);
			fprintf(stdout, "Proccesor: %s\n", (char*)temp);			
			
			memcpy(temp, input + (16 * 6 + 8) , 16);
			fprintf(stdout, "Firmware Version: ");
			for(i = 0 ; i < 16; i++){
				t = (unsigned char) temp[i];
				if (t != 0xff && t > 31)
					fprintf(stdout, "%c",t );
				else
					i = 16;
			}			
			fprintf(stdout, "\n");
			
			memcpy(temp, input + ( 9 * 16 + 12), 8);
			fprintf(stdout, "User Name: ");
			for(i = 0 ; i < 8; i++){
				t = (unsigned char) temp[i];
				if (t != 0xff && t > 31)
					fprintf(stdout, "%c",t );
				else
					i = 16;
			}
			
			fprintf(stdout, "\n");
			
			
			if (temp != NULL)
				free(temp);
			
			break;
		}
		
		case 0x1:
			break;
		
	};
	
}
