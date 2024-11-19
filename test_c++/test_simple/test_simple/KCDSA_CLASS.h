#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "def.h"


#pragma once

#undef BIG_ENDIAN
#undef LITTLE_ENDIAN

#define BIG_ENDIAN		//	Big-Endian machine with pointer casting
//#define LITTLE_ENDIAN
#define DIGITSIZE			4
#define BitsInDIGIT			(8*DIGITSIZE)
#define CheckBitDIGIT(A, k)	CHECK_BIT_D(A, k)
#define SetBitDIGIT(A, k)	SET_BIT_D(A, k)

#define isEven0(A)		( ( (A)[0]&1 )==0 )
#define isOdd0(A)		( ( (A)[0]&1 )==1 )

#define BN_MAX_BITS		2048			//	Long #의 최대 비트수 : 512/768/1024/2048
#define MaxDIGIT		((BN_MAX_BITS-1)/BitsInDIGIT+1)	//	Long #의 최대 자리수
#define MAX_SEED_LEN	(512/8)			//	in BYTEs

////////	bit control macros	////////
//	bit 반전
#define NOT(x)				(~(x))
//	check k-th bits of A (array of DWORD or QWORD)
#define CHECK_BIT_B(A, k)	( 1 & ( (A)[(k)>>3] >> ((k) & ( 8-1)) ) )
#define CHECK_BIT_D(A, k)	( 1 & ( (A)[(k)>>5] >> ((k) & (32-1)) ) )
//	set k-th bits of A (array of DWORD or QWORD)
#define SET_BIT_B(A, k)		(A)[(k)>>3] |= ((DWORD)1 << ((k) & ( 8-1)) )
#define SET_BIT_D(A, k)		(A)[(k)>>5] |= ((DWORD)1 << ((k) & (32-1)) )
//	clear k-th bits of A (array of DWORD or QWORD)
#define CLEAR_BIT_B(A, k)	(A)[(k)>>3] &= NOT((DWORD)1 << ((k) & ( 8-1)) )
#define CLEAR_BIT_D(A, k)	(A)[(k)>>5] &= NOT((DWORD)1 << ((k) & (32-1)) )
//	change k-th bits of A (array of DWORD or QWORD)
#define CHANGE_BIT_B(A, k)	(A)[(k)>>3] ^= ((DWORD)1 << ((k) & ( 8-1)) )
#define CHANGE_BIT_D(A, k)	(A)[(k)>>5] ^= ((DWORD)1 << ((k) & (32-1)) )

#define BOOL	int					//	1-bit data type
#define BYTE	unsigned char		//	unsigned 1-byte data type
#define WORD	unsigned short int	//	unsigned 2-bytes data type
//#define DWORD	unsigned int		//	unsigned 4-bytes data type


#if defined(_MSC_VER)
#define QWORD	unsigned _int64		//	unsigned 8-bytes data type
#else
#define QWORD	unsigned long long	//	unsigned 8-bytes data type
#endif

#define DIGIT				DWORD
#if defined(QWORD)
#define DOUBLE_DIGIT	QWORD
#endif
#define RET_VAL				DWORD			//	return values

////	BIG_NUM..
typedef struct _BIG_NUM
{
	DWORD	Length;		//	유효한 DIGIT의 수를 나타냄
	DWORD	Space;		//	MUST : MemoryLen>=DataLen+1
	DIGIT	*pData;		//	실제로 데이터가 저장된 주소
} BIG_NUM;

////////	rotate by using shift operations	////////
#if defined(_MSC_VER)
#define ROTL_DWORD(x, n) _lrotl((x), (n))
#define ROTR_DWORD(x, n) _lrotr((x), (n))
#else
#define ROTL_DWORD(x, n) ( (DWORD)((x) << (n)) | (DWORD)((x) >> (32-(n))) )
#define ROTR_DWORD(x, n) ( (DWORD)((x) >> (n)) | (DWORD)((x) << (32-(n))) )
#endif

////////	reverse the byte order of DWORD(DWORD:4-bytes integer) and WORD.
#define ENDIAN_REVERSE_DWORD(dwS)	( (ROTL_DWORD((dwS),  8) & 0x00ff00ff)	\
									 | (ROTL_DWORD((dwS), 24) & 0xff00ff00) )

////////	move DWORD type to BYTE type and BYTE type to DWORD type
#if defined(BIG_ENDIAN)		////	Big-Endian machine
#define BIG_B2D(B, D)		D = *(DWORD *)(B)
#define BIG_D2B(D, B)		*(DWORD *)(B) = (DWORD)(D)
#define LITTLE_B2D(B, D)	D = ENDIAN_REVERSE_DWORD(*(DWORD *)(B))
#define LITTLE_D2B(D, B)	*(DWORD *)(B) = ENDIAN_REVERSE_DWORD(D)
#elif defined(LITTLE_ENDIAN)	////	Little-Endian machine
#define BIG_B2D(B, D)		D = ENDIAN_REVERSE_DWORD(*(DWORD *)(B))
#define BIG_D2B(D, B)		*(DWORD *)(B) = ENDIAN_REVERSE_DWORD(D)
#define LITTLE_B2D(B, D)	D = *(DWORD *)(B)
#define LITTLE_D2B(D, B)	*(DWORD *)(B) = (DWORD)(D)
#else
#error ERROR : Invalid DataChangeType
#endif

////	HAS160에 관련된 상수들
#define HAS160_DIGEST_BLOCKLEN	64		//	in bytes
#define HAS160_DIGEST_VALUELEN	20		//	in bytes

////	HAS160..
typedef struct _HAS160_ALG_INFO
{
	DWORD		ChainVar[HAS160_DIGEST_VALUELEN / 4];	//	Chaining Variable 저장
	DWORD		Count[4];							//	
	BYTE		Buffer[HAS160_DIGEST_BLOCKLEN];		//	Buffer for unfilled block
} HAS160_ALG_INFO;

////	Error Code - 정리하고, 적당히 출력해야 함.
#define CTR_SUCCESS					0
#define CTR_VERIFY_FAIL				1
#define CTR_FATAL_ERROR				2
#define CTR_INVALID_POINTER			3
#define CTR_INVALID_ALG_PARAMS		4
#define CTR_MEMORY_ALLOC_ERROR		5
#define CTR_BUFFER_TOO_SMALL		6
#define CTR_INVALID_DATA_LEN		7
#define CTR_INVALID_SIGNATURE_LEN	8

#define ERROR_MemLen1				21	//	input : DataLen<=MemoryLen
#define ERROR_MemLen2				22	//	output : DataLen<=MemoryLen
#define ERROR_OverModulus			23	//	modulus oper. : NO input > modulus
#define CTR_BN_NEGATIVE_RESULT		24	//	음수는 지원하지 않음
#define KI_DOMAIN_PARAMS	1
#define KI_PRIVATE_KEY		2
#define KI_PUBLIC_KEY		3

typedef struct _BN_KCDSA_KEY
{
	BIG_NUM 		*KCDSA_p;		//	prime(1024 + 128i bits i=0..8)
	BIG_NUM 		*KCDSA_q;		//	subprime(128 + 32j bits j=0..4)
	BIG_NUM 		*KCDSA_g;		//	Base
	BIG_NUM 		*KCDSA_x;		//	private key
	BIG_NUM 		*KCDSA_y;		//	public key
	BYTE			*KCDSA_z;		//	= Y mod 2^512 :: 64bytes
	DWORD			Count;			//	Prime Type ID
	DWORD			SeedLen;		//	in BYTEs
	BYTE			*Seed;			//	
} BN_KCDSA_KEY;

#define CTR_KEYFILE_ERROR		0x2002

extern BIG_NUM	BN_Zero, BN_One, BN_Two;

#define Max_W_size		6
#define PP_W(x) Window_PreData[x]


//--------------------------------------------------------------------------//
//	bignum.c	kcdsa.c	ct_kcdsa.c	has160.c 통합 클래스					//
//	실제 저 파일은 ../algorithm_ 폴더에 있음
//--------------------------------------------------------------------------//

class KCDSA_CLASS
{
public:
	BOOL CertificationPKS(unsigned char *key, char *sz_eTerPublicket, unsigned char *sz_eSignature, long l_signsize);

	KCDSA_CLASS();
	virtual ~KCDSA_CLASS();

	DIGIT	Window_PreData2_CHK[2][MaxDIGIT];
	int		Add_Chain[BN_MAX_BITS / Max_W_size][2];
	DIGIT	Window_PreData[1 << (Max_W_size - 1)][MaxDIGIT + 1];
	DIGIT Montgo_Inv;					//	<- L_Modulus[0]^{-1} mod (2^BitsInDIGIT)
	DIGIT Montgo_Rto2modN[MaxDIGIT + 2];	//	<- R^2 mod L_Modulus[]

	void D_Mul_D(DIGIT *D_Res, DIGIT D_Multiplicand, DIGIT D_Multiplier);
	int bn_Cmp(DIGIT *L_Src1, DWORD SrcLen1, DIGIT *L_Src2, DWORD SrcLen2);
	void bn_Copy(DIGIT *L_Dst, DIGIT *L_Src, DWORD SrcLen);
	DIGIT bn_Sub(DIGIT *L_Dst, DIGIT *L_Src1, DWORD SrcLen1, DIGIT *L_Src2, DWORD SrcLen2);
	DIGIT bn_MulAdd(DIGIT *L_Dst, DWORD DstLen, DIGIT *L_Src, DWORD SrcLen, DIGIT D_Multiplier);

	DIGIT DD_Div_D(DIGIT D_Dividend1, DIGIT D_Dividend2, DIGIT D_Divisor);
	DIGIT DD_Mod_D(DIGIT D_Dividend1, DIGIT D_Dividend2, DIGIT D_Divisor);
	DIGIT D_Gcd(DIGIT D_Src1, DIGIT D_Src2);
	DIGIT D_Inv(DIGIT D_Src);
	DIGIT bn_ModD(DIGIT *L_Src, DWORD SrcLen, DIGIT D_Divisor);
	DIGIT bn_SHL(DIGIT *L_Dst, DIGIT *L_Src, DIGIT SrcLen, DIGIT NumOfShift);
	DIGIT bn_SHR(DIGIT *L_Dst, DIGIT *L_Src, DIGIT SrcLen, DIGIT NumOfShift);
	DIGIT bn_Add(DIGIT *L_Dst, DIGIT *L_Src1, DWORD SrcLen1, DIGIT *L_Src2, DWORD SrcLen2);
	DIGIT bn_MulD(DIGIT *L_Dst, DIGIT *L_Src, DIGIT SrcLen, DIGIT D_Multiplier);
	DIGIT bn_MulSub(DIGIT *L_Dst, DWORD DstLen, DIGIT *L_Src, DWORD SrcLen, DIGIT D_Multiplier);
	void bn_Sqr(DIGIT *L_Dst, DIGIT *L_Src, DWORD SrcLen);
	void bn_Mul(DIGIT *L_Dst, DIGIT *L_Src1, DWORD SrcLen1, DIGIT *L_Src2, DWORD SrcLen2);
	void bn_KaraSqr(DIGIT *L_Dst, DIGIT *L_Src, DWORD SrcLen);
	void	bn_KaraMul(DIGIT *L_Dst, DIGIT *L_Src1, DIGIT *L_Src2, DWORD SrcLen);
	DIGIT bn_Div(DIGIT *L_Dst, DIGIT *L_Rem, DIGIT *L_Src1, DWORD SrcLen1, DIGIT *L_Src2, DWORD SrcLen2);
	RET_VAL bn_Euclid(DIGIT *L_Dst, DIGIT *L_Src, DIGIT *L_Modulus, DWORD ModLen);

	DWORD MakeAddChain(
		int		AddChain[][2],	//	결과를 넣을 메무리
		int		WindowSize,		//	
		DIGIT	*L_Exponent,	//
		DWORD	msb,			//
		BOOL	Type);			//

	RET_VAL bn_ModExp(DIGIT *L_Dst, DIGIT *L_Base, DWORD BaseLen,
		DIGIT *L_Exponent, DWORD ExpLen,
		DIGIT *L_Modulus, DWORD ModLen);

	void HAS160_Transform(DWORD *Y, DWORD *DigestValue);

	RET_VAL KCDSA_PRNG(
		HAS160_ALG_INFO	*HAS160_AlgInfo,
		BYTE		*pbSrc,
		DWORD		dSrcByteLen,	//	in Bytes
		BYTE		*pbDst,
		DWORD		dDstBitLen);		//	in Bits



	//	Create "BIG_NUM" data and return the pointer
	BIG_NUM	*CreateBigNum(
		DWORD		dMemoryLen);	//	in DIGITs

//	Destroy "BIG_NUM" data
	void	DestroyBigNum(
		BIG_NUM		*BN_Src);		//	pointer of BIG_NUM to be destroyed

//
	RET_VAL	BN2OS(
		BIG_NUM	*BN_Src,	//	Source integer
		DWORD	dDstLen,	//	Destination Length in BYTEs
		BYTE	*pbDst);	//	Destination Octet string pointer
	RET_VAL	OS2BN(
		BYTE	*pbSrc,		//	Source Octet string pointer
		DWORD	dSrcLen,	//	Source Length in BYTEs
		BIG_NUM	*BN_Dst);	//	Destination DIGIT array pointer

//	Long Number Copy : BN_Dst <- BN_Src
	RET_VAL BN_Copy(BIG_NUM *BN_Dst, BIG_NUM *BN_Src);

	//	Long Number Compare : return the sign of (BN_Src1 - BN_Src2)
	int BN_Cmp(BIG_NUM *BN_Src1, BIG_NUM *BN_Src2);

	//	Long Random Number : BN_Dst <- 'BitLen'-digit random long number
	RET_VAL BN_Rand(BIG_NUM *BN_Dst, DWORD BitLen);

	//########################################
	//	BIG_NUM의 핵심연산 함수
	//########################################

	//	Long Number (NumOfShift)-bits shift left : BN_Dst = BN_Src << NumOfShift
	RET_VAL BN_SHL(BIG_NUM *BN_Dst, BIG_NUM *BN_Src, DWORD NumOfShift);
	//	Long Number (NumOfShift)-bits shift right : BN_Dst = BN_Src >> NumOfShift
	RET_VAL BN_SHR(BIG_NUM *BN_Dst, BIG_NUM *BN_Src, DWORD NumOfShift);

	//	Long Nymber Addition : BN_Dst <- BN_Src1 + BN_Src2
	RET_VAL BN_Add(BIG_NUM *BN_Dst, BIG_NUM *BN_Src1, BIG_NUM *BN_Src2);
	//	Long Number Subtraction : BN_Dst <- BN_Src1 - BN_Src2
	RET_VAL BN_Sub(BIG_NUM *BN_Dst, BIG_NUM *BN_Src1, BIG_NUM *BN_Src2);

	//	Long Number Multiple : BN_Dst <- BN_Src1 * BN_Src2
	//			the most general multiple function
	RET_VAL BN_Mul(BIG_NUM *BN_Dst, BIG_NUM *BN_Src1, BIG_NUM *BN_Src2);

	//	Long Number Division : BN_Res <- BN_Dividend div BN_Divisor
	RET_VAL BN_Div(BIG_NUM *BN_Quotient, BIG_NUM *BN_Remainder,
		BIG_NUM *BN_Dividend, BIG_NUM *BN_Divisor);

	//########################################
	//	BIG_NUM의 Modulus 연산 함수
	//########################################

	//	Long Number Modular addtion :
	//				BN_Dst <- BN_Src1 + BN_Src2 mod BN_Modulus
	RET_VAL BN_ModAdd(BIG_NUM *BN_Dst, BIG_NUM *BN_Src1, BIG_NUM *BN_Src2,
		BIG_NUM *BN_Modulus);
	//	Long Number Modular subtraction :
	//				BN_Dst <- BN_Src1 - BN_Src2 mod BN_Modulus
	RET_VAL BN_ModSub(BIG_NUM *BN_Dst, BIG_NUM *BN_Src1, BIG_NUM *BN_Src2,
		BIG_NUM *BN_Modulus);

	//	Long Number Modular Reduction - Classical Algorithm
	RET_VAL BN_ModRed(BIG_NUM *BN_Dst, BIG_NUM *BN_Src, BIG_NUM *BN_Modulus);
	//	Long Number Modular Multiple - Classical Algorithm
	RET_VAL BN_ModMul(BIG_NUM *BN_Res, BIG_NUM *BN_Src1, BIG_NUM *BN_Src2,
		BIG_NUM *BN_Modulus);

	//	Extended Euclid Algorithm
	//		return CTR_SUCCESS	if gcd(BN_Src,BN_Modulus)==1 :
	//					BN_Dst <- BN_Src^-1 mod BN_Modulus
	//		return !CTR_SUCCESS if gcd(BN_Src,BN_Modulus)!=1 :
	//					BN_Dst <- gcd(BN_Src, BN_Modulus)
	RET_VAL BN_ModInv(BIG_NUM *BN_Dst, BIG_NUM *BN_Src, BIG_NUM *BN_Modulus);

	//	Long Number Modular Exponential Algorithm - 
	//		Window Algorithm and Montgomery Reduction Algorithm
	RET_VAL BN_ModExp(BIG_NUM *BN_Dst, BIG_NUM *BN_Base, BIG_NUM *BN_Exponent,
		BIG_NUM *BN_Modulus);

	/*************** Prototypes ***********************************************/
	RET_VAL Classical_REDC(DIGIT *L_Dst, DWORD DstLen,
		DIGIT *L_Modulus, DWORD ModLen);
	RET_VAL Montgomery_Init(DIGIT *L_Modulus, DWORD ModLen);
	//	RET_VAL Montgomery_REDC(DIGIT *L_Dst, DWORD DstLen,
	//							DIGIT *L_Modulus, DWORD ModLen);
	RET_VAL Montgomery_Zn2RZn(DIGIT *L_Dst, DIGIT *L_Src,
		DIGIT *L_Modulus, DWORD ModLen);


	//########################################
	//	기타 보조 연산 함수
	//########################################

	//
	RET_VAL MillerRabin(
		BIG_NUM		*BN_Num);

	void	HAS160_Init(
		HAS160_ALG_INFO	*AlgInfo);
	void	HAS160_Update(
		HAS160_ALG_INFO	*AlgInfo,
		BYTE			*Message,		//	데이타가 입력됨
		DWORD			MessageLen);
	void	HAS160_Final(
		HAS160_ALG_INFO	*AlgInfo,
		BYTE			*Digest);		//	해쉬값이 출력됨

/*************** Prototypes ***********************************************/
////	BN_KCDSA_KEY 형식의 데이타를 파일에 쓰거나 읽어들이는 함수
	RET_VAL KCDSA_GetKeyData(
		char			*filename,		//	파라미터/개인키/공개키가 있는 파일명
		DWORD			KeyType,		//	KI_???
		BN_KCDSA_KEY	*KCDSA_Params);
	RET_VAL KCDSA_SaveKeyData(
		char			*filename,
		DWORD			KeyType,
		BN_KCDSA_KEY	*KCDSA_Params);

	////	BN_KCDSA_KEY 형식의 데이타의 메모리를 할당하거나 소거하는 함수
	RET_VAL KCDSA_CreateKeyObject(
		BN_KCDSA_KEY	**KCDSA_Params);//	출력

	RET_VAL KCDSA_DestroyKeyObject(
		BN_KCDSA_KEY	**KCDSA_Params);//	출력

////	KCDSA용 도메인 파라미터와 키쌍을 생성하는 함수
	RET_VAL KCDSA_GenerateParameters(
		DWORD			PrimeBits,
		DWORD			SubPrimeBits,
		BN_KCDSA_KEY	*KCDSA_Params);	//	출력
	RET_VAL KCDSA_GenerateKeyPair(
		BN_KCDSA_KEY	*KCDSA_Key);	//	Generated Private/Public Key

////	KCDSA 서명 생성 및 서명 검증
	RET_VAL KCDSA_Sign(
		BN_KCDSA_KEY	*KCDSA_PrivKey,	//	서명키
		BYTE			*MsgDigest,		//	해쉬값 Hash(z||M) 입력
		DWORD			MsgDigestLen,
		BYTE			*Signature, 	//	서명값 출력
		DWORD			*SignLen);

	RET_VAL KCDSA_Verify(
		BN_KCDSA_KEY	*KCDSA_PubKey,	//	공개키
		BYTE			*MsgDigest, 	//	해쉬값 Hash(M) 입력
		DWORD			MsgDigestLen,
		BYTE			*Signature, 	//	서명값 입력
		DWORD			SignLen);

	RET_VAL Montgomery_REDC(DIGIT *L_Dst, DWORD DstLen,
		DIGIT *L_Modulus, DWORD ModLen);


	// FROM ct_kcdsa.c
	/* key 생성 및 파괴 */
	RET_VAL CT_KCDSA_CreateKeyObject(BN_KCDSA_KEY **KCDSA_Params);
	RET_VAL CT_KCDSA_DestroyKeyObject(BN_KCDSA_KEY **KCDSA_Params);

	/* key pair 생성 */
	RET_VAL CT_KCDSA_GenerateKeyPair(BN_KCDSA_KEY *KCDSA_Key);

	/* sign */
	RET_VAL CT_KCDSA_Sign(BN_KCDSA_KEY *KCDSA_PrivKey,	// private key
		BYTE *pbSrcData,				// sign할 데이터
		int srcDataLen,				// sign할 데이터 길이
		BYTE *pbSign,					// sign 저장 버퍼
		unsigned long *signLen);				// sign 길이 반환

/* verify */
	RET_VAL CT_KCDSA_Verify(BN_KCDSA_KEY *KCDSA_PubKey, // public key
		BYTE *pbSrcData,			// sign을 검증할 데이터
		int srcDataLen,				// sign을 검증할 데이터 길이 
		BYTE *pbSign,				// sign 값
		int signLen);				// sign 길이

/* hash */
	void CT_HAS160_Hash(BYTE *pbSrcData,
		int srcDataLen,
		BYTE *pbHashValue,
		int *hashValueLen);

	/* private key를 Hex로 변환시 길이 구한다. */
	int CT_KCDSA_GetPrivKeyStringLen(BN_KCDSA_KEY *KCDSA_PrivKey);

	/* public key를 Hex로 변환시 길이 구한다. */
	int CT_KCDSA_GetPubKeyStringLen(BN_KCDSA_KEY *KCDSA_PubKey);

	/* private key, public key를 Hex 문자열로 변환한다 */
	int CT_KCDSA_PrivKeyToHex(BN_KCDSA_KEY *KCDSA_PrivKey, BYTE *hexPrivKey);
	int CT_KCDSA_PubKeyToHex(BN_KCDSA_KEY *KCDSA_PubKey, BYTE *hexPubKey);

	/* Hex 문자열을 private key, public key로 변환한다. */
	int CT_KCDSA_HexToPrivKey(BN_KCDSA_KEY *KCDSA_PrivKey, char *hexPrivKey);
	int CT_KCDSA_HexToPubKey(BN_KCDSA_KEY *KCDSA_PubKey, char *hexPubKey);
	void CT_KCDSA_HexToBin(unsigned char *bin, const char *hex, int binLength);
	void CT_KCDSA_GetDomainParameter(BN_KCDSA_KEY *KCDSA_Key);
	void CT_KCDSA_BinToHex(char *hex, const unsigned char *bin, int binLength);
	int CT_KCDSA_CharToInt(char ch);

};
