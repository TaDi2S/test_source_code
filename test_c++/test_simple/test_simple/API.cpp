// API.cpp: implementation of the API class.

#include "Algorithm.h"
#include "API.h"
#include <time.h>

//#include "algorithm/aes.h"
//#include "algorithm/kcdsa.h"
//#include "algorithm/ct_kcdsa.h"

API::API()
{
	m_nKcIndex = 0;
}

API::~API()
{

}


//--------------  PK 관련 암복호화 
int API::AES_Decrypt(unsigned char *key, unsigned char *data, int size)
{
	u32 rk[44];
	int i;
	unsigned char buff[16];
	size >>= 4;

	algo.rijndaelKeySetupDec(rk, key);
	for (i = 0; i < size; i++)
	{
		algo.rijndaelDecrypt(rk, data, buff);
		memcpy(data, buff, 16);
		data += 16;
	}
	return i;
}

int API::AES_Encrypt(unsigned char *key, unsigned char *data, int size)
{
	u32 rk[44];
	int i;
	unsigned char buff[16];
	size >>= 4;

	algo.rijndaelKeySetupEnc(rk, key);
	for (i = 0; i < size; i++)
	{
		algo.rijndaelEncrypt(rk, data, buff);
		memcpy(data, buff, 16);
		data += 16;
	}
	return i;
}


int API::GenerateEncKey(unsigned char *X, unsigned char *Y, unsigned char *Ek)
{
	unsigned char k[16];
	int i;

	/// Make B
	memcpy(k, X, 8);
	memcpy(k + 8, Y, 8);

	memcpy(Ek, X + 8, 8);
	memcpy(Ek + 8, Y + 8, 8);

	for (i = 8; i < 16; i++)
	{
		k[i] ^= 0xCA;
	}

	/// Make K
	AES_Encrypt(k, Ek, 16);

	return 1;
}


int API::makeKey(keyInstance *key, BYTE direction, char *keyMaterial) {
	int i;
	char *keyMat;
	u8 cipherKey[MAXKB];

	key->direction = direction;
	strncpy(key->keyMaterial, keyMaterial, 32);

	/* initialize key schedule: */
	keyMat = key->keyMaterial;
	for (i = 0; i < 16; i++) {
		int t, v;

		t = *keyMat++;
		if ((t >= '0') && (t <= '9')) v = (t - '0') << 4;
		else if ((t >= 'a') && (t <= 'f')) v = (t - 'a' + 10) << 4;
		else if ((t >= 'A') && (t <= 'F')) v = (t - 'A' + 10) << 4;
		else return BAD_KEY_MAT;

		t = *keyMat++;
		if ((t >= '0') && (t <= '9')) v ^= (t - '0');
		else if ((t >= 'a') && (t <= 'f')) v ^= (t - 'a' + 10);
		else if ((t >= 'A') && (t <= 'F')) v ^= (t - 'A' + 10);
		else return BAD_KEY_MAT;

		cipherKey[i] = (u8)v;
	}
	if (direction == DIR_ENCRYPT) {
		algo.rijndaelKeySetupEnc(key->rk, cipherKey);
	}
	else {
		algo.rijndaelKeySetupDec(key->rk, cipherKey);
	}
	return TRUE;
}

// Encryption from ibuffer to obuffer of 128 bits
// by calling algo.RihndaelEncrypt

// Encryption from iblock to oblock of 128 bits
// Use userKeyInHexa Private member
void API::BinToHex(BYTE *hexBlock, const BYTE *binBlock, int blockLen)
{
	int i;

	for (i = 0; i < blockLen; i++) {
		sprintf((char*)hexBlock + 2 * i, "%02X", (unsigned char)binBlock[i]);
	}
}

int API::Encrypt(char *Block, char *eBlock)
{
	int r;
	keyInstance keyInst;

	r = makeKey(&keyInst, DIR_ENCRYPT, (char*)userKeyInHexa);
	algo.rijndaelEncrypt(keyInst.rk, (unsigned char*)Block, (unsigned char*)eBlock);

	return 1;
}



int API::Decrypt(char *eBlock, char *Block)
{

	int r;
	keyInstance keyInst;

	r = makeKey(&keyInst, DIR_DECRYPT, (char*)userKeyInHexa);
	algo.rijndaelDecrypt(keyInst.rk, (unsigned char*)eBlock, (unsigned char*)Block);

	return 1;
}

int API::Encrypt(char *Block, char *eBlock, int blockcount)
{
	int r;
	keyInstance keyInst;

	r = makeKey(&keyInst, DIR_ENCRYPT, (char*)userKeyInHexa);

	::ZeroMemory(eBlock, 16 * blockcount);

	for (int i = 0; i < blockcount; i++)
	{
		algo.rijndaelEncrypt(keyInst.rk, (unsigned char*)(Block + (i * 16)), (unsigned char*)(eBlock + (i * 16)));
	}

	return 1;
}


int API::Decrypt(char *Block, char *eBlock, int blockcount)
{

	int r;
	keyInstance keyInst;

	r = makeKey(&keyInst, DIR_DECRYPT, (char*)userKeyInHexa);

	for (int i = 0; i < blockcount; i++)
	{
		algo.rijndaelDecrypt(keyInst.rk, (unsigned char*)(Block + (i * 16)), (unsigned char*)(eBlock + (i * 16)));
	}

	return 1;
}


void API::SetKey16(char *key)
{
	BinToHex(userKeyInHexa, (unsigned char*)key, 16);
}

void API::SetKey32(char *key)
{
	memcpy(userKeyInHexa, key, 32);
}

void API::SetKeyCString(CString key)
{
	int length;
	char charKey[17];
	CString temp;
	CString emptyfill = "www.coretrust.com";

	length = key.GetLength();

	if (length < 16)
	{
		temp = key + emptyfill;
	}
	else
	{
		temp = key;
	}

	temp = temp.Left(16);

	sprintf(charKey, "%s", temp);
	BinToHex(userKeyInHexa, (unsigned char*)charKey, 16);
	userKeyInHexa[32] = '\0';

}



int API::Char2Int(char ch)
{
	switch (ch) {
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	case 'a':
	case 'A':
		return 10;
	case 'b':
	case 'B':
		return 11;
	case 'c':
	case 'C':
		return 12;
	case 'd':
	case 'D':
		return 13;
	case 'e':
	case 'E':
		return 14;
	case 'f':
	case 'F':
		return 15;
	default:
		return 0;
	}
}


int API::Hex2Bin(unsigned char *bin, char *hex, int binlength)
{
	for (int i = 0; i < binlength; i++)
		bin[i] = 16 * Char2Int(hex[2 * i]) + Char2Int(hex[2 * i + 1]);

	return 0;
}
/*
unsigned long API::Hex2ULong(char* szHex) {
	unsigned long hex = 0; // 반환될 값. 초기에는 0이다.
	int nibble;  // 16진수의 한 니블(4비트)값을 담아둘 곳

	while (*szHex) {
		hex <<= 4;

		if (*szHex >= '0' && *szHex <= '9') {
			nibble = *szHex - '0';
		}
		else if (*szHex >= 'a' && *szHex <= 'f') {
			nibble = *szHex - 'a' + 10;
		}
		else if (*szHex >= 'A' && *szHex <= 'F') {
			nibble = *szHex - 'A' + 10;
		}
		else {
			nibble = 0;
		}

		hex |= nibble;

		szHex++;
	}

	return hex;
}
*/
/**************************************************************************************
 *		KC값을 받아서 암호된 Hex CString으로 리턴한다.
 **************************************************************************************/
CString API::EncryptKc(char *inKc)
{

	char chEkcdata[33];
	unsigned char	chEhexKc[65];
	CString strRetKc;

	Encrypt(inKc, chEkcdata, 2);
	chEkcdata[32] = '\0';

	BinToHex((BYTE*)chEhexKc, (BYTE*)chEkcdata, 32);
	chEhexKc[64] = '\0';
	strRetKc.Format("%s", chEhexKc);

	return strRetKc;
}


/**************************************************************************************
 *		Id값을 받아서 암호된 Hex CString으로 리턴한다.
 **************************************************************************************/
void API::DecryptKc(CString inHexKc, char *reKc)
{
	char	chEhexKc[65];
	char	chBinKc[33];
	sprintf(chEhexKc, "%s", inHexKc);
	Hex2Bin((unsigned char*)chBinKc, chEhexKc, 32);
	Decrypt(chBinKc, reKc, 2);
	reKc[32] = '\0';
}


/**************************************************************************************
 *		Id값을 받아서 암호된 Hex CString으로 리턴한다.
 **************************************************************************************/
CString API::DecryptKc(CString inHexKc)
{
	char	chEhexKc[65];
	char	chBinKc[33];
	char	reKc[33];
	CString strReKc;

	sprintf(chEhexKc, "%s", inHexKc);
	Hex2Bin((unsigned char*)chBinKc, chEhexKc, 32);
	Decrypt(chBinKc, reKc, 2);
	reKc[32] = '\0';

	strReKc.Format("%s", reKc);

	return strReKc;
}


/**************************************************************************************
 *		암호화된 Hex CString문자열을 받아서  원래 ID로 변환 한다.
 **************************************************************************************/
CString API::EncryptIdRn(CString inID)
{

	char chEIddata[17];
	char chId[17];
	unsigned char	chEhexId[33];
	CString strRetId;

	inID.Format("%swiziwiziwiziwizi", inID);
	sprintf(chId, "%s", inID.Left(16));
	Encrypt(chId, chEIddata);
	chEIddata[16] = '\0';

	BinToHex((BYTE*)chEhexId, (BYTE*)chEIddata, 16);
	chEhexId[32] = '\0';
	strRetId.Format("%s", chEhexId);

	return strRetId;
}


/**************************************************************************************
 *		암호화된 Hex CString문자열을 받아서  원래 ID로 변환 한다.
 **************************************************************************************/
CString API::DecryptIdRn(CString inHexId)
{
	char	chEhexId[33];
	char	chBinId[16];
	char	reId[17];
	CString	strReId;

	sprintf(chEhexId, "%s", inHexId);
	Hex2Bin((unsigned char*)chBinId, chEhexId, 16);
	Decrypt(chBinId, reId, 2);
	reId[16] = '\0';
	strReId.Format("%s", reId);

	return strReId;
}

CString API::EncryptKc(CString inKc)
{
	char chEkcdata[33];
	char chKc[33];
	unsigned char	chEhexKc[65];
	CString strRetKc;

	sprintf(chKc, "%s", inKc);
	Encrypt(chKc, chEkcdata, 2);
	chEkcdata[32] = '\0';

	BinToHex((BYTE*)chEhexKc, (BYTE*)chEkcdata, 32);
	chEhexKc[64] = '\0';
	strRetKc.Format("%s", chEhexKc);

	return strRetKc;

}

CString API::MakeRn(long Init)
{
	int temp1;
	CString Rn = "";
	CString temp;

	m_nKcIndex++;

	srand((unsigned)time(NULL) + Init + m_nKcIndex);
	for (int i = 0; i < 16; i++)
	{
		temp1 = rand() % 10;
		temp.Format("%d", temp1);
		Rn = Rn + temp;
	}
	return Rn;
}

CString API::MakeKc(long Init)
{
	int temp1;
	CString Rn = "";
	CString temp;
	m_nKcIndex++;

	srand((unsigned)time(NULL) + Init + m_nKcIndex);
	for (int i = 0; i < 32; i++)
	{
		temp1 = rand() % 10;
		temp.Format("%d", temp1);
		Rn = Rn + temp;
	}
	return Rn;
}


int API::CreateKeyPair(BYTE* PrivateKey, BYTE* PublicKey, BYTE *sz_eTerAuthKey)
{
	// Generate a new key pair for the terminal ID
	BN_KCDSA_KEY *KeyPair;

	KCDSA_CLASS *kc;
	kc = new KCDSA_CLASS;

	kc->CT_KCDSA_CreateKeyObject(&KeyPair);
	kc->CT_KCDSA_GenerateKeyPair(KeyPair);

	kc->CT_KCDSA_PrivKeyToHex(KeyPair, PrivateKey);
	kc->CT_KCDSA_PubKeyToHex(KeyPair, PublicKey);

	kc->CT_KCDSA_DestroyKeyObject(&KeyPair);

	sprintf((char *)sz_eTerAuthKey, "%s", MakeRn(0));

	delete kc;

	return 1;
}

/*
BOOL API::CertificationPKS(unsigned char *key, char *sz_eTerPublicket, unsigned char *sz_eSignatureSize, long l_signsize)
{
	BN_KCDSA_KEY *KeyObj;
	RET_VAL RetVal;
	CT_KCDSA_CreateKeyObject(&KeyObj);
	CT_KCDSA_HexToPubKey(KeyObj, (unsigned char*)sz_eTerPublicket);
	RetVal = CT_KCDSA_Verify(
		KeyObj,
		key,
		16,
		sz_eSignatureSize,
		l_signsize);
	CT_KCDSA_DestroyKeyObject(&KeyObj);

	if (RetVal == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
*/