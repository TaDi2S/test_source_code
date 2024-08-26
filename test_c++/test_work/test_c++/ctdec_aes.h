#pragma once
//#include "ctglo_def.h"
// #include "CARMManagerConstant.h"
//#include "ctdec.h"

#ifndef _CTDEC_AES_H_
#define _CTDEC_AES_H_

#include "CARMManagerConstant.h"

/*
extern const	u32 Te0[256];
extern const	u32 Te1[256];
extern const	u32 Te2[256];
extern const	u32 Te3[256];
extern const	u32 Te4[256];

extern const	u32 Td0[256];
extern const	u32 Td1[256];
extern const	u32 Td2[256];
extern const	u32 Td3[256];
extern const	u32 Td4[256];
extern const	u32 rcon[];
*/



#ifdef ALIGNED_BYTE_ACCESS
//2010-06-01
#define			GETU32(pt) (((u32)(pt)[0] << 24) ^ ((u32)(pt)[1] << 16) ^ ((u32)(pt)[2] <<  8) ^ ((u32)(pt)[3]))
#define			PUTU32(ct, st) { (ct)[0] = (u8)((st) >> 24); (ct)[1] = (u8)((st) >> 16); (ct)[2] = (u8)((st) >>  8); (ct)[3] = (u8)(st); }

#else

#define			tlrotl(x)			((unsigned int) ( (unsigned int)((unsigned int)0x00FFFFFF & (unsigned int)x) << 8 ) | ((unsigned int) (unsigned int) ((unsigned int)0xFF000000 & (unsigned int)x ) >> 24) )
#define			tlrotr(x)			((unsigned int) ( (unsigned int)((unsigned int)0xFFFFFF00 & (unsigned int)x) >> 8 ) | ((unsigned int) (unsigned int) ((unsigned int)0x000000FF & (unsigned int)x ) << 24) )
#define			SWAP(x)				((unsigned int)tlrotl(x) & 0x00ff00ff | (unsigned int)tlrotr(x) & 0xff00ff00)

#define			GETU32(p)			SWAP(*((u32 *)(p)))
#define			PUTU32(ct, st)		{ *((u32 *)(ct)) = SWAP((st)); }

#endif //ALIGNED_BYTE_ACCESS

#define AES_BLOCK_SIZE 16
#define			MAXKC				(128/32)
#define			MAXKB				(128/8)
#define			MAXNR				10

#define			TRUE				1
#define			FALSE				0
#define			BITSPERBLOCK		128	

//  Error Codes - CHANGE POSSIBLE: inclusion of additional error codes 
#define			BAD_KEY_DIR			-1  	//  Key direction is invalid, e.g., unknown value 
#define			BAD_KEY_MAT			-2  	//  Key material not of correct length 
#define			BAD_KEY_INSTANCE	-3  	//  Key passed is not valid 

#define			DIR_ENCRYPT			0		//  Are we encrpyting? 
#define			DIR_DECRYPT			1		//  Are we decrpyting?

#ifdef __cplusplus
extern "C" {
#endif
	void			CTAGENT_OLD_CTAES_SetKey(int CARMIndex, char *key, int nDirection);
	void			CTAGENT_OLD_CTAES_SetPackageKey(int CARMIdex, char *key, int nDirection);
	int				CTAGENT_OLD_CTAES_MakeKey(keyInstAES *key, char *keyMaterial, int nDirection);
	int				CTAGENT_OLD_CTAES_MakeKey_16Byte(keyInstAES *key, char *keyMaterial, int nDirection);
	void			CTAGENT_OLD_CTAES_SetKeyEnc(u32 rk[], const u8 cipherKey[]);
	void			CTAGENT_OLD_CTAES_SetKeyDec(u32 rk[], const u8 cipherKey[]);

	void			CTAGENT_OLD_CTAES_Encrypt_Onetime(char *input, char *output, unsigned int nSize, char *kc);
	void			CTAGENT_OLD_CTAES_Encrypt(int CARMIndex, int program_index, char *input, char *output, unsigned int size);
	void			CTAGENT_OLD_CTAES_DoEncrypt(const u32 rk[], const u8 pt[], u8 ct[]);
	void			CTAGENT_OLD_CTAES_DoEncrypt_cbc(u8 in[], u8 out[], unsigned int length, u32 key[], unsigned char *ivec);

	void			CTAGENT_OLD_CTAES_Decrypt_Onetime(char *input, char *output, unsigned int nSize, char *kc);
	void			CTAGENT_OLD_CTAES_Decrypt_Onetime_16Byte(char *input, char *output, unsigned int nSize, char *kc);
	void			CTAGENT_OLD_CTAES_Decrypt(int CARMIndex, int program_index, char *input, char *output, unsigned int size);
	void			CTAGENT_OLD_CTAES_Decrypt_Audio(int CARMIndex, int program_index, char *input, char *output, unsigned int size);
	void			CTAGENT_OLD_CTAES_DoDecrypt(const u32 rk[], const u8 pt[], u8 ct[]);
	void			CTAGENT_OLD_CTAES_Decrypt_PMT_Data(int CARMIndex, char *input, char *output, unsigned int size);
	void			CTAGENT_OLD_CTAES_Decrypt_SK_PMT_Data(int CARMIndex, char *input, char *output, unsigned int size);
	void			CTAGENT_OLD_CTAES_DoDecrypt_cbc(u8 in[], u8 out[], unsigned int length, u32 key[], unsigned char *ivec);

	void			CTAGENT_OLD_CTAES_DoEncrypt_ctr(u8 in[], u8 out[], u32 length, u32 key[], u8 iv[]);

#ifdef __cplusplus
}
#endif


#endif

