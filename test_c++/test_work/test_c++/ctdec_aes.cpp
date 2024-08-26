#include "ctdec_aes.h"


void CTAGENT_OLD_CTAES_Decrypt_Onetime_16Byte(char *input, char *output, unsigned int nSize, char *kc)
{
	keyInstAES keyInst;
	unsigned int i, pos;
	unsigned int nBlock;
	//#ifdef CTCOM_CPU_ARM
	//	char in[16], out[16];
	//#endif

	nBlock = nSize / 16;
	CTAGENT_OLD_CTAES_MakeKey_16Byte(&keyInst, kc, DIR_DECRYPT);

	for (i = 0; i < nBlock; i++)
	{
		//#ifdef CTCOM_CPU_ARM
		//		memcpy(in, input + (i*16), 16);
		//		CTAGENT_OLD_CTAES_DoDecrypt((u32 *)keyInst.rk, (unsigned char*)(in),(unsigned char*)(out));
		//		memcpy(output + (i*16), out, 16);
		//#else
		CTAGENT_OLD_CTAES_DoDecrypt((u32 *)keyInst.rk, (unsigned char*)(input + (i * 16)), (unsigned char*)(output + (i * 16)));
		//#endif
	}
	if (nSize % 16 != 0)
	{
		pos = nSize - (nSize % 16);
		memcpy(output + pos, input + pos, nSize % 16);
	}
	return;
}