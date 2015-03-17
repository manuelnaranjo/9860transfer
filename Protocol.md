**Note: Protocol documentation is being written and kept at http://xfer9860.sourceforge.net/doc/.**

For software using the protocol of the Casio fx-9860G and fx-9860 G SD, check out xfer9860. You can find xfer9860 at sourceforge at http://xfer9860.sourceforge.net/, version 0.1 of xfer9860 is able to send files to the calculators flash memory.

# (Old stuff) #
In this page we will make the protocol analysis:

All values are in HEX.

# Initialization #

**SEND: Inquiry:**
> | ` 05 30 30 30 `|
|:---------------|
> | ` 37 37       `|

**RECV: Acknowledge:**
> |` 06 30 30 30 37 30 `|
|:--------------------|

**SEND: Identify:**
> |` 01 30 31 30 `|
|:--------------|
> |` 36 46 `|

**RECV:**

Transmission is divided in 3 packages, the most long is 60d bytes long, and the hole transfer seems to be off 174d (AE) bytes long.

_Transfer:_
| `06 30 32 31 30 30 41 34 47 79 33 36 33 30 30 37` | `.02100A4Gy363007 `|
|:--------------------------------------------------|:-------------------|
| `52 45 4E 45 53 41 53 20 53 48 37 33 33 37 30 31` | `RENESAS SH733701 `|
| `30 30 30 30 30 30 30 30 30 30 30 30 34 30 39 36` | `0000000000004096 `|
| `30 30 30 30 30 31 32 38 FF FF FF FF FF FF FF FF` | `00000128ÿÿÿÿÿÿÿÿ `|
| `FF FF FF FF FF FF FF FF 30 30 2E 31 30 2E 30 30` | `ÿÿÿÿÿÿÿÿ00.10.00 `|
| `31 30 FF FF FF FF FF FF 30 30 30 30 30 30 30 30` | `10ÿÿÿÿÿÿ00000000 `|
| `30 30 30 30 30 30 36 34 30 31 2E 30 33 2E 30 30` | `0000006401.03.00 `|
| `30 30 FF FF FF FF FF FF 30 30 30 31 30 30 30 30` | `00ÿÿÿÿÿÿ00010000 `|
| `30 30 30 30 32 34 39 36 37 2E 30 30 56 54 4F 4E` | `000024967.00VTON` |
| `4A 46 6F 47 FF FF FF FF FF FF FF FF 4D 41 4E 55` | `JFoGÿÿÿÿÿÿÿÿMANU` |
| `45 4C FF FF FF FF FF FF FF FF FF FF 34 32      ` | `ELÿÿÿÿÿÿÿÿÿÿ42  ` |

_Protocol Analysis:_

| `06 30 32 31 30 30 <LEN> 47 79 33 36 33 30 30 37` | `.02100A4Gy363007` | 

&lt;LEN&gt;

 Seems to be Total Transfer - 10 (Possible header len?)|
|:--------------------------------------------------|:-------------------|:----------------------------------------------------------------------|
| `<MICRO INFO>>` | `RENESAS SH733701` |  |
| `31 30 30 30 30 30 30 30 30 30 30 30 34 30 39 36` | `0000000000004096` |  |
| `36 30 30 30 30 31 32 38 FF FF FF FF FF FF FF FF` | `00000128ÿÿÿÿÿÿÿÿ` |  |
| `FF FF FF FF FF FF FF FF 30 30 2E 31 30 2E 30 30` | `ÿÿÿÿÿÿÿÿ00.10.00` |  |
| `30 30 FF FF FF FF FF FF 30 30 30 30 30 30 30 30` | `10ÿÿÿÿÿÿ00000000` |  |
| `30 30 30 30 30 30 36 34 30 < ROMVERSION ROMVERS` | `0000006401.02.00` |  |
| `ION > FF FF FF FF FF FF 30 30 30 31 30 30 30 30` | `00ÿÿÿÿÿÿ00010000` |  |
| `30 30 30 30 32 34 39 36 37 2E 30 30 ?? ?? ?? ??` | `000024967.00PECB` | ?? Junk that changed between Manuel's calc and Andreas's |
| `?? ?? ?? ?? FF FF FF FF FF FF FF FF < NAME NAME` | `BGhlÿÿÿÿÿÿÿÿAAAA` | Name is up to 8 chars Complete with FF |
| `NAME NAME > FF FF FF FF FF FF FF FF ?? ??      ` | `AAAAÿÿÿÿÿÿÿÿ11  ` |  |




# User Memory #
**Ask System Settings:**
> |`01 33 33 30`|
|:------------|
> |`36 41`|

**Start:**
> |`03 30 30 30 `|
|:-------------|
> |`37 30 `|

**Next Element:**
> |`06 30 30 30`|
|:------------|
> |`37 30`|

_Example Memory Variable:_
01 33 32 31 30 30 31 46 30 30 38 30 30 30 30 30		.321001F00800000
30 30 30 30 30 35 30 32 30 30 30 30 30 30 30 30		0000050200000000
41 6E 67 6C 65 30 31 42 43				Angle01BC


```
Example settings:
01 33 32 31 30 30 31 46 30 30 38 30 30 30 30 30		.321001F00800000
30 30 30 30 30 35 30 32 30 30 30 30 30 30 30 30		0000050200000000
41 6E 67 6C 65 30 31 42 43				Angle01BC

01 33 32 31 30 30 31 45 30 30 38 30 30 30 30 30		.321001E00800000
30 30 30 30 30 34 30 32 30 30 30 30 30 30 30 30		0000040200000000
4D 6F 64 65 30 30 32 31					Mode0021

01 33 32 31 30 30 32 33 30 30 38 30 30 30 30 30		.321002300800000
30 30 30 30 30 39 30 32 30 30 30 30 30 30 30 30		0000090200000000
46 75 6E 63 20 54 79 70 65 30 30 36 34			Func Type0064

Protocol?
01 33 32 31 30 30 XX XX 30 30 38 30 30 30 30 30		// X = Y + 0x1A (address?)
30 30 30 30 YY YY 30 32 30 30 30 30 30 30 30 30		// Y = length of SETTING NAME
<SETTING NAME> <VALUE *4>

No more left:
	03 30 30 30 37 30

Ask memory: (gives memory and basic programs in main memory)
	01 32 39 30
	36 35

Example memory:
01 32 35 31 30 30 32 32 30 30 30 30 30 30 30 30		.251002200000000
30 30 31 38 30 30 30 31 30 39 30 30 30 30 30 30		0018000109000000
41 41 4C 50 48 41 20 4D 45 4D 36 42			AALPHA MEM6B

NEXT

02 32 35 31 30 30 33 33 30 30 30 31 30 30 30 31		.251003300010001
5C 30 5C 32 50 5C 20					\0\2P\

5C 20 5C 20 5C 20 5C 20 5C 20 5C 20 5C 20 5C 20		\ \ \ \ \ \ \ \ 
5C 20 5C 20 5C 20 5C 27 80 5C 2C 5C 3F 86 80 5C		\ \ \ \'€\,\?†€\
2C 5C 3F A4 46 33					,\?¤F3

A=0x19

01 32 35 31 30 30 32 32 30 30 30 30 30 30 30 30		.251002200000000
30 30 31 38 30 30 30 31 30 39 30 30 30 30 30 30		0018000109000000
42 41 4C 50 48 41 20 4D 45 4D 36 41			BALPHA MEM6A

NEXT

02 32 35 31 30 30 33 33 30 30 30 31 30 30 30 31		.251003300010001
5C 30 5C 39 60 5C 20 5C 20 5C 20 5C 20 5C 20 5C		\0\9`\ \ \ \ \ \
20 5C 20 5C 20 5C 20 5C 20 5C 20 5C 20 5C 27 80		 \ \ \ \ \ \ \'€
5C 2C 5C 3F 86 80 5C 2C 5C 3F A4 44 43			\,\?†€\,\?¤DC

B=0x60

01 32 35 31 30 30 32 43 30 30 30 31 30 30 30 30		.251002C00010000
30 30 46 38 30 36 30 37 30 37 30 30 30 30 30 30		00F8060707000000
73 79 73 74 65 6D 41 50 50 46 52 41 43 50 52 4F		systemAPPFRACPRO
47 52 41 4D 32 36					GRAM26

NEXT
02 32 35 31 30 31 35 31 30 30 30 31 30 30 30 31		.251015100010001
5C 20 5C 20 5C 20 5C 20 5C 20 5C 20 5C 20 5C 20		\ \ \ \ \ \ \ \ 
5C 20 5C 20 F7 5C 38 5C 2D 22 4E 22 3F 5C 2E 41		\ \ ÷\8\-"N"?\.A
7E 42 5C 2D 31 5C 2E 52 5C 2D F7 5C 2A 5C 2D 31		~B\-1\.R\-÷\*\-1

30 41 5C 2E 41 5C 2D 31 30 52 5C 2E 52 5C 2D F7		0A\.A\-10R\.R\-÷
5C 2B B6 41 5C 31 30 5C 2D 31 5C 2E 53 7E 59 5C		\+¶A\10\-1\.S~Y\
2D 30 5C 2E 54 7E 58 5C 2D F7 5C 2A 5C 2D 41 5C		-0\.T~X\-÷\*\-A\
2E 4D 3A 52 5C 2E 41 5C 2D A6 28 4D B9 41 5C 2E		.M:R\.A\-¦(M¹A\.

51 5C 2D 4D 99 51 41 5C 2E 52 5C 2D 51 59 89 54		Q\-M™QA\.R\-QY‰T
5C 2E 55 5C 2D 51 58 89 53 5C 2E 56 5C 2D 55 B9		\.U\-QX‰S\.V\-U¹
56 5C 2E 5A 5C 2D 42 99 5A 5C 2E 43 5C 2D F7 5C		V\.Z\-B™Z\.C\-÷\
38 5C 2D F7 5C 30 32 2C 31 2C 55 5C 2D F7 5C 30		8\-÷\02,1,U\-÷\0

32 2C 32 2C 56 5C 2D F7 5C 30 32 2C 33 2C 22 99		2,2,V\-÷\02,3,"™
99 99 99 99 99 99 99 99 99 99 99 99 22 5C 2D F7		™™™™™™™™™™™™"\-÷
5C 30 32 2C 34 2C 5A 5C 2D F7 5C 30 32 2C 35 2C		\02,4,Z\-÷\02,5,
42 5C 2D F7 5C 30 32 2C 36 2C 22 99 99 99 99 99		B\-÷\02,6,"™™™™™

99 99 99 99 99 99 99 99 22 5C 2D F7 5C 30 32 2C		™™™™™™™™"\-÷\02,
37 2C 97 43 5C 2D F7 5C 2A 5C 2D F7 5C 2B 7F 8F		7,—C\-÷\*\-÷\+
5C 31 33 31 5C 2D 59 5C 2E 54 3A 55 5C 2E 59 5C		\131\-Y\.T:U\.Y\
2D 58 5C 2E 53 3A 56 5C 2E 58 5C 2D F7 5C 2B 43		-X\.S:V\.X\-÷\+C

5C 31 30 5C 2D F7 5C 30 32 2C 37 2C 22 45 4E 44		\10\-÷\02,7,"END
22 5C 20 5C 20 5C 20 5C 20 46 30			"\ \ \ \ F0

APPFRAC:
ClrText
"N"?->A~B
1->R
Do
10A->A
10R->R
LpWhile Frac A!=0
1->S~Y
0->T~X
Do
A->M:R->A
Int (M/A->Q
M-QA->R
QY+T->U
QX+S->V
U/V->Z
B-Z->C
ClrText
Locate 2,1,U
Locate 2,2,V
Locate 2,4,"-------------"
Locate 2,4,Z
Locate 2,5,B
Locate 2,6,"-------------"
Locate 2,6,Abs C
Do
LpWhile GetKey!=31
Y->T:U->Y
X->S:V->X
LpWhile C!=0
Locate 2,7,"END"

```





