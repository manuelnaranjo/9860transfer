#ifndef CASIO_9860__
#define CASIO_9860__

#include <usb.h>

/**
 * This method initilizates the calc transfer
 */
int init_9860(usb_dev_handle* usb_handle);


/**
 * 
 */
int sendPackage(struct usb_dev_handle* usb_handle, char* out, int len);
int readPackage(struct usb_dev_handle* usb_handle, char* out, int len);
int readBuffer(struct usb_dev_handle* usb_handle, char* out);

void parseHeader(char* input, int len);

#endif /*CASIO_9860__*/
