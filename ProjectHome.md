This project aims to create an open source replacement for the FA-124 from Casio.

In the [downloads section](http://code.google.com/p/9860transfer/downloads/list) you can find two different logs from FA-124 (the first one is the initialization process + getting user memory, and the second one is a program uploading), while in the [wiki section](http://code.google.com/p/9860transfer/w/list) you can find my deductions on the protocol, a known hardware from the 9860 and a guide on how to build.

Anyone is welcomed to collaborate, there's no need to be a coder or hacker (while it's better) firstly we need to decode the protocol from the logs. You just need to be smart to get this out. All the logs are in hex base and have the ASCII equivalent next to it. EJ:

```

000027: Bulk or Interrupt Transfer (UP), 15.02.2007 17:47:56.1875000 +0.2500000
Pipe Handle: 0x81e963d4 (Endpoint Address: 0x82)
Get 0x29 bytes from the device:


01 33 32 31 30 30 31 46 30 30 38 30 30 30 30 30 .321001F00800000
30 30 30 30 30 35 30 32 30 30 30 30 30 30 30 30 0000050200000000
41 6E 67 6C 65 30 31 42 43                      Angle01BC
```

If you want to collaborate please contact me at naranjo _dot_ manuel _at_ gmail _dot_ com