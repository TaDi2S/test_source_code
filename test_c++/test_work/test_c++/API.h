// API.h: interface for the API class.

//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_API_H__1FF35BC2_079D_11D5_BD32_0040D013BC1D__INCLUDED_)
#define AFX_API_H__1FF35BC2_079D_11D5_BD32_0040D013BC1D__INCLUDED_

#include "header.h"
#include "Algorithm.h"	// Added by ClassView
#include "KCDSA_CLASS.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define     DIR_ENCRYPT     0    /*  Are we encrpyting?  */
#define     DIR_DECRYPT     1    /*  Are we decrpyting?  */
#define     TRUE            1
#define     FALSE           0
#define		BITSPERBLOCK		128		/* Default number of bits in a cipher block */

/*  Error Codes - CHANGE POSSIBLE: inclusion of additional error codes  */
#define     BAD_KEY_DIR        -1  /*  Key direction is invalid, e.g.,
					unknown value */
#define     BAD_KEY_MAT        -2  /*  Key material not of correct 
					length */
#define     BAD_KEY_INSTANCE   -3  /*  Key passed is not valid  */

// for file check
#define		CANNOTOPEN	(-1)
#define		ORDISTATE	0
#define		TOOSHORT	1
#define		ENCRYPTED	2
#define		PLAINTEXT	3


/*  CHANGE POSSIBLE:  inclusion of algorithm specific defines  */
//#define     MAX_KEY_SIZE	64  /* # of ASCII char's needed to
//					represent a key */

/*  Typedefs:

	Typedef'ed data storage elements.  Add any algorithm specific 
parameters at the bottom of the structs as appropriate.
*/

/*  The structure for key information */
#define     MAX_KEY_SIZE	32  /* # of ASCII char's needed to
					represent a key */


/*  The structure for key information */
typedef struct {
    BYTE  direction;                /* Key used for encrypting or decrypting? */
    char  keyMaterial[MAX_KEY_SIZE+1];  /* Raw key data in ASCII, e.g., user input or KAT values */
	u32   rk[4*(10 + 1)];        /* key schedule */
} keyInstance;


class CDecryptDlg;

class API
{
public:
	Algorithm algo;

	int AES_Decrypt(unsigned char *key, unsigned char *data, int size);
	int AES_Encrypt(unsigned char *key, unsigned char *data, int size);
	int GenerateEncKey(unsigned char *X, unsigned char *Y, unsigned char *Ek);
	int CreateKeyPair(BYTE* PrivateKey, BYTE* PublicKey, BYTE *sz_eTerAuthKey);

//	BOOL CertificationPKS(unsigned char *key, char *sz_eTerPublicket, unsigned char *sz_eSignatureSize, long l_signsize);
	
	int		m_nKcIndex;
//	int		GenerateEncKey(unsigned char *X, unsigned char *Y, unsigned char *Ek); // encKey를 생성한다.
	string MakeKc(long Init);
	string MakeRn(long Init);
	string EncryptKc(string inKc);
	void DecryptKc(string inHexKc, char *reKc);
	string DecryptKc(string inHexKc);
	string EncryptKc(char *inKc);

	string DecryptIdRn(string inHexId);
	string EncryptIdRn(string inID);


	unsigned long Hex2ULong(char* szHex);
	int Hex2Bin(unsigned char *bin, char *hex, int binlength);
	int Char2Int(char ch);
	void SetKey16(char *key);
	void SetKey32(char *key);
	void SetKeystring(string key);


	int Decrypt(char *eblock, char *block, int blockcount);
	int Decrypt(char *eblock, char *block);

	int Encrypt(char *block, char *eblock, int blockcount);
	int Encrypt(char *block, char *eblock);
	

	void BinToHex(BYTE *hexBlock, const BYTE *binBlock, int blockLen);
	int makeKey(keyInstance *key, BYTE, char *keyMaterial);

	API();
	virtual ~API();

private:
	BYTE userKeyInHexa[33];
};

#endif // !defined(AFX_API_H__1FF35BC2_079D_11D5_BD32_0040D013BC1D__INCLUDED_)
