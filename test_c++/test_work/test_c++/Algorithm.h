// Algorithm.h: interface for the Algorithm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALGORITHM_H__311A2303_058A_4B54_BDB1_0CCC37CDA8BF__INCLUDED_)
#define AFX_ALGORITHM_H__311A2303_058A_4B54_BDB1_0CCC37CDA8BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

#endif // !defined(AFX_ALGORITHM_H__311A2303_058A_4B54_BDB1_0CCC37CDA8BF__INCLUDED_)
