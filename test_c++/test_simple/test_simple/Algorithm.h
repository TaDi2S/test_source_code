#pragma once

#define MAXKC	(128/32)
#define MAXKB	(128/8)
#define MAXNR	10

typedef unsigned char	u8;
typedef unsigned short	u16;
typedef unsigned int	u32;

class Algorithm
{
public:
	void rijndaelDecrypt(const u32 rk[], const u8 ct[16], u8 pt[16]);
	void rijndaelEncrypt(const u32 rk[], const u8 pt[16], u8 ct[16]);
	int rijndaelKeySetupDec(u32 rk[], const u8 cipherKey[]);
	int rijndaelKeySetupEnc(u32 rk[], const u8 cipherKey[]);
	Algorithm();
	virtual ~Algorithm();

};
