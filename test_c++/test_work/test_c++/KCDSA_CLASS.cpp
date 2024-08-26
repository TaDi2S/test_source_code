// KCDSA_CLASS.cpp: implementation of the KCDSA_CLASS class.
//
//////////////////////////////////////////////////////////////////////

#include "header.h"
#include "KCDSA_CLASS.h"
#include <assert.h>
#include <string.h>
#include <time.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define FirstWindowMayBeEven	0
#define FirstWindowMustBeOdd	1

#undef MyDebug
//#define MyDebug

#define Kara_Length			9632

#define GOTO_END		if( ret!=CTR_SUCCESS )	goto LABEL_END

#define CheckInput_MemLen(L)	if( L->Space<L->Length ) return ERROR_MemLen1
#define CheckOutput_MemLen(L)	if( L->Space<L->Length ) return ERROR_MemLen2
#define CheckMostSignificantDIGIT(BN_Num) {			\
	for(  ; BN_Num->Length>0; BN_Num->Length--)		\
		if( BN_Num->pData[BN_Num->Length-1]!=0 )	\
			break;									\
}

static DIGIT	SmallPrimes[ ]={
	0xC8E15F2A, 0x16FA4227, 0x87B81DA9, 0xDA38C071, 0xFDB17C23, 0xFE5E796B,
	0xC7E4CBF5, 0x7EB0F0B1, 0xB72EFC93, 0xF46CEE57, 0x80B2C2BB, 0x34A77199,
	0x447D1BD5, 0xEA4C7C31, 0xF046D45B, 0xFF55A7BF, 0x9B287041, 0x85663BEF,
	0x7856625B, 0,	/* 100-primes */
	0xF53CB8EF, 0x0BF8B47B, 0x302F3B45, 0xF7889105, 0xAEB9C343, 0xE4703BE3,
	0x7E15A86D, 0x8DFBFF6D, 0xE3FF5767, 0xF4DC76E3, 0xFFDEB1BB, 0xF1CCD229,
	0xAD97C169, 0x44655D23, 0xD39EFD0F, 0x39E3CD4D, 0xE049D915, 0xF9CD1761,
	0xF7B3D683, 0x5170C36F, 0xC22F6765, 0x81779DA7, 0x76EC6BF5, 0};

static DIGIT	IterNo[][2]={
	{ 100, 27},
	{ 150, 18},
	{ 200, 15},
	{ 250, 12},
	{ 500,  9},
	{ 500,  6},
	{ 600,  5},
	{ 800,  4},
	{1250,  3},
	{2048,  2},
	{9999,  1},
};

DWORD Kara_Sqr_Length = Kara_Length / 100;
DWORD Kara_Mul_Length = Kara_Length % 100;

//
DIGIT bn_Zero[2] = {0, 0};
DIGIT bn_One[2] = {1, 0};
DIGIT bn_Two[2] = {2, 0};
BIG_NUM BN_Zero={0, 2, bn_Zero},
		BN_One={1, 2, bn_One},
		BN_Two={1, 2, bn_Two};


#define F(x, y, z)  ( (z)^( (x) & ((y)^(z)) ) )
#define G(x, y, z)  ( (x) ^ (y) ^ (z) )
#define H(x, y, z)  ( (y)^( (x) | ~(z) ) )

//	step operations
#define FF(a, b, c, d, e, x, s) {					\
	(e) += ROTL_DWORD(a, s) + F(b, c, d) + (x);		\
	(b) = ROTL_DWORD(b, 10);						\
}
#define GG(a, b, c, d, e, x, s) {					\
	(e) += ROTL_DWORD(a, s) + G(b, c, d) + (x) + K;	\
	(b) = ROTL_DWORD(b, 17);						\
}
#define HH(a, b, c, d, e, x, s) {					\
	(e) += ROTL_DWORD(a, s) + H(b, c, d) + (x) + K;	\
	(b) = ROTL_DWORD(b, 25);						\
}
#define II(a, b, c, d, e, x, s) {					\
	(e) += ROTL_DWORD(a, s) + G(b, c, d) + (x) + K;	\
	(b) = ROTL_DWORD(b, 30);						\
}

#define DOMAIN_KCDSA_P_LEN		32
#define DOMAIN_KCDSA_Q_LEN		5
#define DOMAIN_KCDSA_G_LEN		32
#define DOMAIN_KCDSA_COUNT		47009
#define DOMAIN_KCDSA_SEED_LEN	20

static const DWORD DOMAIN_KCDSA_P[DOMAIN_KCDSA_P_LEN] =
{
	0x91CE5F76, 0xE443539C, 0x91893D96, 0x8BB2B2DB, 0x15B829C2, 
	0xDE91A27B, 0x9DC4B80D, 0xE7894C26, 0x463A4C48, 0xB7200437, 
	0x81EA08DA, 0x6825F82E, 0x5A17C56A, 0x8A059D80, 0x2EA1E3DF, 
	0xADC6379B, 0x2589A008, 0xD94867E3, 0x0B807EB1, 0x1BEBE002, 
	0xD18565AE, 0xD2EC01DC, 0x4CF1863B, 0x15317DFC, 0xB55C19D2, 
	0x78746549, 0xD3BEE240, 0x95524008, 0x643335D6, 0x2BDB3349, 
	0x208FD095, 0xBE3E1A27
};

static const DWORD DOMAIN_KCDSA_Q[DOMAIN_KCDSA_Q_LEN] =
{
	0x8F7603FF, 0x835B3EF7, 0x1B1833ED, 0x8B12F4F5, 0x6F5AE4C1
};

static const DWORD DOMAIN_KCDSA_G[DOMAIN_KCDSA_G_LEN] =
{
	0x24829FEF,	0x97B4F9E0, 0x400CEE8F, 0x201B8E64, 0xA2586C30,
	0xB7917CE5, 0xA13326A7, 0x7E57F048, 0xC61BB10D, 0x0BBC83F3,
	0x3225F577, 0xCE689A94, 0x512537DB, 0x5CE8E25A, 0xEC8078DC,
	0x9C72FD82, 0x4BBA10C1, 0x4E9C9F16, 0x93298B04, 0x8629627B,
	0x4211B561, 0x40F09E78, 0x2D5FB2BE, 0xF3E2A2D0, 0xB3F3FC6A,
	0xBC4273AC, 0x38861D62, 0x2AE16CD8, 0x2E1783A9, 0xF9551083,
	0xAC4FADB5, 0xA9F586F7
};

static const BYTE DOMAIN_KCDSA_SEED[DOMAIN_KCDSA_SEED_LEN] =
{
	0x9F, 0xB0, 0x2D, 0x04, 0x15, 0xF4, 0x83, 0xA6, 0x66, 0x30, 
	0x19, 0x58, 0x43, 0x9D, 0xFC, 0x7C, 0x31, 0xEC, 0x49, 0x2B
};


KCDSA_CLASS::KCDSA_CLASS()
{
	DIGIT	Window_PreData2_CHK[2][MaxDIGIT]={{0,0,0,0,0},{0,0,0,0,0}};
	unsigned long	l_SignatureSize = 0;
}

KCDSA_CLASS::~KCDSA_CLASS()
{
}

void KCDSA_CLASS::D_Mul_D(DIGIT *D_Res, DIGIT D_Multiplicand, DIGIT D_Multiplier) 
{
#if defined(DOUBLE_DIGIT)
	DOUBLE_DIGIT tmp;

	tmp = (DOUBLE_DIGIT)D_Multiplicand * D_Multiplier;
	D_Res[0] = (DIGIT) tmp;
	D_Res[1] = (DIGIT) (tmp>>BitsInDIGIT);
#else
	DIGIT	Da, Db, R1, R0;

	R1 = HL(D_Multiplicand) * HL(D_Multiplier);
	R0 = LL(D_Multiplicand) * LL(D_Multiplier);

	Da = HL(D_Multiplicand) * LL(D_Multiplier);
	Db = LL(D_Multiplicand) * HL(D_Multiplier);

	if( (Db+=Da)<Da )			R1 += ((DIGIT)1) << BIDdiv2;
	if( (R0+=LH(Db))<LH(Db) )	R1++;

	D_Res[0] = R0;
	D_Res[1] = R1 + HL(Db);
#endif
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
DIGIT KCDSA_CLASS::DD_Div_D(DIGIT D_Dividend1, DIGIT D_Dividend2, DIGIT D_Divisor)
{
#if defined(DOUBLE_DIGIT)
	return (DIGIT)(( (((QWORD)D_Dividend1)<<BitsInDIGIT) + D_Dividend2 )
					 / D_Divisor);
#else
	DIGIT	Dq, Dr, Dx, Dy, Dt;

	//	Estimate high half of quotient
	Dq = D_Dividend1 / (HL(D_Divisor)+1);

	//	Subtract the product of estimate and divisor from the dividend
	Dr = D_Dividend1 - (HL(D_Divisor) * Dq);
	Dx = HL(Dr);
	Dy = LH(Dr) + HL(D_Dividend2);
	if( (Dr=Dy-(LL(D_Divisor) * Dq)) > Dy )
		Dx--;

	//	Correct estimate
	while( (Dx>0) || ((Dx==0) && (Dr>=D_Divisor)) ) {
		if( (Dr-=D_Divisor) > NOT(D_Divisor) )
			Dx--;
		Dq++;
	}
	Dt = LH(Dq);

	//	Estimate low half of quotient
	Dq = Dr / (HL(D_Divisor)+1);

	//	Subtract the product of estimate and divisor from the dividend
	Dr -= HL(D_Divisor) * Dq;
	Dx = HL(Dr);
	Dy = LH(Dr) + LL(D_Dividend2);
	if( (Dr=Dy-(LL(D_Divisor) * Dq)) > Dy )
		Dx--;

	//	Correct estimate
	while( (Dx>0) || ((Dx==0) && (Dr>=D_Divisor)) ) {
		if( (Dr-=D_Divisor) > NOT(D_Divisor) )
			Dx--;
		Dq++;
	}

	return( Dt + Dq );
#endif
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
DIGIT KCDSA_CLASS::DD_Mod_D(DIGIT D_Dividend1, DIGIT D_Dividend2, DIGIT D_Divisor)
{
#if defined(DOUBLE_DIGIT)
	return (DIGIT)(( (((QWORD)D_Dividend1)<<BitsInDIGIT) + D_Dividend2 )
					 % D_Divisor);
#else
	DIGIT	Dq, Dr, Dx, Dy;

	//	Estimate high half of quotient
	Dq = D_Dividend1 / (HL(D_Divisor)+1);

	//	Subtract the from dividend the product of estimate and divisor
	Dr = D_Dividend1 - (HL(D_Divisor) * Dq);
	Dx = HL(Dr);
	Dy = LH(Dr) + HL(D_Dividend2);
	if( (Dr=Dy-(LL(D_Divisor) * Dq)) > Dy )
		Dx--;

	//	Correct estimate
	while( (Dx>0) || ((Dx==0) && (Dr>=D_Divisor)) )
		if( (Dr-=D_Divisor) > NOT(D_Divisor) )
			Dx--;

	//	Estimate low half of quotient
	Dq = Dr / (HL(D_Divisor)+1);

	//	Subtract the from dividend the product of estimate and divisor
	Dr -= HL(D_Divisor) * Dq;
	Dx = HL(Dr);
	Dy = LH(Dr) + LL(D_Dividend2);
	if( (Dr=Dy-(LL(D_Divisor) * Dq)) > Dy )
		Dx--;

	//	Correct estimate
	while( (Dx>0) || ((Dx==0) && (Dr>=D_Divisor)) )
		if( (Dr-=D_Divisor) > NOT(D_Divisor) )
			Dx--;

	return( Dr );
#endif
}

/**************************************************************************
*
*	Function Description
*		DIGIT inverse : return D_Src^{-1} mod (b=2^BitsInDIGIT)
*
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
DIGIT KCDSA_CLASS::D_Inv(DIGIT D_Src)
{
	DIGIT	F, G, d, B, C;

	#define XXXX4	3

	//
	C = 1;
	B = 0 - C;
	G = D_Src;
	F = 0 - G;

	for(  ;  ;  ) {
		if( G==1 )
			break;

		if( (F>>XXXX4)>G ) {
			d = F / G;
			F -= d * G;
			B -= d * C;
		}
		else {
			do {
				F -= G;
				B -= C;
			} while( F>G );
		}

		if( F==1 ) {
			C = B;
			break;
		}

		if( (G>>XXXX4)>F ) {
			d = G / F;
			G -= d * F;
			C -= d * B;
		}
		else {
			do {
				G -= F;
				C -= B;
			} while( G>F );
		}
	}

	//
	return (DIGIT)C;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
DIGIT KCDSA_CLASS::D_Gcd(DIGIT D_Src1, DIGIT D_Src2)
{
	DIGIT		tmp;

	//	assert D_Src1>=D_Src2
	if( D_Src1<D_Src2 ) {
		tmp=D_Src1;		D_Src1=D_Src2;	D_Src2=tmp;
	}

	//
	while( D_Src2!=0 ) {
		tmp = D_Src1 % D_Src2;
		D_Src1 = D_Src2;
		D_Src2 = tmp;
	}

	return D_Src1;
}

//########################################
//	DIGIT array의 핵심연산 함수
//########################################

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
DIGIT KCDSA_CLASS::bn_ModD(DIGIT *L_Src, DWORD SrcLen, DIGIT D_Divisor)
{
	DWORD	i;
	DIGIT	xx=0;

	for( i=SrcLen-1; i!=(DWORD)-1; i--) {
		xx = DD_Mod_D(xx, L_Src[i], D_Divisor);
	}

	return xx;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
int KCDSA_CLASS::bn_Cmp(DIGIT *L_Src1, DWORD SrcLen1, DIGIT *L_Src2, DWORD SrcLen2)
{
	DWORD	i;

	if( SrcLen1>=SrcLen2 ) {
		for( i=SrcLen1-1; i!=SrcLen2-1; i--)
			if( L_Src1[i] )		return +1;
	}
	else {
		for( i=SrcLen2-1; i!=SrcLen1-1; i--)
			if( L_Src2[i] )		return -1;
	}

	for(  ; i!=(DWORD)-1; i--) {
		if( L_Src1[i]==L_Src2[i] )		continue;
		else if( L_Src1[i]>L_Src2[i] )	return +1;
		else							return -1;
	}

	return 0;
}

/*************** Function *************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
void KCDSA_CLASS::bn_Copy(DIGIT *L_Dst, DIGIT *L_Src, DWORD SrcLen)
{
	DWORD	i;

	for( i=0; i<SrcLen; i++)
		L_Dst[i] = L_Src[i];
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
 DIGIT KCDSA_CLASS::bn_SHL(DIGIT *L_Dst, DIGIT *L_Src, DIGIT SrcLen, DIGIT NumOfShift)
{
	DWORD	i=SrcLen-1;
	DIGIT	ret;

	ret = L_Src[i] >> (BitsInDIGIT-NumOfShift);
	for(  ; i!=0; i--)
		L_Dst[i] = (L_Src[i]<<NumOfShift)
				 ^ (L_Src[i-1]>>(BitsInDIGIT-NumOfShift));
	L_Dst[i] = L_Src[i] << NumOfShift;

	return ret;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
 DIGIT KCDSA_CLASS::bn_SHR(DIGIT *L_Dst, DIGIT *L_Src, DIGIT SrcLen, DIGIT NumOfShift)
{
	DWORD	i=0;
	DIGIT	ret;

	ret = L_Src[i] << (BitsInDIGIT-NumOfShift);
	for( i=0; i<SrcLen-1; i++)
		L_Dst[i] = (L_Src[i]>>NumOfShift)
				 ^ (L_Src[i+1]<<(BitsInDIGIT-NumOfShift));
	L_Dst[i] = L_Src[i] >> NumOfShift;

	return ret;
}


/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
 DIGIT KCDSA_CLASS::bn_Add(DIGIT *L_Dst, DIGIT *L_Src1, DWORD SrcLen1,
						   DIGIT *L_Src2, DWORD SrcLen2)
{
	DWORD	i;
	DIGIT	carry, tmp;

	//
	for( carry=i=0; i<SrcLen2; i++) {
		if( (L_Src2[i]==((DWORD)-1)) && (carry==1) )
			L_Dst[i] = L_Src1[i];
		else {
			tmp = L_Src2[i] + carry;
			L_Dst[i] = L_Src1[i] + tmp;
			carry = ( (L_Dst[i])<tmp ) ? 1 : 0;
		}
	}

	//
	if( carry==0 ) {
		if( L_Dst!=L_Src1 )
			for(  ; i<SrcLen1; i++)
				L_Dst[i] = L_Src1[i];
		return 0;
	}

	//
	for(  ; i<SrcLen1; i++)
		if( (++L_Dst[i])!=0 )	return 0;

	return 1;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
DIGIT KCDSA_CLASS::bn_Sub(DIGIT *L_Dst, DIGIT *L_Src1, DWORD SrcLen1,
						   DIGIT *L_Src2, DWORD SrcLen2)
{
	DWORD	i;
	DIGIT	carry, tmp;

	//
//	tmp = ASM_Sub(L_Dst, L_Src1, L_Src2, SrcLen2);
	for( carry=i=0; i<SrcLen2; i++) {
		if( L_Src2[i]+carry==0 )
			L_Dst[i] = L_Src1[i];
		else {
			tmp = L_Src2[i] + carry;
			L_Dst[i] = L_Src1[i] - tmp;
			carry = ( (L_Dst[i])>NOT(tmp) ) ? 1 : 0;
		}
	}

	//
	if( carry==0 ) {
		if( L_Dst!=L_Src1 )
			for(  ; i<SrcLen1; i++)
				L_Dst[i] = L_Src1[i];
		return 0;
	}

	//
	for(  ; i<SrcLen1; i++)
		if( (L_Dst[i]--)!=0 )	return 0;

	return 1;
}

/*************** Function *************************************************
*	Long multiple DIGIT ::
*		L_Dst[SrcLen-1..0] = L_Multiplicand[SrcLen-1..0] * D_Multiplier
*			and return the carries
*	** Assume SrcLen>0 **
*/
 DIGIT KCDSA_CLASS::bn_MulD(DIGIT *L_Dst, DIGIT *L_Src, DIGIT SrcLen,
					 DIGIT D_Multiplier)
{
	DWORD	i;
	DIGIT	La[2], tmp=0;

	for( i=0; i<SrcLen; i++) {
		D_Mul_D(La, D_Multiplier, L_Src[i]);
		La[0] += tmp;
		if( (La[0])<tmp )	La[1]++;
		L_Dst[i] = La[0];
		tmp = La[1];
	}

	return tmp;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
DIGIT KCDSA_CLASS::bn_MulAdd(DIGIT *L_Dst, DWORD DstLen,
						  DIGIT *L_Src, DWORD SrcLen, DIGIT D_Multiplier)
{
	DWORD	i;
	DIGIT	tmp, La[2];

	for( tmp=0, i=0; i<SrcLen; i++) {
		D_Mul_D(La, D_Multiplier, L_Src[i]);
		if( (tmp+=La[0])<La[0] )	La[1]++;
		if( (L_Dst[i]+=tmp)<tmp )	La[1]++;
		tmp = La[1];
	}

	//
	if( i==DstLen )				return tmp;
	if( (L_Dst[i]+=tmp)>=tmp )	return 0;

	//
	for( i++; i<DstLen; i++)
		if( (++L_Dst[i])!=0 )	return 0;

	return 1;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
 DIGIT	KCDSA_CLASS::bn_MulSub(DIGIT *L_Dst, DWORD DstLen,
						  DIGIT *L_Src, DWORD SrcLen, DIGIT D_Multiplier)
{
	DWORD	i;
	DIGIT	tmp, La[2];

	for( tmp=0, i=0; i<SrcLen; i++) {
		D_Mul_D(La, D_Multiplier, L_Src[i]);
		tmp += La[0];
		if( tmp<La[0] ) La[1]++;
		if( L_Dst[i]<tmp )  La[1]++;
		L_Dst[i] -= tmp;
		tmp = La[1];
	}

	//
	if( i==DstLen )				return tmp;
	if( L_Dst[i]>=tmp ) {
		L_Dst[i] -= tmp;
		return 0;
	}
	else
		L_Dst[i] -= tmp;

	//
	for( i++; i<DstLen; i++)
		if( (L_Dst[i]--)!=0 )	return 0;

	return 1;
}

/*************** Function *************************************************
*	Long square : L_Dst[2*SrcLen-1..0] <- L_Src[SrcLen-1..0] ^ 2
*	** Assume SrcLen>1 **
*/
 void KCDSA_CLASS::bn_Sqr(DIGIT *L_Dst, DIGIT *L_Src, DWORD SrcLen)
{
	DWORD	i, j;
	DIGIT	tmp, La[2];

	//	Step 1 : L_Dst[SrcLen-1..0] = 0
	L_Dst[0] = L_Dst[SrcLen+SrcLen-1] = 0;

	//	Step 2 : L_Dst <- Sum_{i<j} L_Src[i]*L_Src[j]*b^{i+j}
	L_Dst[SrcLen] = bn_MulD( L_Dst+1, L_Src+1, SrcLen-1, L_Src[0]);
	for( i=1; i<=SrcLen-2; i++)
		L_Dst[SrcLen+i] = bn_MulAdd(L_Dst+1+i+i, SrcLen-1-i,
									L_Src+1+i, SrcLen-1-i, L_Src[i]);

	//	Step 3 : L_Dst[2n-1..1] <<1 (= C*2) , L_Dst[0] = 0
	bn_SHL(L_Dst, L_Dst, SrcLen+SrcLen, 1);

	//	Step 4 : L_Dst <- L_Dst + Sum_{i=0..SrcLen-1} L_Src[i]^2 * b^{2i}
	for( tmp=i=j=0; i<SrcLen; i++,j+=2 ) {
		D_Mul_D(La, L_Src[i], L_Src[i]);
		if( (La[0]+=tmp)<tmp )			La[1]++;
		if( (L_Dst[j]+=La[0])<La[0] )	La[1]++;
		if( (L_Dst[j+1]+=La[1])<La[1] ) tmp = 1;
		else							tmp = 0;
	}
}

/*************** Function *************************************************
*	Long multiple :  L_Dst[SrcLen1+SrcLen2-1..0] = L_Src1[SrcLen1-1..0]
*												 * L_Src2[SrcLen2-1..0]
*	** Assume SrcLen1>=SrcLen2 > 1 **
*/
 void KCDSA_CLASS::bn_Mul(DIGIT *L_Dst, DIGIT *L_Src1, DWORD SrcLen1,
						  DIGIT *L_Src2, DWORD SrcLen2)
{
	DIGIT	La[2], tmp;
	DWORD	i, j;

	//
	for( i=0; i<SrcLen1+SrcLen2; i++)
		L_Dst[i] = 0;

	//
	for( j=0; j<SrcLen2; j++) {
		for( tmp=0,i=0; i<SrcLen1; i++) {
			D_Mul_D(La, L_Src1[i], L_Src2[j]);
			if( (tmp+=La[0])<La[0] )	La[1]++;
			if( (L_Dst[i+j]+=tmp)<tmp )	La[1]++;
			tmp = La[1];
		}
		L_Dst[i+j] = tmp;
	}
}

/*************** Function *************************************************
*	Long square by Karatsuba algorithm ::
*		L_Dst[2*SrcLen-1..0] <- L_Src[SrcLen-1..0] ^ 2
*	** Assume SrcLen>1 **
*/
 void KCDSA_CLASS::bn_KaraSqr(DIGIT *L_Dst, DIGIT *L_Src, DWORD SrcLen)
{
	BOOL	FLAG=0;
	DIGIT	n2=(SrcLen+1)/2, tmp=0;
	DIGIT	S[MaxDIGIT+2];
	DIGIT	T[MaxDIGIT+2];

	if( SrcLen==1 ) {
		D_Mul_D(L_Dst, L_Src[0], L_Src[0]);
		return;
	}

	if( SrcLen<Kara_Sqr_Length ) {
		bn_Sqr(L_Dst, L_Src, SrcLen);
		return;
	}

	if( SrcLen&1 ) {
		tmp = L_Src[SrcLen];
		L_Src[SrcLen] = 0;
		FLAG = 1;
		SrcLen ++;
	}

	if( bn_Cmp(L_Src+n2, n2, L_Src, n2)==1 )
		bn_Sub(S, L_Src+n2, n2, L_Src, n2);
	else
		bn_Sub(S, L_Src, n2, L_Src+n2, n2);

	bn_KaraSqr(T, S, n2);
	bn_KaraSqr(L_Dst, L_Src, n2);
	bn_KaraSqr(L_Dst+SrcLen, L_Src+n2, n2);

	S[SrcLen] = bn_Add(S, L_Dst, SrcLen, L_Dst+SrcLen, SrcLen);
	bn_Sub(S, S, SrcLen+1, T, SrcLen);
	bn_Add(L_Dst+n2, L_Dst+n2, SrcLen+n2, S, SrcLen+1);

	if( FLAG ) {
		SrcLen--;
		L_Src[SrcLen] = tmp;
	}
}

/*************** Function *************************************************
*	Long multiple by Karatsuba algorithm ::
*		L_Dst[2*SrcLen-1..0] = L_Src1[SrcLen-1..0] * L_Src2[SrcLen-1..0]
*	** Assume SrcLen > 1 **
*/
 void	KCDSA_CLASS::bn_KaraMul(DIGIT *L_Dst, DIGIT *L_Src1, DIGIT *L_Src2, DWORD SrcLen)
{
	BOOL	FLAG=0, SIGN=0;
	DIGIT	n2=(SrcLen+1)/2, tmp1=0, tmp2=0;
	DIGIT	S[MaxDIGIT+2];
	DIGIT	T[MaxDIGIT+2];

#define TempHalf	(MaxDIGIT+2)/2

	if( SrcLen==1 ) {
		D_Mul_D(L_Dst, L_Src1[0], L_Src2[0]);
		return;
	}

	if( SrcLen<Kara_Mul_Length ) {
		bn_Mul( L_Dst, L_Src1, SrcLen, L_Src2, SrcLen);
		return;
	}

	if( SrcLen&1 ) {
		tmp1 = L_Src1[SrcLen];
		L_Src1[SrcLen] = 0;
		tmp2 = L_Src2[SrcLen];
		L_Src2[SrcLen] = 0;
		FLAG = 1;
		SrcLen++;
	}

	if( bn_Cmp(L_Src1+n2, n2, L_Src1, n2)==1 )
		bn_Sub(S, L_Src1+n2, n2, L_Src1, n2);
	else {
		bn_Sub(S, L_Src1, n2, L_Src1+n2, n2);
		SIGN++;
	}
	if( bn_Cmp(L_Src2+n2, n2, L_Src2, n2)==1 )
		bn_Sub(S+TempHalf, L_Src2+n2, n2, L_Src2, n2);
	else {
		bn_Sub(S+TempHalf, L_Src2, n2, L_Src2+n2, n2);
		SIGN++;
	}

	bn_KaraMul(T, S, S+TempHalf, n2);
	bn_KaraMul(L_Dst, L_Src1, L_Src2, n2);
	bn_KaraMul(L_Dst+SrcLen, L_Src1+n2, L_Src2+n2, n2);

	S[SrcLen] = bn_Add(S, L_Dst, SrcLen, L_Dst+SrcLen, SrcLen);
	if( SIGN==1 )
		bn_Add(S, S, SrcLen+1, T, SrcLen);
	else
		bn_Sub(S, S, SrcLen+1, T, SrcLen);
	bn_Add(L_Dst+n2, L_Dst+n2, SrcLen+n2, S, SrcLen+1);

	if( FLAG ) {
		SrcLen --;
		L_Src1[SrcLen] = tmp1;
		L_Src2[SrcLen] = tmp2;
	}
}

/*************** Function *************************************************
*	Long division ::
*		L_Dst[SrcLen1-SrcLen2-1..0] = L_Src1[SrcLen1-1..0]
*									  div L_Src2[SrcLen2-1..0]
*		L_Rem[SrcLen1-1..0] = L_Src1[SrcLen1-1..0]
*							  mod L_Src2[SrcLen2-1..0]
*				& return 0 if remainder equals 0
*/
 DIGIT KCDSA_CLASS::bn_Div(DIGIT *L_Dst, DIGIT *L_Rem,
					DIGIT *L_Src1, DWORD SrcLen1,
					DIGIT *L_Src2, DWORD SrcLen2)
{
	DIGIT	i, q, c, make_MSB;
	DIGIT	C[2*(MaxDIGIT+2)];

	bn_Copy(C, L_Src1, SrcLen1);
	C[SrcLen1] = 0;
	c = SrcLen1 + 1;

	//	Step 1 : Standardize L_Src2 s.t. L_Src2[SrcLen2-1] > b/2
	make_MSB = 0;
	for( i=SrcLen2*BitsInDIGIT-1; !CheckBitDIGIT(L_Src2,i); i--,make_MSB++);
	if( make_MSB!=0 ) {
		bn_SHL(C, C, c, make_MSB);
		bn_SHL(L_Src2, L_Src2, SrcLen2, make_MSB);
	}

	//	Step 2 : main part
	for( i=c-SrcLen2-1; i!=(DIGIT)-1; i--) {
		//	Step 2-1 : Estimate q
		if( C[SrcLen2+i]==L_Src2[SrcLen2-1] )
			q = (DIGIT)-1;
		else
			q = DD_Div_D(C[SrcLen2+i], C[SrcLen2+i-1], L_Src2[SrcLen2-1]);

		//	Step 2-2 : Make C <- C-q*L_Src2
		if( bn_MulSub(C+i, SrcLen2+1, L_Src2, SrcLen2, q) ) {
			q--;
			if( bn_Add(C+i, C+i, SrcLen2+1, L_Src2, SrcLen2)==0 ) {
				q--;
				bn_Add(C+i, C+i, SrcLen2+1, L_Src2, SrcLen2);
			}
		}
		L_Dst[i] = q;
	}

	//	Step 3 : Recover L_Src2
	if( make_MSB!=0 ) {
		bn_SHR(L_Src2, L_Src2, SrcLen2, make_MSB);
		bn_SHR(C, C, SrcLen2, make_MSB);
	}

	//	
	if( L_Rem!=NULL )
		bn_Copy(L_Rem, C, SrcLen2);

	//
	for( i=0; i<c; i++)
		if( C[i]!=0 )
			return 1;
	return 0;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
 RET_VAL KCDSA_CLASS::bn_Euclid(DIGIT *L_Dst, DIGIT *L_Src,
						 DIGIT *L_Modulus, DWORD ModLen)
{
	DWORD		i, ret, make_ODD, Len_1=ModLen+1;
	DIGIT		*U1, *U2, *U3, *T1, *T2, *T3, *Temp;
	DIGIT		Value1[MaxDIGIT+2], Value2[MaxDIGIT+2], Value3[MaxDIGIT+2],
				Value4[MaxDIGIT+2], Value5[MaxDIGIT+2], Value6[MaxDIGIT+2];

	U1 = Value1;
	U2 = Value2;
	U3 = Value3;
	T1 = Value4;
	T2 = Value5;
	T3 = Value6;

	//	Step 1 : Divide both L_Src and L_Modulus by 2 as long as possible
	for ( make_ODD=0;  ; make_ODD++)
		if( CheckBitDIGIT(L_Src, make_ODD) || CheckBitDIGIT(L_Modulus, make_ODD) )
			break;
	if( make_ODD!=0 ) {
		if( make_ODD>=BitsInDIGIT ) {
			assert( 1==0 );
		}
		else {
			ret = bn_SHR(L_Src, L_Src, ModLen, make_ODD);
			assert( ret==0 );
			ret = bn_SHR(L_Modulus, L_Modulus, ModLen, make_ODD);
			assert( ret==0 );
		}
	}

	//	Initialize U1, U2, U3, V1, V2, V3, T1, T2, T3
	for( i=0; i<ModLen; i++) {
		U1[i] = U2[i] = 0;
		U3[i] = T2[i] = L_Modulus[i];
		T1[i] = T3[i] = L_Src[i];
	}
	U1[i] = U2[i] = U3[i] = T2[i] = T1[i] = T3[i] = 0;
	U1[0] = 1;
	if( T2[0]!=0 )	T2[0] -= 1;
	else			bn_Sub(T2, T2, ModLen, bn_One, 1);

	//	main part
	do {
		do {
			if( isEven0(U3) ) {
				if( isOdd0(U1) || isOdd0(U2) ) {
					bn_Add(U1, U1, Len_1, L_Src, ModLen);
					bn_Add(U2, U2, Len_1, L_Modulus, ModLen);
				}
				bn_SHR(U1, U1, Len_1, 1);
				bn_SHR(U2, U2, Len_1, 1);
				bn_SHR(U3, U3, Len_1, 1);
			}

			if( isEven0(T3) || bn_Cmp(U3, Len_1, T3, Len_1)<0 ) {
				Temp = U1;	U1 = T1;	T1 = Temp;
				Temp = U2;	U2 = T2;	T2 = Temp;
				Temp = U3;	U3 = T3;	T3 = Temp;
			}
		} while( isEven0(U3) );

		while( (bn_Cmp(U1, Len_1, T1, Len_1)<0)
			|| (bn_Cmp(U2, Len_1, T2, Len_1)<0) ) {
			bn_Add(U1, U1, Len_1, L_Src, ModLen);
			bn_Add(U2, U2, Len_1, L_Modulus, ModLen);
		}

		bn_Sub(U1, U1, Len_1, T1, Len_1);
		bn_Sub(U2, U2, Len_1, T2, Len_1);
		bn_Sub(U3, U3, Len_1, T3, Len_1);
	} while( bn_Cmp(T3, Len_1, bn_Zero, 1)>0 );

	while( (bn_Cmp(U1, Len_1, L_Src, ModLen)>=0)
		&& (bn_Cmp(U2, Len_1, L_Modulus, ModLen)>=0) ) {
		bn_Sub(U1, U1, Len_1, L_Src, ModLen);
		bn_Sub(U2, U2, Len_1, L_Modulus, ModLen);
	}

	//	Inverse of Step 1
	if( make_ODD!=0 ) {
		if( make_ODD>=BitsInDIGIT ) {
			assert( 1==0 );
		}
		else {
			ret = bn_SHL(L_Src, L_Src, ModLen, make_ODD);
			assert( ret==0 );
			ret = bn_SHL(L_Modulus, L_Modulus, ModLen, make_ODD);
			assert( ret==0 );
			ret = bn_SHL(U3, U3, ModLen, make_ODD);
			assert( ret==0 );
		}
	}

	//
	if ( bn_Cmp(U3, ModLen, bn_One, 1)==0 ) {//	gcd(L_Dst,L_Modulus)==1
		bn_Sub(L_Dst, L_Modulus, ModLen, U2, ModLen);
											//	L_Dst <- L_Src^{-1} mod L_Modulus
		return CTR_SUCCESS;
	}
	else {									//	gcd(L_Dst,L_Modulus)>1
		for( i=0; i<ModLen; i++)
			L_Dst[i] = U3[i];				//	L_Dst <- gcd(L_Src,L_Modulus)
		return CTR_VERIFY_FAIL;
	}
}

 DWORD KCDSA_CLASS::MakeAddChain(
			int		AddChain[][2],	//	결과를 넣을 메무리
			int		WindowSize,		//	
			DIGIT	*L_Exponent,	//
			DWORD	msb,			//
			BOOL	Type)			//	Type='FirstWindowMayBeEven'이면
									//	최초 window가 짝수일수도 있음
{
	int		i=msb, j, SubExp, idx=0;

	//
	for( i=msb; i>=0; i--)
		if( CheckBitDIGIT(L_Exponent, i) )	break;
	if( i==-1 ) {
		AddChain[idx][0] = -1;
		AddChain[idx][1] = -1;
		return 0;
	}

	//
	if( Type==FirstWindowMayBeEven ) {
		j = ( i-(int)WindowSize+1>=0 ) ? i-(int)WindowSize+1 : 0;
		for( SubExp=0; i>=j; i--) {
			SubExp <<= 1;
			if( CheckBitDIGIT(L_Exponent, i) )
				SubExp ^= 1;
		}
		AddChain[idx][0] = i+1;
		AddChain[idx][1] = SubExp;
		idx++;
	}

	//	main part of this function
	for(  ; i>=0;  ) {
		if( CheckBitDIGIT(L_Exponent, i)==0 ) {
			i--;
			continue;
		}

		//	Find LSB of the Window
		j = i - (int)WindowSize + 1;
		if( j<0 )	j = 0;
		for(  ; j<=i; j++)
			if( CheckBitDIGIT(L_Exponent, j) )
				break;

		//	Get the Window value = 'SubExp'
		for( SubExp=0; i>=j; i--) {
			SubExp <<= 1;
			if( CheckBitDIGIT(L_Exponent, i) )
				SubExp ^= 1;
		}
		AddChain[idx][0] = i+1;
		AddChain[idx][1] = SubExp;
		idx++;
	}

	AddChain[idx][0] = -1;
	AddChain[idx][1] = -1;
	return idx;
}
/*************** Function *************************************************
*	Montgomery Modular Reduction Algorithm
*		L_Dst[ModLen..0] <- L_Dst[DstLen..0] mod L_Modulus[ModLen..0]
*	** Assume L_Dst[] has at least (2*ModLen+1)-DIGIT memory **
*/
RET_VAL KCDSA_CLASS::Montgomery_REDC(DIGIT *L_Dst, DWORD DstLen,
						DIGIT *L_Modulus, DWORD ModLen)
{
	DIGIT i;

	if( DstLen!=ModLen+ModLen ) {
		for( i=DstLen; i<ModLen+ModLen+1; i++)
			L_Dst[i] = 0;
		DstLen = ModLen + ModLen;
	}

	L_Dst[DstLen] = 0;

	//	Secial Case :: Modulus = 64times 1 || **** || 64times 1
	if( Montgo_Inv==1 ) {
		DIGIT j;

		for( j=0;  ; j++)
			if( (++L_Modulus[j])!=0 )	break;
		for( i=0; i<ModLen; i++)
			bn_MulAdd(L_Dst+i+j, ModLen+ModLen+2-i-j,
					  L_Modulus+j, ModLen-j, L_Dst[i]);
		for(  ; j!=-1; j--)
			L_Modulus[j]--;
	}
	else
		for( i=0; i<ModLen; i++)
			bn_MulAdd(L_Dst+i, ModLen+ModLen+2-i,
					  L_Modulus, ModLen, (DIGIT)(L_Dst[i]*Montgo_Inv));

	//
	if( bn_Cmp(L_Dst+ModLen, ModLen+1, L_Modulus, ModLen)>=0 )
		bn_Sub(L_Dst, L_Dst+ModLen, ModLen+1, L_Modulus, ModLen);
	else
		bn_Copy(L_Dst, L_Dst+ModLen, ModLen);

	//

	return CTR_SUCCESS;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/


RET_VAL KCDSA_CLASS::bn_ModExp(DIGIT *L_Dst, DIGIT *L_Base, DWORD BaseLen,
						 DIGIT *L_Exponent, DWORD ExpLen,
						 DIGIT *L_Modulus, DWORD ModLen)
{
	RET_VAL		ret;
	DWORD		i, j, MSB=0, WindowSize;
	DIGIT		*P1, *P2, *P3;

//	KCDSA_CLASS	KC;

	RET_VAL (KCDSA_CLASS::*Algorithm_REDC)(DIGIT *L_Dst, DWORD Len_Res,
							DIGIT *L_Modulus, DWORD ModLen);

	DIGIT		L_Temp1[2*(MaxDIGIT+2)];
	DIGIT		L_Temp2[2*(MaxDIGIT+2)];

	//	Fine the Most-Significant-Bit of exponent L_Exponent
	i = (int)ExpLen * BitsInDIGIT - 1;
	for(  ; i!=(DWORD)-1; i--)
		if( CheckBitDIGIT(L_Exponent, i) )
			break;

	if( i==(DWORD)-1 ) {		//	L_Exponent[] = 0
		L_Dst[0] = 1;
		for( j=1; j<(int)ModLen; j++)	L_Dst[j] = 0;
		//
		ret = CTR_SUCCESS;
		goto LABEL_END;
	}
	if( i==0 ) {				//	L_Exponent[] = 1
		for( j=0; j<(int)BaseLen; j++)	L_Dst[j] = L_Base[j];
		for(    ; j<(int)ModLen; j++)	L_Dst[j] = 0;
		//
		ret = CTR_SUCCESS;
		goto LABEL_END;
	}

	//	Determine Window size
	if	   ( i<32 )		WindowSize = 1;
	else if( i<60 )		WindowSize = 3;
	else if( i<220 )	WindowSize = 4;
	else if( i<636 )	WindowSize = 5;
	else if( i<1758 )	WindowSize = 6;
	else				WindowSize = 7;
	if( WindowSize>Max_W_size )		WindowSize = Max_W_size;

	//	Determine
	Algorithm_REDC = &KCDSA_CLASS::Montgomery_REDC;   //	Apply Montgomery Algorithm

	//
	for( j=0; j<(int)BaseLen; j++)	L_Temp2[j] = L_Base[j];
	for(    ; j<(int)ModLen; j++)	L_Temp2[j] = 0;

	//	initialize for ModRed depand on L_Modulus
	ret = Montgomery_Init(L_Modulus, ModLen);					GOTO_END;

	//	Change number system Zn to RZn : L_Temp1<-L_Base in RZn
	ret = Montgomery_Zn2RZn(L_Temp1, L_Temp2, L_Modulus, ModLen);
																GOTO_END;

	////	Binary method
	if( WindowSize==1 ) {
		bn_Copy(L_Dst, L_Temp1, ModLen);
		P1 = L_Temp1;
		P2 = L_Temp2;
		for( i--; i!=-1; i--) {
			bn_KaraSqr(P2, P1, ModLen);
			ret = (this->*Algorithm_REDC)(P2, 2*ModLen, L_Modulus, ModLen);
																	GOTO_END;
			if( CheckBitDIGIT(L_Exponent, i) ) {
				bn_KaraMul(P1, P2, L_Dst, ModLen);
				ret = (this->*Algorithm_REDC)(P1, 2*ModLen, L_Modulus, ModLen);
																	GOTO_END;
			}
			else {
				P3 = P1;	P1 = P2;	P2 = P3;
			}
		}
	}
	else {	////	Window method
		//	Precompute and save in *PP_W(x)
		bn_KaraSqr(L_Temp2, L_Temp1, ModLen);	//	L_Temp2<-L_Base^2
		ret = (this->*Algorithm_REDC)(L_Temp2, 2*ModLen, L_Modulus, ModLen);	GOTO_END;
		bn_Copy(PP_W(0), L_Temp1, ModLen);
		for( j=1; j<((DWORD)1<<(WindowSize-1)); j++) {
			bn_KaraMul(L_Temp1, PP_W(j-1), L_Temp2, ModLen);
			ret = (this->*Algorithm_REDC)(L_Temp1, 2*ModLen, L_Modulus, ModLen);	GOTO_END;
			bn_Copy(PP_W(j), L_Temp1, ModLen);
		}

		//	Get Addition Chain
		i = MakeAddChain(Add_Chain, WindowSize, L_Exponent, i, FirstWindowMayBeEven);
		if( i>=BN_MAX_BITS/Max_W_size ) {
			ret = CTR_FATAL_ERROR;
			goto LABEL_END;
		}

		if( Add_Chain[0][1]&1 )		//	if SubExp == odd
			bn_Copy(L_Temp2, PP_W(Add_Chain[0][1]/2), ModLen);
		else {
			bn_KaraMul(L_Temp2, PP_W(0), PP_W(Add_Chain[0][1]/2-1), ModLen);
			ret = (this->*Algorithm_REDC)(L_Temp2, 2*ModLen, L_Modulus, ModLen);	GOTO_END;
		}
		i = Add_Chain[0][0] - 1;
		j = 1;

		//	main part of this function
		P1 = L_Temp2;
		P2 = L_Temp1;
		for(  ; i!=(DWORD)-1; i--) {
			bn_KaraSqr(P2, P1, ModLen);
			ret = (this->*Algorithm_REDC)(P2, 2*ModLen, L_Modulus, ModLen);	GOTO_END;
			P3 = P1;	P1 = P2;	P2 = P3;

			if( i==(DWORD)Add_Chain[j][0] ) {
				bn_KaraMul(P2, P1, PP_W(Add_Chain[j][1]>>1), ModLen);
				ret = (this->*Algorithm_REDC)(P2, 2*ModLen, L_Modulus, ModLen);	GOTO_END;
				P3 = P1;	P1 = P2;	P2 = P3;
				j++;
			}
		}
	}

	//
	if( Algorithm_REDC == &KCDSA_CLASS::Montgomery_REDC ) {
		//	Change number system RZn to Zn
		ret = Montgomery_REDC(P1, ModLen, L_Modulus, ModLen);		GOTO_END;
	}
	else if( MSB ) {
		bn_SHR(L_Modulus, L_Modulus, ModLen, MSB);
		ret = Classical_REDC(P1, ModLen+1, L_Modulus, ModLen);		GOTO_END;
	}

	//
	bn_Copy(L_Dst, P1, ModLen);

	ret = CTR_SUCCESS;
LABEL_END:
	return ret;
}

//########################################
//	BIG_NUM의 지원 함수 
//########################################

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
BIG_NUM*	KCDSA_CLASS::CreateBigNum(DWORD dMemoryLen)
{
	BYTE		*pbTemp;
	BIG_NUM		*BN_Num;

	//
	if( dMemoryLen==0 )
		dMemoryLen = MaxDIGIT;

	pbTemp = (BYTE *) malloc(sizeof(BIG_NUM)+(dMemoryLen+1)*sizeof(DIGIT));
	if( pbTemp==NULL )	return NULL;

	//
	BN_Num = (BIG_NUM *) pbTemp;
	BN_Num->Length = 0;
	BN_Num->Space = dMemoryLen+1;
	BN_Num->pData = (DIGIT *) (pbTemp + sizeof(BIG_NUM));

	//
	return BN_Num;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
void	KCDSA_CLASS::DestroyBigNum(BIG_NUM *BN_Src)
{
	DWORD		i;

	if( BN_Src!=NULL ) {
		i = sizeof(BIG_NUM) + BN_Src->Space * sizeof(DIGIT);

		memset((BYTE *)BN_Src, 0, i);
		free(BN_Src);
	}
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
RET_VAL	KCDSA_CLASS::BN2OS(
			BIG_NUM	*BN_Src,	//	Source integer
			DWORD	dDstLen,	//	Destination Length in BYTEs
			BYTE	*pbDst)		//	Destination Octet string pointer
{
	DWORD	i;

	CheckMostSignificantDIGIT(BN_Src);

	if( (DIGITSIZE*BN_Src->Length)<=dDstLen ) {
		for( i=0; i<dDstLen; i++)
			pbDst[i] = 0;
		for( i=0; (dDstLen!=0) && (i<BN_Src->Length); i++) {
			pbDst[--dDstLen] = (BYTE)((BN_Src->pData[i]    ) & 0xFF);
			pbDst[--dDstLen] = (BYTE)((BN_Src->pData[i]>> 8) & 0xFF);
			pbDst[--dDstLen] = (BYTE)((BN_Src->pData[i]>>16) & 0xFF);
			pbDst[--dDstLen] = (BYTE)((BN_Src->pData[i]>>24) & 0xFF);
		}
	}
	else {
		i = (DIGITSIZE*BN_Src->Length) - dDstLen;
		if( i>=DIGITSIZE )
			return CTR_BUFFER_TOO_SMALL;
		else if( BN_Src->pData[BN_Src->Length-1]>>(8*(DIGITSIZE-i))  )
			return CTR_BUFFER_TOO_SMALL;

		for( i=0;  ; i++) {
			pbDst[--dDstLen] = (BYTE)((BN_Src->pData[i]    ) & 0xFF);
			if( dDstLen==0 )	break;
			pbDst[--dDstLen] = (BYTE)((BN_Src->pData[i]>> 8) & 0xFF);
			if( dDstLen==0 )	break;
			pbDst[--dDstLen] = (BYTE)((BN_Src->pData[i]>>16) & 0xFF);
			if( dDstLen==0 )	break;
			pbDst[--dDstLen] = (BYTE)((BN_Src->pData[i]>>24) & 0xFF);
			if( dDstLen==0 )	break;
		}
	}

	return CTR_SUCCESS;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
RET_VAL	KCDSA_CLASS::OS2BN(
			BYTE	*pbSrc,		//	Source Octet string pointer
			DWORD	dSrcLen,	//	Source Length in BYTEs
			BIG_NUM	*BN_Dst)	//	Destination DIGIT array pointer
{
	DWORD	i;
	RET_VAL	ret;

	BN_Dst->Length = 0;
	BN_Dst->pData[0] = 0;

	for( i=0; i<dSrcLen; i++) {
		ret = BN_SHL(BN_Dst, BN_Dst, 8);	if( ret!=CTR_SUCCESS )	return ret;
		BN_Dst->pData[0] ^= pbSrc[i];
		if( BN_Dst->Length==0 )
			BN_Dst->Length = 1;
	}

	return CTR_SUCCESS;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
RET_VAL KCDSA_CLASS::BN_Copy(BIG_NUM *BN_Dst, BIG_NUM *BN_Src)
{
	DWORD	i;

	if( BN_Dst!=BN_Src ) {
		CheckInput_MemLen(BN_Src);

		BN_Dst->Length = BN_Src->Length;
		CheckOutput_MemLen(BN_Dst);

		//	copy long values
		for( i=0; i<BN_Dst->Length; i++)
			BN_Dst->pData[i] = BN_Src->pData[i];
	}

	return CTR_SUCCESS;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
int KCDSA_CLASS::BN_Cmp(BIG_NUM *BN_Src1, BIG_NUM *BN_Src2)
{
	CheckInput_MemLen(BN_Src1);
	CheckInput_MemLen(BN_Src2);

	//
	if( BN_Src1->Length >= BN_Src2->Length )
		return  bn_Cmp(BN_Src1->pData, BN_Src1->Length,
					   BN_Src2->pData, BN_Src2->Length);
	else
		return -bn_Cmp(BN_Src2->pData, BN_Src2->Length,
					   BN_Src1->pData, BN_Src1->Length);
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
RET_VAL KCDSA_CLASS::BN_Rand(BIG_NUM *BN_Dst, DWORD BitLen)
{
	DWORD	i, j;

	//
	for( i=0; i<BitLen/BitsInDIGIT; i++)
		BN_Dst->pData[i] = rand() ^ (rand()<<11);

	j = BitLen%BitsInDIGIT;
	if( j ) {
		BN_Dst->pData[i] = rand() ^ (rand()<<11);
		BN_Dst->pData[i] &= (((DIGIT)1)<<j) - 1;
		i++;
	}

	//
	BN_Dst->Length = (BitLen-1)/BitsInDIGIT + 1;
	CheckOutput_MemLen(BN_Dst);

	return CTR_SUCCESS;
}

//########################################
//	BIG_NUM의 핵심연산 함수
//########################################

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
RET_VAL KCDSA_CLASS::BN_SHL(BIG_NUM *BN_Dst, BIG_NUM *BN_Src, DWORD NumOfShift)
{
	DWORD	i;
	DIGIT	t;

	CheckInput_MemLen(BN_Src);

	//
	if( BN_Src->Length==0 ) {
		return BN_Copy(BN_Dst, &BN_Zero);
	}

	//	if ( n>=BitsInDIGIT ) then DIGIT 단위로 left shift
	t = NumOfShift % BitsInDIGIT;
	if( t ) {
		BN_Dst->Length = BN_Src->Length;
		t = bn_SHL(BN_Dst->pData, BN_Src->pData, BN_Src->Length, t);
		if( t )
			BN_Dst->pData[BN_Dst->Length++] = t;
	}

	t = NumOfShift / BitsInDIGIT;
	if( t ) {
		BN_Dst->Length = BN_Src->Length + t;
		CheckOutput_MemLen(BN_Dst);
		for( i=BN_Dst->Length-t-1; i!=(DIGIT)-1; i--)
			BN_Dst->pData[t+i] = BN_Src->pData[i];
		for( i=0; i<t; i++)
			BN_Dst->pData[i] = 0;
	}

	//
	CheckMostSignificantDIGIT(BN_Dst);
	return CTR_SUCCESS;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
RET_VAL KCDSA_CLASS::BN_SHR(BIG_NUM *BN_Dst, BIG_NUM *BN_Src, DWORD NumOfShift)
{
	DWORD	i;
	DIGIT	t;

	CheckInput_MemLen(BN_Src);

	//	if ( n>=BitsInDIGIT ) then DIGIT 단위로 left shift
	t = NumOfShift / BitsInDIGIT;
	if( t ) {
		if( t>=BN_Src->Length )
			return BN_Copy(BN_Dst, &BN_Zero);

		for( i=0; i<BN_Src->Length-t; i++)
			BN_Dst->pData[i] = BN_Src->pData[i+t];
		BN_Dst->Length = BN_Src->Length - t;
		CheckMostSignificantDIGIT(BN_Dst);
	}
	else
		BN_Copy(BN_Dst, BN_Src);

	t = NumOfShift % BitsInDIGIT;
	if( t ) {
		bn_SHR(BN_Dst->pData, BN_Dst->pData, BN_Dst->Length, t);
	}

	//
	CheckMostSignificantDIGIT(BN_Dst);
	return CTR_SUCCESS;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
RET_VAL KCDSA_CLASS::BN_Add(BIG_NUM *BN_Dst, BIG_NUM *BN_Src1, BIG_NUM *BN_Src2)
{
	DWORD	tmp;
	DIGIT	carry;

	CheckInput_MemLen(BN_Src1);
	CheckInput_MemLen(BN_Src2);

	if( BN_Src1->Length==0 )	return BN_Copy(BN_Dst, BN_Src2);
	if( BN_Src2->Length==0 )	return BN_Copy(BN_Dst, BN_Src1);

	//
	if( BN_Src1->Length>=BN_Src2->Length ) {
		tmp = BN_Src2->Length;
		BN_Dst->Length = BN_Src1->Length;
		CheckOutput_MemLen(BN_Dst);
		carry = bn_Add(BN_Dst->pData, BN_Src1->pData, BN_Src1->Length,
									 BN_Src2->pData, tmp);
	}
	else {
		tmp = BN_Src1->Length;
		BN_Dst->Length = BN_Src2->Length;
		CheckOutput_MemLen(BN_Dst);
		carry = bn_Add(BN_Dst->pData, BN_Src2->pData, BN_Src2->Length,
									 BN_Src1->pData, tmp);
	}

	if( carry ) {
		BN_Dst->pData[BN_Dst->Length++] = carry;
		CheckOutput_MemLen(BN_Dst);
	}

	//
	CheckMostSignificantDIGIT(BN_Dst);
	return CTR_SUCCESS;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
RET_VAL KCDSA_CLASS::BN_Sub(BIG_NUM *BN_Dst, BIG_NUM *BN_Src1, BIG_NUM *BN_Src2)
{
	DWORD	tmp;
	RET_VAL	carry;

	CheckInput_MemLen(BN_Src1);
	CheckInput_MemLen(BN_Src2);

	//
	if( bn_Cmp(BN_Src1->pData, BN_Src1->Length,
			   BN_Src2->pData, BN_Src2->Length)<0 )
		return CTR_BN_NEGATIVE_RESULT;

	tmp = BN_Src2->Length;
	BN_Dst->Length = BN_Src1->Length;
	CheckOutput_MemLen(BN_Dst);
	carry = bn_Sub(BN_Dst->pData, BN_Src1->pData, BN_Src1->Length,
								 BN_Src2->pData, tmp);

	if( carry ) {
		BN_Dst->pData[BN_Dst->Length++] = carry;
		CheckOutput_MemLen(BN_Dst);
	}

	//
	CheckMostSignificantDIGIT(BN_Dst);
	return CTR_SUCCESS;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
RET_VAL KCDSA_CLASS::BN_Mul(BIG_NUM *BN_Dst, BIG_NUM *BN_Multiplicand, BIG_NUM *BN_Multiplier)
{
	CheckInput_MemLen(BN_Multiplicand);
	CheckInput_MemLen(BN_Multiplier);

	BN_Dst->Length = BN_Multiplicand->Length + BN_Multiplier->Length;
//	BN_Dst->Sign = (BN_Multiplicand->Sign==BN_Multiplier->Sign)
//							? SIGN_POS : SIGN_NEG;
	CheckOutput_MemLen(BN_Dst);

	if( (BN_Multiplicand->Length==0) || (BN_Multiplier->Length==0) ) {
		BN_Dst->Length = 0;
//		BN_Dst->Sign = SIGN_POS;
		return CTR_SUCCESS;
	}
	else if( BN_Multiplicand->Length>BN_Multiplier->Length ) {
		bn_Mul(BN_Dst->pData,
				BN_Multiplicand->pData, BN_Multiplicand->Length,
				BN_Multiplier->pData, BN_Multiplier->Length);
	}
	else {
		bn_Mul(BN_Dst->pData,
				BN_Multiplier->pData, BN_Multiplier->Length,
				BN_Multiplicand->pData, BN_Multiplicand->Length);
	}

	//
	CheckMostSignificantDIGIT(BN_Dst);
	return CTR_SUCCESS;
}

/*************** Function *************************************************
*	Long Number Division : BN_Res <- BN_Dividend div BN_Divisor
*/

RET_VAL KCDSA_CLASS::BN_Div(BIG_NUM *BN_Quotient, BIG_NUM *BN_Remainder,
			  BIG_NUM *BN_Dividend, BIG_NUM *BN_Divisor)
{
	DIGIT	tmp;
	DIGIT		bnTmp[MaxDIGIT+2];

	CheckInput_MemLen(BN_Dividend);
	CheckInput_MemLen(BN_Divisor);

	if( BN_Quotient==NULL ) {
		BN_Remainder->Length = BN_Divisor->Length;
//		BN_Remainder->Sign = BN_Dividend->Sign;
		CheckOutput_MemLen(BN_Remainder);

		//
		tmp = bn_Div(bnTmp, BN_Remainder->pData,
					 BN_Dividend->pData, BN_Dividend->Length,
					 BN_Divisor->pData, BN_Divisor->Length);
	}
	else if( BN_Remainder==NULL ) {
		BN_Quotient->Length = BN_Dividend->Length - BN_Divisor->Length + 1;
//		BN_Quotient->Sign = (BN_Dividend->Sign==BN_Divisor->Sign)
//									? SIGN_POS : SIGN_NEG;
		CheckOutput_MemLen(BN_Quotient);

		//
		tmp = bn_Div(BN_Quotient->pData, bnTmp,
					 BN_Dividend->pData, BN_Dividend->Length,
					 BN_Divisor->pData, BN_Divisor->Length);
	}
	else {
		BN_Quotient->Length = BN_Dividend->Length - BN_Divisor->Length + 1;
//		BN_Quotient->Sign = (BN_Dividend->Sign==BN_Divisor->Sign)
//									? SIGN_POS : SIGN_NEG;
		CheckOutput_MemLen(BN_Quotient);
		BN_Remainder->Length = BN_Divisor->Length;
//		BN_Remainder->Sign = BN_Dividend->Sign;
		CheckOutput_MemLen(BN_Remainder);

		//
		tmp = bn_Div(BN_Quotient->pData, BN_Remainder->pData,
					 BN_Dividend->pData, BN_Dividend->Length,
					 BN_Divisor->pData, BN_Divisor->Length);
	}

	//
	CheckMostSignificantDIGIT(BN_Quotient);
	return CTR_SUCCESS;
}

//########################################
//	BIG_NUM의 Modulus 연산 함수
//########################################

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/


RET_VAL KCDSA_CLASS::BN_ModAdd(BIG_NUM *BN_Dst, BIG_NUM *BN_Src1, BIG_NUM *BN_Src2,
				 BIG_NUM *BN_Modulus)
{
	DIGIT	tmp;

	if( (BN_Cmp(BN_Src1, BN_Modulus)>=0) || (BN_Cmp(BN_Src2, BN_Modulus)>=0) )
		return ERROR_OverModulus;

	if( BN_Src1->Length>=BN_Src2->Length ) {
		BN_Dst->Length = BN_Src1->Length;
		tmp = bn_Add(BN_Dst->pData, BN_Src1->pData, BN_Src1->Length,
									 BN_Src2->pData, BN_Src2->Length);
	}
	else {
		BN_Dst->Length = BN_Src2->Length;
		tmp = bn_Add(BN_Dst->pData, BN_Src2->pData, BN_Src2->Length,
									 BN_Src1->pData, BN_Src1->Length);
	}

	if( tmp )
		BN_Dst->pData[BN_Dst->Length++] = tmp;

	if( bn_Cmp(BN_Dst->pData, BN_Dst->Length,
			   BN_Modulus->pData, BN_Modulus->Length)>=0 ) {
		bn_Sub(BN_Dst->pData, BN_Dst->pData, BN_Dst->Length,
							   BN_Modulus->pData, BN_Modulus->Length);
	}

	//
	CheckMostSignificantDIGIT(BN_Dst);
	return CTR_SUCCESS;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
RET_VAL KCDSA_CLASS::BN_ModSub(BIG_NUM *BN_Dst, BIG_NUM *BN_Src1, BIG_NUM *BN_Src2,
				 BIG_NUM *BN_Modulus)
{
	DIGIT	tmp;

	if( (BN_Cmp(BN_Src1, BN_Modulus)>=0) || (BN_Cmp(BN_Src2, BN_Modulus)>=0) )
		return ERROR_OverModulus;

	if( bn_Cmp(BN_Src1->pData, BN_Src1->Length,
			   BN_Src2->pData, BN_Src2->Length)>=0 ) {
		BN_Dst->Length = BN_Src1->Length;
		tmp = bn_Sub(BN_Dst->pData, BN_Src1->pData, BN_Src1->Length,
									 BN_Src2->pData, BN_Src2->Length);
	}
	else {
BN_Dst->Length = BN_Modulus->Length;
		bn_Add(BN_Dst->pData, BN_Modulus->pData, BN_Modulus->Length,
							   BN_Src1->pData, BN_Src1->Length);
		bn_Sub(BN_Dst->pData, BN_Dst->pData, BN_Dst->Length,
							   BN_Src2->pData, BN_Src2->Length);
	}

	//
	CheckMostSignificantDIGIT(BN_Dst);
	return CTR_SUCCESS;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
RET_VAL KCDSA_CLASS::BN_ModRed(BIG_NUM *BN_Dst, BIG_NUM *BN_Src, BIG_NUM *BN_Modulus)
{
	DWORD		i;
	DIGIT		Value[2*(MaxDIGIT+2)];
	RET_VAL		ret;

	//
	if( BN_Cmp(BN_Src, BN_Modulus)<0 )
		return BN_Copy(BN_Dst, BN_Src);

	//
	for( i=0; i<BN_Src->Length; i++)	Value[i] = BN_Src->pData[i];

	ret = Classical_REDC(Value, BN_Src->Length,
					BN_Modulus->pData, BN_Modulus->Length);
	if( ret!=CTR_SUCCESS )	return ret;

	for( i=0; i<BN_Modulus->Length; i++)	BN_Dst->pData[i] = Value[i];

	//	
	BN_Dst->Length = BN_Modulus->Length;
	CheckMostSignificantDIGIT(BN_Dst);
	return CTR_SUCCESS;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
RET_VAL KCDSA_CLASS::BN_ModMul(BIG_NUM *BN_Dst, BIG_NUM *BN_Src1, BIG_NUM *BN_Src2,
				 BIG_NUM *BN_Modulus)
{
	DWORD		i;
	RET_VAL		ret;
	DIGIT		Value[2*(MaxDIGIT+2)];

	//	Long Number Multiple Part
	bn_Mul(Value, BN_Src1->pData, BN_Src1->Length,
				  BN_Src2->pData, BN_Src2->Length);

	//	Long Number Reduction Part
	ret = Classical_REDC(Value, BN_Src1->Length+BN_Src2->Length,
					BN_Modulus->pData, BN_Modulus->Length);
	if( ret!=CTR_SUCCESS )	return ret;

	//	Long Number Saving Part
	BN_Dst->Length = BN_Modulus->Length;
//	BN_Dst->Sign = (BN_Src1->Sign==BN_Src2->Sign) ? SIGN_POS : SIGN_NEG;
	for( i=0; i<BN_Modulus->Length; i++)	BN_Dst->pData[i] = Value[i];

	//	
	CheckMostSignificantDIGIT(BN_Dst);

	//
	return CTR_SUCCESS;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
RET_VAL KCDSA_CLASS::BN_ModInv(BIG_NUM *BN_Dst, BIG_NUM *BN_Src, BIG_NUM *BN_Modulus)
{
	DWORD		i;
	RET_VAL		ret;
	DIGIT		BN_Temp[MaxDIGIT+2];

	//
	for( i=0; i<BN_Src->Length; i++)		BN_Temp[i] = BN_Src->pData[i];
	for(  ; i<BN_Modulus->Length; i++)		BN_Temp[i] = 0;

	BN_Dst->Length = BN_Modulus->Length;
//	BN_Dst->Sign = BN_Modulus->Sign;
	ret = bn_Euclid(BN_Dst->pData, BN_Temp,
				  BN_Modulus->pData, BN_Modulus->Length);
	CheckMostSignificantDIGIT(BN_Dst);

	//
	return ret;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/

RET_VAL KCDSA_CLASS::BN_ModExp(BIG_NUM *BN_Dst, BIG_NUM *BN_Base, BIG_NUM *BN_Exponent,
				 BIG_NUM *BN_Modulus)
{
	RET_VAL	ret=CTR_SUCCESS;

	//	
	ret = bn_ModExp(BN_Dst->pData, BN_Base->pData, BN_Base->Length,
					BN_Exponent->pData, BN_Exponent->Length,
					BN_Modulus->pData, BN_Modulus->Length);
	if( ret!=CTR_SUCCESS )	return ret;

	//	
	BN_Dst->Length = BN_Modulus->Length;
	CheckMostSignificantDIGIT(BN_Dst);
	return CTR_SUCCESS;
}

//########################################
//	기타 보조연산 함수
//########################################

/*************** Function *************************************************
*	Classical Modular Reduction Algorithm
*		L_Dst[ModLen-1..0] <- L_Dst[DstLen-1..0]
*								   mod L_Modulus[ModLen-1..0]
*	** Assume L_Dst[] has at least (DstLen+1)-DIGIT memory **
*/
RET_VAL KCDSA_CLASS::Classical_REDC(DIGIT *L_Dst, DWORD DstLen,
					   DIGIT *L_Modulus, DWORD ModLen)
{
	DWORD	i;
	DIGIT	MSB=0, TTTT=0, FLAG=0,
			D_Quotient,			//	Estimated quotient
			MSD_Modulus;			//	Most Significant DIGIT of L_Modulus

	//	Step 1 :
	if( DstLen<ModLen )
		return CTR_SUCCESS;

	//	Step 1.5 :
	if( L_Dst[DstLen-1]>=L_Modulus[ModLen-1] ) {
		FLAG++;
		TTTT = L_Dst[DstLen];
		L_Dst[DstLen++] = 0;
	}

	//	Step 2 : set MSB
	for( i=BitsInDIGIT-1; i!=(DWORD)-1; i--) {
		if( L_Modulus[ModLen-1]&((DIGIT)1<<i) )
			break;
		MSB++;
	}
	if( MSB ) {
		bn_SHL(L_Modulus, L_Modulus, ModLen, MSB);
		bn_SHL(L_Dst, L_Dst, DstLen, MSB);
	}

	//	Step 2 : main part
	MSD_Modulus = L_Modulus[ModLen-1];
	for( i=DstLen-ModLen-1; i!=(DWORD)-1; i--) {
		//	Step 2-1 : Estimate D_Quotient
		if( L_Dst[ModLen+i]==MSD_Modulus )
			D_Quotient = (DIGIT)-1;
//		else if( MSD_Modulus==(DIGIT)-1 )
//			D_Quotient = L_Dst[ModLen+i] + 1;
		else
			D_Quotient = DD_Div_D(L_Dst[ModLen+i], L_Dst[ModLen+i-1], MSD_Modulus);

		//	Step 2-2 : Make L_Dst <- L_Dst-D_Quotient*L_Modulus
		if( bn_MulSub(L_Dst+i, ModLen+1, L_Modulus, ModLen, D_Quotient) )
			if( bn_Add(L_Dst+i, L_Dst+i, ModLen+1, L_Modulus, ModLen)==0 )
				bn_Add(L_Dst+i, L_Dst+i, ModLen+1, L_Modulus, ModLen);
	}

	//	Step 4 : inverse part of Step 2
	if( MSB ) {
		bn_SHR(L_Modulus, L_Modulus, ModLen, MSB);
		bn_SHR(L_Dst, L_Dst, ModLen, MSB);
	}

	//	Step 4.5 : inverse part of Step 1.5
	if( FLAG ) {
		DstLen--;
		L_Dst[DstLen] = TTTT;
	}

	//
	return CTR_SUCCESS;
}


RET_VAL KCDSA_CLASS::Montgomery_Init(DIGIT *L_Modulus, DWORD ModLen)
{
	DIGIT		i;
	DIGIT		T[2*(MaxDIGIT+2)];

	//	Calculate N[0]^{-1} mod (b=2^BitsInDigit)
	Montgo_Inv = D_Inv( (DIGIT)(0-L_Modulus[0]) );

	//	Compute MEMORY_Montgo = b^{2*ModLen} (mod N)
	for( i=0; i<2*ModLen; i++) 			//	T <- b^{2*ModLen}
		T[i] = 0;
	T[i] = 1;
	Classical_REDC(T, 2*ModLen+1, L_Modulus, ModLen);
												//	T <- T (mod N)
	bn_Copy(Montgo_Rto2modN, T, ModLen);	//	MEMORY_Montgo <- T

	//
	return CTR_SUCCESS;
}


/*************** Function *************************************************
*	Change number system Zn to RZn
*/
RET_VAL KCDSA_CLASS::Montgomery_Zn2RZn(DIGIT *L_Dst, DIGIT *L_Src,
						  DIGIT *L_Modulus, DWORD ModLen)
{
	RET_VAL		ret;
	DIGIT		T[2*(MaxDIGIT+2)];

	//	main part
	bn_KaraMul(T, L_Src, Montgo_Rto2modN, ModLen);
	ret = Montgomery_REDC(T, 2*ModLen, L_Modulus, ModLen);			GOTO_END;

	bn_Copy(L_Dst, T, ModLen);

	//
	ret = CTR_SUCCESS;
LABEL_END:
	return ret;
}


/*************** Function ************************************************/
RET_VAL KCDSA_CLASS::MillerRabin(
		BIG_NUM		*BN_Num)
{
	DWORD		s, i, j, NoTest, DigitLen=BN_Num->Length;
	DIGIT		tmp;
	RET_VAL		ret;
	BIG_NUM		*BN_Num_1=NULL, *BN_Tmp=NULL, *T=NULL, *M=NULL;

	//
	ret = CTR_VERIFY_FAIL;
	if( BN_Num->Length==0 )			goto LABEL_END;
	if( isEven0(BN_Num->pData) )	goto LABEL_END;

	//	Trivial Division
	for( i=0; SmallPrimes[i]!=0; i++) {
		tmp = bn_ModD(BN_Num->pData, DigitLen, SmallPrimes[i]);
		tmp = D_Gcd(SmallPrimes[i], tmp);
		if( tmp!=1 )	goto LABEL_END;
	}

	//
	j = BitsInDIGIT * DigitLen;
	for( i=0;  ; i++) {
		NoTest = IterNo[i][1];
		if( j<IterNo[i][0] )	break;
	}

	//
	ret = CTR_MEMORY_ALLOC_ERROR;

	if( (BN_Num_1=CreateBigNum(DigitLen+1))==NULL )	goto LABEL_END;
	if( (BN_Tmp=CreateBigNum(DigitLen+1))==NULL )	goto LABEL_END;
	if( (T=CreateBigNum(DigitLen+1))==NULL )		goto LABEL_END;
	if( (M=CreateBigNum(DigitLen+1))==NULL )		goto LABEL_END;

	//
	ret = BN_Sub(BN_Num_1, BN_Num, &BN_One);		GOTO_END;

	//	Compute s, T satisfing BN_Num-1 = T * 2^s with T odd
	ret = BN_Copy(T, BN_Num_1);						GOTO_END;
	for( s=0; isEven0(T->pData); s++) {
		ret = BN_SHR(T, T, 1);						GOTO_END;
	}

	//
	for( i=0; i<=NoTest; i++) {
		//
		if( i==0 ) {
			ret = BN_Copy(BN_Tmp, &BN_Two);			GOTO_END;
		}
		else {
			ret = BN_Rand(BN_Tmp, BitsInDIGIT*DigitLen-1);	GOTO_END;
		}

		//
		ret = BN_ModExp(M, BN_Tmp, T, BN_Num);		GOTO_END;

		//
		if( (BN_Cmp(M, &BN_One)==0) || (BN_Cmp(M, BN_Num_1)==0) )	continue;

		//
		for( j=0; j<s; j++) {
			ret = BN_ModMul(M, M, M, BN_Num);		GOTO_END;

			ret = CTR_VERIFY_FAIL;
			if( BN_Cmp(M, &BN_One)==0 )	goto LABEL_END;

			if( BN_Cmp(M, BN_Num_1)==0 )	break;
		}

		//
		ret = CTR_VERIFY_FAIL;
		if( s==j )	goto LABEL_END;		//return CTR_VERIFY_FAIL;
	}

	//
	ret = CTR_SUCCESS;
LABEL_END:
	if( BN_Num_1!=NULL )	DestroyBigNum(BN_Num_1);
	if( BN_Tmp!=NULL )		DestroyBigNum(BN_Tmp);
	if( T!=NULL )			DestroyBigNum(T);
	if( M!=NULL )			DestroyBigNum(M);
	return ret;
}

 void KCDSA_CLASS::HAS160_Transform(DWORD *Y, DWORD *DigestValue)
{
	DWORD	a, b, c, d, e, T[16], K;

#if defined(BIG_ENDIAN)
	DWORD	ZZ[16];

	ZZ[ 0] = ENDIAN_REVERSE_DWORD(Y[ 0]);
	ZZ[ 1] = ENDIAN_REVERSE_DWORD(Y[ 1]);
	ZZ[ 2] = ENDIAN_REVERSE_DWORD(Y[ 2]);
	ZZ[ 3] = ENDIAN_REVERSE_DWORD(Y[ 3]);
	ZZ[ 4] = ENDIAN_REVERSE_DWORD(Y[ 4]);
	ZZ[ 5] = ENDIAN_REVERSE_DWORD(Y[ 5]);
	ZZ[ 6] = ENDIAN_REVERSE_DWORD(Y[ 6]);
	ZZ[ 7] = ENDIAN_REVERSE_DWORD(Y[ 7]);
	ZZ[ 8] = ENDIAN_REVERSE_DWORD(Y[ 8]);
	ZZ[ 9] = ENDIAN_REVERSE_DWORD(Y[ 9]);
	ZZ[10] = ENDIAN_REVERSE_DWORD(Y[10]);
	ZZ[11] = ENDIAN_REVERSE_DWORD(Y[11]);
	ZZ[12] = ENDIAN_REVERSE_DWORD(Y[12]);
	ZZ[13] = ENDIAN_REVERSE_DWORD(Y[13]);
	ZZ[14] = ENDIAN_REVERSE_DWORD(Y[14]);
	ZZ[15] = ENDIAN_REVERSE_DWORD(Y[15]);
#else
	DWORD	*ZZ=Y;
#endif

	T[ 0] = ZZ[ 8] ^ ZZ[ 9] ^ ZZ[10] ^ ZZ[11];
	T[ 1] = ZZ[12] ^ ZZ[13] ^ ZZ[14] ^ ZZ[15];
	T[ 2] = ZZ[ 0] ^ ZZ[ 1] ^ ZZ[ 2] ^ ZZ[ 3];
	T[ 3] = ZZ[ 4] ^ ZZ[ 5] ^ ZZ[ 6] ^ ZZ[ 7];

	T[ 4] = ZZ[11] ^ ZZ[14] ^ ZZ[ 1] ^ ZZ[ 4];
	T[ 5] = ZZ[ 7] ^ ZZ[10] ^ ZZ[13] ^ ZZ[ 0];
	T[ 6] = ZZ[ 3] ^ ZZ[ 6] ^ ZZ[ 9] ^ ZZ[12];
	T[ 7] = ZZ[15] ^ ZZ[ 2] ^ ZZ[ 5] ^ ZZ[ 8];

	T[ 8] = ZZ[ 4] ^ ZZ[13] ^ ZZ[ 6] ^ ZZ[15];
	T[ 9] = ZZ[ 8] ^ ZZ[ 1] ^ ZZ[10] ^ ZZ[ 3];
	T[10] = ZZ[12] ^ ZZ[ 5] ^ ZZ[14] ^ ZZ[ 7];
	T[11] = ZZ[ 0] ^ ZZ[ 9] ^ ZZ[ 2] ^ ZZ[11];

	T[12] = ZZ[15] ^ ZZ[10] ^ ZZ[ 5] ^ ZZ[ 0];
	T[13] = ZZ[11] ^ ZZ[ 6] ^ ZZ[ 1] ^ ZZ[12];
	T[14] = ZZ[ 7] ^ ZZ[ 2] ^ ZZ[13] ^ ZZ[ 8];
	T[15] = ZZ[ 3] ^ ZZ[14] ^ ZZ[ 9] ^ ZZ[ 4];

	//
	a=DigestValue[0];
	b=DigestValue[1];
	c=DigestValue[2];
	d=DigestValue[3];
	e=DigestValue[4];

	//	round 1
	FF(a, b, c, d, e, T[ 0],  5);
	FF(e, a, b, c, d, ZZ[ 0], 11);
	FF(d, e, a, b, c, ZZ[ 1],  7);
	FF(c, d, e, a, b, ZZ[ 2], 15);
	FF(b, c, d, e, a, ZZ[ 3],  6);
	FF(a, b, c, d, e, T[ 1], 13);
	FF(e, a, b, c, d, ZZ[ 4],  8);
	FF(d, e, a, b, c, ZZ[ 5], 14);
	FF(c, d, e, a, b, ZZ[ 6],  7);
	FF(b, c, d, e, a, ZZ[ 7], 12);
	FF(a, b, c, d, e, T[ 2],  9);
	FF(e, a, b, c, d, ZZ[ 8], 11);
	FF(d, e, a, b, c, ZZ[ 9],  8);
	FF(c, d, e, a, b, ZZ[10], 15);
	FF(b, c, d, e, a, ZZ[11],  6);
	FF(a, b, c, d, e, T[ 3], 12);
	FF(e, a, b, c, d, ZZ[12],  9);
	FF(d, e, a, b, c, ZZ[13], 14);
	FF(c, d, e, a, b, ZZ[14],  5);
	FF(b, c, d, e, a, ZZ[15], 13);

	//	round 2
	K = 0x5A827999;
	GG(a, b, c, d, e, T[ 4],  5);
	GG(e, a, b, c, d, ZZ[ 3], 11);
	GG(d, e, a, b, c, ZZ[ 6],  7);
	GG(c, d, e, a, b, ZZ[ 9], 15);
	GG(b, c, d, e, a, ZZ[12],  6);
	GG(a, b, c, d, e, T[ 5], 13);
	GG(e, a, b, c, d, ZZ[15],  8);
	GG(d, e, a, b, c, ZZ[ 2], 14);
	GG(c, d, e, a, b, ZZ[ 5],  7);
	GG(b, c, d, e, a, ZZ[ 8], 12);
	GG(a, b, c, d, e, T[ 6],  9);
	GG(e, a, b, c, d, ZZ[11], 11);
	GG(d, e, a, b, c, ZZ[14],  8);
	GG(c, d, e, a, b, ZZ[ 1], 15);
	GG(b, c, d, e, a, ZZ[ 4],  6);
	GG(a, b, c, d, e, T[ 7], 12);
	GG(e, a, b, c, d, ZZ[ 7],  9);
	GG(d, e, a, b, c, ZZ[10], 14);
	GG(c, d, e, a, b, ZZ[13],  5);
	GG(b, c, d, e, a, ZZ[ 0], 13);

	//	round 3
	K = 0x6ED9EBA1;
	HH(a, b, c, d, e, T[ 8],  5);
	HH(e, a, b, c, d, ZZ[12], 11);
	HH(d, e, a, b, c, ZZ[ 5],  7);
	HH(c, d, e, a, b, ZZ[14], 15);
	HH(b, c, d, e, a, ZZ[ 7],  6);
	HH(a, b, c, d, e, T[ 9], 13);
	HH(e, a, b, c, d, ZZ[ 0],  8);
	HH(d, e, a, b, c, ZZ[ 9], 14);
	HH(c, d, e, a, b, ZZ[ 2],  7);
	HH(b, c, d, e, a, ZZ[11], 12);
	HH(a, b, c, d, e, T[10],  9);
	HH(e, a, b, c, d, ZZ[ 4], 11);
	HH(d, e, a, b, c, ZZ[13],  8);
	HH(c, d, e, a, b, ZZ[ 6], 15);
	HH(b, c, d, e, a, ZZ[15],  6);
	HH(a, b, c, d, e, T[11], 12);
	HH(e, a, b, c, d, ZZ[ 8],  9);
	HH(d, e, a, b, c, ZZ[ 1], 14);
	HH(c, d, e, a, b, ZZ[10],  5);
	HH(b, c, d, e, a, ZZ[ 3], 13);

	//	round 4
	K = 0x8F1BBCDC;
	II(a, b, c, d, e, T[12],  5);
	II(e, a, b, c, d, ZZ[ 7], 11);
	II(d, e, a, b, c, ZZ[ 2],  7);
	II(c, d, e, a, b, ZZ[13], 15);
	II(b, c, d, e, a, ZZ[ 8],  6);
	II(a, b, c, d, e, T[13], 13);
	II(e, a, b, c, d, ZZ[ 3],  8);
	II(d, e, a, b, c, ZZ[14], 14);
	II(c, d, e, a, b, ZZ[ 9],  7);
	II(b, c, d, e, a, ZZ[ 4], 12);
	II(a, b, c, d, e, T[14],  9);
	II(e, a, b, c, d, ZZ[15], 11);
	II(d, e, a, b, c, ZZ[10],  8);
	II(c, d, e, a, b, ZZ[ 5], 15);
	II(b, c, d, e, a, ZZ[ 0],  6);
	II(a, b, c, d, e, T[15], 12);
	II(e, a, b, c, d, ZZ[11],  9);
	II(d, e, a, b, c, ZZ[ 6], 14);
	II(c, d, e, a, b, ZZ[ 1],  5);
	II(b, c, d, e, a, ZZ[12], 13);

	//	chaining variables update
	DigestValue[0] += a;
	DigestValue[1] += b;
	DigestValue[2] += c;
	DigestValue[3] += d;
	DigestValue[4] += e;
}

/*************** Function *************************************************
*	
*/
void	KCDSA_CLASS::HAS160_Init(
		HAS160_ALG_INFO	*AlgInfo)
{
	//
	AlgInfo->ChainVar[0] = 0x67452301;
	AlgInfo->ChainVar[1] = 0xefcdab89;
	AlgInfo->ChainVar[2] = 0x98badcfe;
	AlgInfo->ChainVar[3] = 0x10325476;
	AlgInfo->ChainVar[4] = 0xC3D2E1F0;

	//
	AlgInfo->Count[0] = AlgInfo->Count[1] = 0;
}

/*************** Function *************************************************
*	
*/
void	KCDSA_CLASS::HAS160_Update(
		HAS160_ALG_INFO	*AlgInfo,
		BYTE			*Message,		//	input Message
		DWORD			MessageLen)		//	in BYTEs
{
	DWORD		RemainedLen, PartLen;

	//	Compute the number of hashed bytes mod HAS160_DIGEST_BLOCKLEN
	RemainedLen = (AlgInfo->Count[0] >> 3) % HAS160_DIGEST_BLOCKLEN;
	//	compute the number of bytes that can be filled up
	PartLen = HAS160_DIGEST_BLOCKLEN - RemainedLen;

	//	Update Count (number of toatl data bits)
	if( (AlgInfo->Count[0] += (MessageLen << 3)) < AlgInfo->Count[0] )
		AlgInfo->Count[1]++;
	AlgInfo->Count[1] += (MessageLen >> 29);

	//	핵심 updtae 부분
	if( MessageLen>=PartLen ) {
		memcpy(AlgInfo->Buffer+RemainedLen, Message, (int)PartLen);
		HAS160_Transform((DWORD *)AlgInfo->Buffer, AlgInfo->ChainVar);

		Message += PartLen;
		MessageLen -= PartLen;
		RemainedLen = 0;

		while( MessageLen>=HAS160_DIGEST_BLOCKLEN ) {
			if( (((int)Message)%4)==0 ) {	//	Speed up technique
				HAS160_Transform((DWORD *)Message, AlgInfo->ChainVar);
			}
			else {
				memcpy((BYTE *)AlgInfo->Buffer, Message, (int)HAS160_DIGEST_BLOCKLEN);
				HAS160_Transform((DWORD *)AlgInfo->Buffer, AlgInfo->ChainVar);
			}
			Message += HAS160_DIGEST_BLOCKLEN;
			MessageLen -= HAS160_DIGEST_BLOCKLEN;
		}
	}

	//	Buffer remaining input
	memcpy((BYTE *)AlgInfo->Buffer+RemainedLen, Message, (int)MessageLen);
}

/*************** Function *************************************************
*	
*/
void	KCDSA_CLASS::HAS160_Final(
		HAS160_ALG_INFO	*AlgInfo,
		BYTE			*Digest)	//	output Hash Value
{
	DWORD		i, dwIndex, CountL, CountH;

	//	마지박 블록 처리
	CountL = AlgInfo->Count[0];
	CountH = AlgInfo->Count[1];
	dwIndex = (CountL >> 3) % HAS160_DIGEST_BLOCKLEN;
	AlgInfo->Buffer[dwIndex++] = 0x80;

	if(dwIndex>HAS160_DIGEST_BLOCKLEN-8) {
		memset((BYTE *)AlgInfo->Buffer + dwIndex, 0, (int)(HAS160_DIGEST_BLOCKLEN-dwIndex));

		HAS160_Transform((DWORD *)AlgInfo->Buffer, AlgInfo->ChainVar);

		memset((BYTE *)AlgInfo->Buffer, 0, (int)HAS160_DIGEST_BLOCKLEN-8);
	}
	else
		memset((BYTE *)AlgInfo->Buffer+dwIndex, 0, (int)(HAS160_DIGEST_BLOCKLEN-dwIndex-8));

	//	출력
#if defined(BIG_ENDIAN)
	CountL = ENDIAN_REVERSE_DWORD(CountL);
	CountH = ENDIAN_REVERSE_DWORD(CountH);
#endif
	((DWORD *)AlgInfo->Buffer)[HAS160_DIGEST_BLOCKLEN/4-2] = CountL;
	((DWORD *)AlgInfo->Buffer)[HAS160_DIGEST_BLOCKLEN/4-1] = CountH;

	HAS160_Transform((DWORD *)AlgInfo->Buffer, AlgInfo->ChainVar);

	for( i=0; i<HAS160_DIGEST_VALUELEN; i+=4)
		LITTLE_D2B((AlgInfo->ChainVar)[i/4], &(Digest[i]));
}

RET_VAL KCDSA_CLASS::KCDSA_CreateKeyObject(
		BN_KCDSA_KEY	**KCDSA_Params)	//	출력
{
	DWORD		tt;
	BN_KCDSA_KEY	*BN_Key;

	//	Step 0 : KCDSA_Params값 확인.
	if( KCDSA_Params==NULL )
		return CTR_INVALID_POINTER;

	//	Step 1 : BN_KCDSA_KEY 형식 데이타의 메모리 할당.
	*KCDSA_Params = BN_Key = (BN_KCDSA_KEY *) malloc(sizeof(BN_KCDSA_KEY));
	if( BN_Key==NULL )		goto LABEL_END0;
	memset((BYTE *)BN_Key, 0, sizeof(BN_KCDSA_KEY));

	//	Step 2 : BN_KCDSA_KEY 형식 데이타의 세부 항목 메모리 할당.
	tt = (BN_MAX_BITS-1)/BitsInDIGIT+1;
	if( (BN_Key->KCDSA_p=CreateBigNum(tt+1))==NULL )	goto LABEL_END0;
	if( (BN_Key->KCDSA_g=CreateBigNum(tt+1))==NULL )	goto LABEL_END0;
	if( (BN_Key->KCDSA_y=CreateBigNum(tt+1))==NULL )	goto LABEL_END0;
	tt = (256-1)/BitsInDIGIT+1;
	if( (BN_Key->KCDSA_q=CreateBigNum(tt+1))==NULL )	goto LABEL_END0;
	if( (BN_Key->KCDSA_x=CreateBigNum(tt+1))==NULL )	goto LABEL_END0;
	if( (BN_Key->KCDSA_z = (unsigned char *)malloc(512/8))==NULL )			goto LABEL_END0;
	BN_Key->Count = 0;
	BN_Key->SeedLen = MAX_SEED_LEN;
	if( (BN_Key->Seed = (unsigned char *)malloc(MAX_SEED_LEN))==NULL )		goto LABEL_END0;

	//	Step 3 : 성공적을 종료함.
	return CTR_SUCCESS;

	//	Step 4 : Error 발생.
LABEL_END0:
	KCDSA_DestroyKeyObject(KCDSA_Params);
	return CTR_MEMORY_ALLOC_ERROR;
}

/**************************************************************************
*
*	Function Description
*		BN_KCDSA_KEY 데이타의 메모리 소거.
*
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*		- CTR_INVALID_POINTER			KCDSA_Params값이 NULL임.
*/
RET_VAL KCDSA_CLASS::KCDSA_DestroyKeyObject(
		BN_KCDSA_KEY	**KCDSA_Params)	//	출력
{
	BN_KCDSA_KEY	*BN_Key=*KCDSA_Params;

	//	Step 0 : KCDSA_Params값 확인.
	if( KCDSA_Params==NULL )	return CTR_INVALID_POINTER;
	if( *KCDSA_Params==NULL )	return CTR_SUCCESS;

	//	Step 1 : BN_KCDSA_KEY 형식 데이타의 세부 항목 메모리 소거.
	if( BN_Key->KCDSA_p!=NULL )		DestroyBigNum(BN_Key->KCDSA_p);
	if( BN_Key->KCDSA_q!=NULL )		DestroyBigNum(BN_Key->KCDSA_q);
	if( BN_Key->KCDSA_g!=NULL )		DestroyBigNum(BN_Key->KCDSA_g);
	if( BN_Key->KCDSA_x!=NULL )		DestroyBigNum(BN_Key->KCDSA_x);
	if( BN_Key->KCDSA_y!=NULL )		DestroyBigNum(BN_Key->KCDSA_y);
	if( BN_Key->KCDSA_z!=NULL ) {
		memset(BN_Key->KCDSA_z, 0, 512/8);
		free(BN_Key->KCDSA_z);
	}
	if( BN_Key->Seed!=NULL ) {
		memset(BN_Key->Seed, 0, MAX_SEED_LEN);
		free(BN_Key->Seed);
	}

	// Step 2 : BN_KCDSA_KEY 형식 데이타의 메모리 소거.
	memset((BYTE *)BN_Key, 0, sizeof(BN_KCDSA_KEY));
	free(BN_Key);
	*KCDSA_Params = NULL;

	//	Step 3 : 성공적을 종료함.
	return CTR_SUCCESS;
}

/**************************************************************************
*
*	Function Description
*		파일로부터 도메인 파라미터/개인키/공개키를 읽어서
*		BN_KCDSA_KEY 데이타에 저장함.
*
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
RET_VAL KCDSA_CLASS::KCDSA_GetKeyData(
		char			*filename,
		DWORD			KeyType,
		BN_KCDSA_KEY	*KCDSA_Params)	//	출력
{
	#define GetBigNum(pfile, BN_Tmp, DataLen) {	\
		BN_Tmp->Length = DataLen;				\
		for( i=DataLen-1; i!=(DWORD)-1; i--) {	\
			fscanf(pfile, " %X", &tt);			\
			BN_Tmp->pData[i] = tt;				\
		}										\
	}
	#define GetBYTE(pfile, Data, DataLen) {		\
		for( i=0; i<DataLen; i++) {				\
			fscanf(pfile, " %X", &tt);			\
			Data[i] = (BYTE) tt;				\
		}										\
	}

	FILE	*pfile;
	char	ch, str[16];
	DWORD	i, tt, Len, Flag;

	//
	Flag = 0;

	//	Step 1 : 파일 열기
	if( (pfile=fopen(filename, "r"))==NULL ) {
		printf( "The file '%s' was not opened\n", filename);
		return CTR_KEYFILE_ERROR;
	}

	//	Step 2 : 정보를 읽어들임.
	for(  ;  ;  ) {
		for(  ;  ;  ) {
			ch = (BYTE) getc(pfile);
			if( ch==EOF )	break;
			if( ch=='#' )	break;
		}
		if( ch==EOF )	break;

		fscanf(pfile, "%s (%d) =", str, &Len);
		if( strcmp(str, "KCDSA_p")==0 ) {
			Flag |= 0x40;
			GetBigNum(pfile, KCDSA_Params->KCDSA_p, Len);
		}
		else if( strcmp(str, "KCDSA_q")==0 ) {
			Flag |= 0x20;
			GetBigNum(pfile, KCDSA_Params->KCDSA_q, Len);
		}
		else if( strcmp(str, "KCDSA_g")==0 ) {
			Flag |= 0x10;
			GetBigNum(pfile, KCDSA_Params->KCDSA_g, Len);
		}
		else if( strcmp(str, "KCDSA_x")==0 ) {
			Flag |= 0x04;
			GetBigNum(pfile, KCDSA_Params->KCDSA_x, Len);
		}
		else if( strcmp(str, "KCDSA_y")==0 ) {
			Flag |= 0x02;
			GetBigNum(pfile, KCDSA_Params->KCDSA_y, Len);
		}
		else if( strcmp(str, "KCDSA_z")==0 ) {
			Flag |= 0x01;
			GetBYTE(pfile, KCDSA_Params->KCDSA_z, Len);
		}
		else if( strcmp(str, "KCDSA_Count")==0 ) {
			fscanf(pfile, " %d", &tt);
			KCDSA_Params->Count = tt;
		}
		else if( strcmp(str, "KCDSA_Seed")==0 ) {
			KCDSA_Params->SeedLen = Len;
			GetBYTE(pfile, KCDSA_Params->Seed, Len);
		}
		else
			return CTR_KEYFILE_ERROR;
	}

	//	Step 3 : 도메인 파라미터/개인키/공개키중 KeyType으로 지시된
	//			 형식의 정보가 모두 입력되었는지 확인.
	if( KeyType==KI_DOMAIN_PARAMS ) {
		if( (Flag&0x70)!=0x70 )	return CTR_KEYFILE_ERROR;
	}
	else if( KeyType==KI_PRIVATE_KEY ) {
		if( (Flag&0x75)!=0x75 )	return CTR_KEYFILE_ERROR;
	}
	else if( KeyType==KI_PUBLIC_KEY ) {
		if( (Flag&0x73)!=0x73 )	return CTR_KEYFILE_ERROR;
	}
	else
		return CTR_KEYFILE_ERROR;

	//	Step 4 : 파일을 닫고 종료함.
	fclose(pfile);
	return CTR_SUCCESS;
}

/**************************************************************************
*
*	Function Description
*		BN_KCDSA_KEY 데이타에서 도메인 파라미터/개인키/공개키의 정보를
*		파일에 저장함.
*
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
RET_VAL KCDSA_CLASS::KCDSA_SaveKeyData(
		char			*filename,
		DWORD			KeyType,
		BN_KCDSA_KEY	*KCDSA_Params)	//	출력
{
	#define SaveBigNum(pfile, msg, BN_Tmp) {				\
		fprintf(pfile, "#%s (%d) =", msg, BN_Tmp->Length);	\
		for( i=BN_Tmp->Length-1; i!=(DWORD)-1; i--)			\
			fprintf(pfile, " 0x%.8X", BN_Tmp->pData[i]);	\
		if( BN_Tmp->Length==0 )	fprintf(pfile, "0");		\
		fprintf(pfile, "\n");								\
	}
	#define SaveBYTE(pfile, msg, Data, DataLen) {	\
		fprintf(pfile, "#%s (%d) =", msg, DataLen);	\
		for( i=0; i<DataLen; i++)					\
			fprintf(pfile, " 0x%.2X", Data[i]);		\
		fprintf(pfile, "\n");						\
	}

	FILE	*pfile;
	DWORD	i;

	//	Step 1 : 파일 열기
	if( (pfile=fopen(filename, "w"))==NULL ) {
		printf( "The file '%s' was not opened\n", filename);
		return CTR_KEYFILE_ERROR;
	}

	//	Step 2 : 정해진 규칙에 따라 정보를 파일에 기록함.
	if( KeyType==KI_DOMAIN_PARAMS )
		fprintf(pfile, "KCDSA Domain Parameters\n\n");
	else if( KeyType==KI_PRIVATE_KEY )
		fprintf(pfile, "KCDSA Private Key\n\n");
	else if( KeyType==KI_PUBLIC_KEY )
		fprintf(pfile, "KCDSA Public Key\n\n");
	else
		return CTR_KEYFILE_ERROR;

	//
	if( (KeyType==KI_DOMAIN_PARAMS)
		|| (KeyType==KI_PRIVATE_KEY)
		|| (KeyType==KI_PUBLIC_KEY) ) {
		SaveBigNum(pfile, "KCDSA_p", KCDSA_Params->KCDSA_p);
		SaveBigNum(pfile, "KCDSA_q", KCDSA_Params->KCDSA_q);
		SaveBigNum(pfile, "KCDSA_g", KCDSA_Params->KCDSA_g);
		fprintf(pfile, "#KCDSA_Count (1) = %d\n", KCDSA_Params->Count);
		SaveBYTE(pfile, "KCDSA_Seed", KCDSA_Params->Seed, KCDSA_Params->SeedLen);
	}
	if( KeyType==KI_PRIVATE_KEY ) {
		SaveBigNum(pfile, "KCDSA_x", KCDSA_Params->KCDSA_x);
		SaveBYTE(pfile, "KCDSA_z", KCDSA_Params->KCDSA_z, 512/8);
	}
	else if( KeyType==KI_PUBLIC_KEY ) {
		SaveBigNum(pfile, "KCDSA_y", KCDSA_Params->KCDSA_y);
		SaveBYTE(pfile, "KCDSA_z", KCDSA_Params->KCDSA_z, 512/8);
	}

	//	Step 4 : 파일을 닫고 종료함.
	fclose(pfile);
	return CTR_SUCCESS;
}

/**************************************************************************
*
*	Function Description
*		KCDSA의 도메인 파라미터 생성시에 사용하는 간단한 형태의
*		random number generator.
*		pbDst <- (... || HAS160(pbSrc||0x01) || HAS160(pbSrc||0x00))의
*				우측 dDstBitLen비트
*
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*/
 RET_VAL KCDSA_CLASS::KCDSA_PRNG(
		HAS160_ALG_INFO	*HAS160_AlgInfo,
		BYTE		*pbSrc,
		DWORD		dSrcByteLen,	//	in Bytes
		BYTE		*pbDst,
		DWORD		dDstBitLen)		//	in Bits
{
	BYTE		Count, DigestValue[HAS160_DIGEST_VALUELEN];
	DWORD		i;

	//
	i = ((dDstBitLen+7) & 0xFFFFFFF8) / 8;
	for( Count=0;  ; Count++) {
		HAS160_Init(HAS160_AlgInfo);
		HAS160_Update(HAS160_AlgInfo, pbSrc, dSrcByteLen);
		HAS160_Update(HAS160_AlgInfo, &Count, 1);
		HAS160_Final(HAS160_AlgInfo, DigestValue);

		if( i>=HAS160_DIGEST_VALUELEN ) {
			i -= HAS160_DIGEST_VALUELEN;
			memcpy(pbDst+i, DigestValue, HAS160_DIGEST_VALUELEN);
			if( i==0 )	break;
		}
		else {
			memcpy(pbDst, DigestValue+HAS160_DIGEST_VALUELEN-i, i);
			break;
		}
	}

	//
	i = dDstBitLen & 0x07;
	if( i )
		pbDst[0] &= (1<<i) - 1;

	//
	return CTR_SUCCESS;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*		- CTR_INVALID_POINTER			KCDSA_Params값이 NULL임.
*		- CTR_INVALID_ALG_PARAMS		PrimeBits/SubPrimeBits이 부적절함.
*		- CTR_MEMORY_ALLOC_ERROR		메모리 할당에서 문제가 있음.
*/
RET_VAL KCDSA_CLASS::KCDSA_GenerateParameters(
		DWORD			PrimeBits,
		DWORD			SubPrimeBits,
		BN_KCDSA_KEY	*KCDSA_Params)	//	출력
{
	BYTE		bzTmp[2048/8+4], tSeed[256/8];
	DWORD		i, Count;
	RET_VAL		ret;
	BIG_NUM		*BN_Tmp1=NULL, *BN_Tmp2=NULL;
	HAS160_ALG_INFO	AlgInfo;

	//	Step 0 : 입력 pointer가 NULL인지 확인
	if( KCDSA_Params==NULL )	return CTR_INVALID_POINTER;

	//	Step 0 : 입력된 PrimeBits, SubPrimeBits의 값이 적절한 지를 확인한다.
	if( (PrimeBits<1024) || (PrimeBits>2048) || (PrimeBits%256) )
		return CTR_INVALID_ALG_PARAMS;
	if( (SubPrimeBits<160) || (SubPrimeBits>256) || (SubPrimeBits%32) )
		return CTR_INVALID_ALG_PARAMS;

	//	Step 0 : KCDSA_Params의 p, q, g, seed에 대한 메모리 할당이 적절하다고 가정.

	//	Initialize Temporary BigNumber
	ret = CTR_MEMORY_ALLOC_ERROR;
	if( (BN_Tmp1=CreateBigNum(PrimeBits/8+1))==NULL )			goto LABEL_END;
	if( (BN_Tmp2=CreateBigNum(PrimeBits/8+1))==NULL )			goto LABEL_END;

	////
LABEL_Start:
	for(  ;  ;  ) {
		//	Step 1. |Q| 비트 크기의 임의의 비트열 Seed를 선택한다.
		KCDSA_Params->SeedLen = SubPrimeBits/8;
		srand( (unsigned)time( NULL ) );
		for( i=0; i<SubPrimeBits/8; i++)
			tSeed[i] = (BYTE) (rand() ^ (rand()<<11));
#ifdef MyDebug
{
	BYTE	TT[] = {0x85,0xe2,0x29,0xc7,0xac,0x16,0x91,0x83,0xdc,0x07,
					0x47,0x18,0xd5,0x41,0x7e,0xb0,0x8f,0xf9,0x32,0xb2};
	memcpy(tSeed, TT, 20);
}
#endif

		//	Step 2. U = PRNG(Seed, |P|-|Q|-4)
		ret = KCDSA_PRNG(&AlgInfo, tSeed, KCDSA_Params->SeedLen,
						bzTmp, PrimeBits-SubPrimeBits-4);			GOTO_END;
		ret = OS2BN(bzTmp, (PrimeBits-SubPrimeBits)/8, BN_Tmp1);	GOTO_END;

		//	Step 3. J = 2|P|-|Q|-1 XOR U XOR 1
		SetBitDIGIT(BN_Tmp1->pData, PrimeBits-SubPrimeBits-1);
		SetBitDIGIT(BN_Tmp1->pData, 0);

		//	Step 4. J가 소수가 아니면 단계 1로 간다.
		if( MillerRabin(BN_Tmp1)==CTR_SUCCESS )		break;
	}

	memcpy(KCDSA_Params->Seed, tSeed, KCDSA_Params->SeedLen);
	//	Step 5. Count를 0으로 둔다.
	for( Count=1; Count<(1<<24); Count++) {
		//	Step 6. Count를 1증가시킨다.
		//	Step 7. Count > 2^24이면 단계 2로 간다.

		//	Step 8. U = PRNG(Seed || Count, |Q|)
		BIG_D2B(Count, (&tSeed[KCDSA_Params->SeedLen]));
		ret = KCDSA_PRNG(&AlgInfo, tSeed, KCDSA_Params->SeedLen+4,
						bzTmp, SubPrimeBits);						GOTO_END;
		ret = OS2BN(bzTmp, SubPrimeBits/8, KCDSA_Params->KCDSA_q);	GOTO_END;

		//	Step 9. Q = 2|Q|-1 XOR U XOR 1
		SetBitDIGIT(KCDSA_Params->KCDSA_q->pData, SubPrimeBits-1);
		SetBitDIGIT(KCDSA_Params->KCDSA_q->pData, 0);

		//	Step 10. P = 2JQ+1의 비트수가 |P|보다 크면 단계 6으로 간다.
		ret = BN_Mul(KCDSA_Params->KCDSA_p, BN_Tmp1,
						KCDSA_Params->KCDSA_q);						GOTO_END;
		if( CheckBitDIGIT(KCDSA_Params->KCDSA_p->pData, PrimeBits-1) )
			continue;
		ret = BN_SHL(KCDSA_Params->KCDSA_p,
						KCDSA_Params->KCDSA_p, 1);					GOTO_END;
		SetBitDIGIT(KCDSA_Params->KCDSA_p->pData, 0);

		//	Step 11. 강한 소수 판정 알고리즘으로 Q를 판정하여 소수가 아니면 단계 6으로 간다.
		if( MillerRabin(KCDSA_Params->KCDSA_q)!=CTR_SUCCESS )	continue;

		//	Step 12. 강한 소수 판정 알고리즘으로 P를 판정하여 소수가 아니면 단계 6으로 간다.
		if( MillerRabin(KCDSA_Params->KCDSA_p)==CTR_SUCCESS )	break;
	}
	if( Count==(1<<24) )	goto LABEL_Start;
	//	Step 13. 소수 P, Q, J과 증거값 Seed, Count를 출력한다. 
	KCDSA_Params->Count = Count;

	ret = BN_SHL(BN_Tmp1, BN_Tmp1, 1);								GOTO_END;
	for(  ;  ;  ) {
		//	Step 1. P보다 작은 임의의 수 U를 발생시킨다.
		for( i=0; i<PrimeBits/8; i++)
			bzTmp[i] = (BYTE) (rand() ^ (rand()<<11));

		ret = OS2BN(bzTmp, PrimeBits/8, BN_Tmp2);					GOTO_END;
		if( BN_Cmp(KCDSA_Params->KCDSA_p, BN_Tmp2)<=0 ) {
			ret = BN_Sub(BN_Tmp2, BN_Tmp2, KCDSA_Params->KCDSA_p);	GOTO_END;
		}
#ifdef MyDebug
{
	DWORD	TT[] = {0x1711797e,0xcf9bc4b8,0x1c5ad487,0xb2d9f3d4,
					0xf4de8616,0xc47bb030,0x355ea4bf,0x2ab07104,
					0x0ee59c95,0x453119d7,0x68af7a79,0x95133c2d,
					0xa1e302c6,0x9128afba,0x129e698d,0xc7982f56,
					0x064c70c1,0x8fb523ba,0x826b76f8,0x1efa58a9,
					0x1226e6af,0xc96e2010,0x97589940,0x8e785fe4,
					0xa338b398,0x065ffd22,0x2fd1e1b7,0xa1da01a0,
					0x90b84168,0xe3522241,0xd2855a4e,0xfe87611a};
	DWORD	i;

	for( i=0; i<32; i++)
		BN_Tmp2->pData[i] = TT[31-i];
}
#endif

		//	Step 2. G = U^2J mod P를 계산한다.
		ret = BN_ModExp(KCDSA_Params->KCDSA_g, BN_Tmp2, BN_Tmp1,
						KCDSA_Params->KCDSA_p);						GOTO_END;

		//	Step 3. G = 1이면 단계 1로 간다.
		if( BN_Cmp(KCDSA_Params->KCDSA_g, &BN_One)!=0 )
			break;
	}
	//	Step 4. G를 출력한다.

	//
	ret = CTR_SUCCESS;
LABEL_END:
	if( BN_Tmp1!=NULL )	DestroyBigNum(BN_Tmp1);
	if( BN_Tmp2!=NULL )	DestroyBigNum(BN_Tmp2);
	return ret;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*		- CTR_INVALID_POINTER			KCDSA_Params값이 NULL임.
*		- CTR_MEMORY_ALLOC_ERROR		메모리 할당에서 문제가 있음.
*/
RET_VAL KCDSA_CLASS::KCDSA_GenerateKeyPair(
		BN_KCDSA_KEY		*KCDSA_Key)		//	Generated Private/Public Key
{
	DWORD		i;
	RET_VAL		ret;
	BIG_NUM		*BN_Tmp1=NULL, *BN_Tmp2=NULL;

	//	Step 0 : 입력 pointer가 NULL인지 확인
	if( KCDSA_Key==NULL )		return CTR_INVALID_POINTER;

	//	Step 0 : KCDSA_Key의 p, q, g, seed, x, y에 대한 메모리 할당이 적절하다고 가정.

	//	
	ret = CTR_MEMORY_ALLOC_ERROR;

	i = KCDSA_Key->KCDSA_p->Length;
	if( (BN_Tmp1=CreateBigNum(i+1))==NULL )	goto LABEL_END;
	if( (BN_Tmp2=CreateBigNum(i+1))==NULL )	goto LABEL_END;

	//	Step 1. 비공개 서명키 X <== random element in (0, q)
	srand( (unsigned)time( NULL ) );
	ret = BN_Rand(KCDSA_Key->KCDSA_x,
					BitsInDIGIT*KCDSA_Key->KCDSA_q->Length);		GOTO_END;
#ifdef MyDebug
	KCDSA_Key->KCDSA_x->pData[4] = 0x068c4ef3;
	KCDSA_Key->KCDSA_x->pData[3] = 0x55d8b6f5;
	KCDSA_Key->KCDSA_x->pData[2] = 0x3eff1df6;
	KCDSA_Key->KCDSA_x->pData[1] = 0xf243f985;
	KCDSA_Key->KCDSA_x->pData[0] = 0x63896c58;
#endif

	//	Step 2. 공개 검증키 Y <== G^{X^{-1} mod Q} mod P
	ret = BN_ModInv(BN_Tmp1, KCDSA_Key->KCDSA_x, KCDSA_Key->KCDSA_q);
																	GOTO_END;
	ret = BN_ModExp(KCDSA_Key->KCDSA_y, KCDSA_Key->KCDSA_g,
					BN_Tmp1, KCDSA_Key->KCDSA_p);				GOTO_END;

	//	Step 3. Z <== Y mod 2^512
	i = KCDSA_Key->KCDSA_y->Length;
	KCDSA_Key->KCDSA_y->Length = 512/BitsInDIGIT;
	ret = BN2OS(KCDSA_Key->KCDSA_y, 512/8, KCDSA_Key->KCDSA_z);		GOTO_END;
	KCDSA_Key->KCDSA_y->Length = i;

	//
	ret = CTR_SUCCESS;
LABEL_END:
	if( BN_Tmp1!=NULL )		DestroyBigNum(BN_Tmp1);
	if( BN_Tmp2!=NULL )		DestroyBigNum(BN_Tmp2);
	return ret;
}

/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*		- CTR_INVALID_POINTER			KCDSA_Params값이 NULL임.
*		- CTR_BUFFER_TOO_SMALL			출력용 Signature의 메모리가 부족함.
*		- CTR_INVALID_DATA_LEN			입력값 MsgDigest의 크기가 부적절함.
*		- CTR_MEMORY_ALLOC_ERROR		메모리 할당에서 문제가 있음.
*/
RET_VAL KCDSA_CLASS::KCDSA_Sign(
		BN_KCDSA_KEY		*KCDSA_PrivKey,	//	서명키
		BYTE				*MsgDigest,		//	해쉬값 Hash(z||M) 입력
		DWORD				MsgDigestLen,
		BYTE				*Signature, 	//	서명값 출력
		DWORD				*SignLen)
{
	//	Step 0. (선택 사항) P, Q, G와 Y가 올바른지 검증.
	//	Step 1. 난수값 K를 {1, ... , Q - 1}에서 랜덤하게 선택.
	//	Step 2. 증거값 W = G^K mod P를 계산한다.
	//	Step 3. 서명의 첫 부분 R = h(W)를 계산한다.
	//	Step 4. 메시지의 해쉬코드 H = h(Z||M)을 계산한다.
	//	Step 5. 중간값 E = (R XOR H) mod Q를 계산한다
	//	Step 6. 서명의 두 번째 값 S = X(K - E) mod Q를 계산한다.
	//	Step 7. 비트열 R과 , 정수 S의 쌍을 서명으로 출력한다. 즉 S = {R, S}.
	BYTE		bzTmp[2048/8];
	DWORD		i, j, qByteLen, DigestLen;
	RET_VAL		ret;
	BIG_NUM		*BN_K=NULL, *BN_Tmp1=NULL, *KCDSA_s=NULL;
	HAS160_ALG_INFO	AlgInfo;

	//	Step 0 : 입력 pointer가 NULL인지 확인
	if( KCDSA_PrivKey==NULL )	return CTR_INVALID_POINTER;

	//	Step 0 : KCDSA_PrivKey의 p, q, g, seed, x에 대한 메모리 할당이 적절하다고 가정.

	//	
	DigestLen = HAS160_DIGEST_VALUELEN;
	qByteLen = DIGITSIZE*KCDSA_PrivKey->KCDSA_q->Length;

	//
	if( SignLen!=NULL ) {
		i = *SignLen;
		*SignLen = DigestLen + qByteLen;
		ret = CTR_BUFFER_TOO_SMALL;
		if( (i!=0) && (i<*SignLen) )			goto LABEL_END;
	}
	if( Signature==NULL )	return CTR_SUCCESS;

	//	Step 0 : 입력 pointer가 NULL인지 확인
	if( MsgDigest==NULL )	return CTR_INVALID_POINTER;

	//
	ret = CTR_INVALID_DATA_LEN;
//	if( MsgDigestLen!=DigestLen )	goto LABEL_END;
	if( MsgDigestLen>qByteLen )		goto LABEL_END;

	//
	ret = CTR_MEMORY_ALLOC_ERROR;
	i = KCDSA_PrivKey->KCDSA_p->Length;
	if( (BN_K=CreateBigNum(i))==NULL )		goto LABEL_END;
	if( (BN_Tmp1=CreateBigNum(i))==NULL )	goto LABEL_END;
	i = KCDSA_PrivKey->KCDSA_q->Length;
	if( (KCDSA_s=CreateBigNum(i))==NULL )	goto LABEL_END;

	//	step 0. (선택 사항) 도메인 변수 P, Q, G와 공개 검증키 Y가 올바른지 검증
	//	step 1. 난수값 K를 [1, Q-1]에서 랜덤하게 선택한다.
	ret = BN_Rand(BN_K, 8*qByteLen);								GOTO_END;
	if( BN_Cmp(BN_K, KCDSA_PrivKey->KCDSA_q)>=0 ) {
		ret = BN_Sub(BN_K, BN_K, KCDSA_PrivKey->KCDSA_q);			GOTO_END;
	}

	//	step 2. 증거값 W=G^K mod P를 계산한다.
	ret = BN_ModExp(BN_Tmp1, KCDSA_PrivKey->KCDSA_g, BN_K,
					KCDSA_PrivKey->KCDSA_p);						GOTO_END;

	//	step 3. 서명의 첫 부분 R=h(W)를 계산한다.
	i = DIGITSIZE * KCDSA_PrivKey->KCDSA_p->Length;
	ret = BN2OS(BN_Tmp1, i, bzTmp);									GOTO_END;
	j = i;
	HAS160_Init(&AlgInfo);
	HAS160_Update(&AlgInfo, bzTmp, j);
	HAS160_Final(&AlgInfo, bzTmp);
	memcpy(Signature, bzTmp, HAS160_DIGEST_VALUELEN);	////	Step 7

	//	step 4. 메시지의 해쉬코드 H=h(Z||M)을 계산한다.

	//	step 5. 중간값 E=(R^H) mod Q를 계산한다.
	if( DigestLen<=MsgDigestLen ) {
		for( i=0; i<DigestLen; i++)	bzTmp[i] ^= MsgDigest[i];
		for(  ; i<MsgDigestLen; i++)			bzTmp[i]  = MsgDigest[i];
	}
	else {
		for( i=0; i<MsgDigestLen; i++)			bzTmp[i] ^= MsgDigest[i];
		i = DigestLen;
	}

	ret = OS2BN(bzTmp, i, BN_Tmp1);									GOTO_END;
	ret = BN_ModRed(BN_Tmp1, BN_Tmp1, KCDSA_PrivKey->KCDSA_q);		GOTO_END;

	//	step 6. 서명의 두 번째 값 S = X(K-E) mod Q를 계산한다.
	ret = BN_ModSub(BN_K, BN_K, BN_Tmp1, KCDSA_PrivKey->KCDSA_q);	GOTO_END;
	ret = BN_ModMul(KCDSA_s, KCDSA_PrivKey->KCDSA_x, BN_K,
					KCDSA_PrivKey->KCDSA_q);						GOTO_END;

	//	step 7. 정수쌍 R, S를 서명으로 출력한다. 즉 서명 = {R, S}.
	ret = BN2OS(KCDSA_s, qByteLen, Signature+DigestLen);
																	GOTO_END;

	//
	ret = CTR_SUCCESS;
LABEL_END:
	if( BN_K!=NULL )	DestroyBigNum(BN_K);
	if( BN_Tmp1!=NULL )	DestroyBigNum(BN_Tmp1);
	if( KCDSA_s!=NULL )	DestroyBigNum(KCDSA_s);
	return ret;
}



/**************************************************************************
*
*	Function Description
*	
*	Return values:
*		- CTR_SUCCESS					함수가 성공적으로 수행됨.
*		- CTR_VERIFY_FAIL				서명 검증에 실패함.
*		- CTR_INVALID_POINTER			KCDSA_Params값이 NULL임.
*		- CTR_INVALID_DATA_LEN			입력값 MsgDigest의 크기가 부적절함.
*		- CTR_INVALID_SIGNATURE_LEN		입력된 Signature의 크기가 부적절함.
*		- CTR_MEMORY_ALLOC_ERROR		메모리 할당에서 문제가 있음.
*/
RET_VAL KCDSA_CLASS::KCDSA_Verify(
		BN_KCDSA_KEY		*KCDSA_PubKey,	//	공개키
		BYTE				*MsgDigest, 	//	해쉬값 Hash(M) 입력
		DWORD				MsgDigestLen,
		BYTE				*Signature, 	//	서명값 입력
		DWORD				SignLen)
{

	//	Step 0.	(선택 사항) 서명자의 인증서를 확인,
	//			서명검증에 필요한 도메인 변수 P, Q, G와 공개 검증키 Y, Z를 추출.
	//	Step 1.	수신된 서명 S={R',S'}에 대해 0< R'<2^|h( )| , 0<S'<Q 임을 확인.
	//	Step 2.	Z = h(Y)를 계산, M'에 대한 해쉬코드 H' = h(Z||M')을 계산한다.
	//	Step 3.	중간값 E' = (R' XOR H') mod Q을 계산한다.
	//	Step 4. Y를 이용하여 증거값 W' =Y^S' G^E' mod P를 계산.
	//	Step 5.	h(W') = R'이 성립하는지 확인한다.
	BYTE		bzTmp[2048/8];
	DWORD		i, j, qByteLen, DigestLen;
	RET_VAL		ret;
	BIG_NUM		*BN_Tmp1=NULL, *BN_Tmp2=NULL, *BN_Tmp3=NULL, *KCDSA_s=NULL;
	HAS160_ALG_INFO	AlgInfo;


//TRACE("[2]0x%X	", AfxGetThread()->m_nThreadID);

	//	Step 0 : 입력 pointer가 NULL인지 확인
	if( (KCDSA_PubKey==NULL) || (MsgDigest==NULL) || (Signature==NULL) )
	{
		return CTR_INVALID_POINTER;
	}

	//	Step 0 : KCDSA_PubKey의 p, q, g, seed, y에 대한 메모리 할당이 적절하다고 가정.

	//	
	DigestLen = HAS160_DIGEST_VALUELEN;
	qByteLen = DIGITSIZE*KCDSA_PubKey->KCDSA_q->Length;

	//
	if( MsgDigestLen>qByteLen )
	{
		return CTR_INVALID_DATA_LEN;
	}
	if( SignLen!=DigestLen+qByteLen )
	{
		return CTR_INVALID_SIGNATURE_LEN;
	}


	//
	ret = CTR_MEMORY_ALLOC_ERROR;
	i = KCDSA_PubKey->KCDSA_p->Length;
	if( (BN_Tmp1=CreateBigNum(i))==NULL )	goto LABEL_END;
	if( (BN_Tmp2=CreateBigNum(i))==NULL )	goto LABEL_END;
	if( (BN_Tmp3=CreateBigNum(i))==NULL )	goto LABEL_END;
	i = KCDSA_PubKey->KCDSA_q->Length;
	if( (KCDSA_s=CreateBigNum(i))==NULL )	goto LABEL_END;

	//
	memcpy(bzTmp, Signature, qByteLen);
	ret = OS2BN(Signature+qByteLen, qByteLen, KCDSA_s);				GOTO_END;

	ret = CTR_VERIFY_FAIL;
	if( BN_Cmp(KCDSA_s, KCDSA_PubKey->KCDSA_q)>=0 )			goto LABEL_END;

	//	step 0. (선택 사항) 서명자의 인증서를 확인하고,
	//			서명검증에 필요한 도메인 변수 P, Q, G와 공개 검증키 Y를 추출
	//	step 1. 수신된 서명 {R', S'}에 대해 0<R'<2^|Q|, 0<S'<Q 임을 확인한다.
	//	step 2. 서명자의 공개키에 대한 해쉬코드 Z=h(Y)를 계산하고,
	//			검증할 메시지 M'에 대한 해쉬코드 H'=h(Z||M')을 계산한다.

	//	step 3. 중간값 E'=(R'^H') mod Q을 계산한다.
	if( DigestLen<=MsgDigestLen ) {
		for( i=0; i<DigestLen; i++)	bzTmp[i] ^= MsgDigest[i];
		for(  ; i<MsgDigestLen; i++)			bzTmp[i]  = MsgDigest[i];
	}
	else {
		for( i=0; i<MsgDigestLen; i++)			bzTmp[i] ^= MsgDigest[i];
		i = DigestLen;
	}
	ret = OS2BN(bzTmp, i, BN_Tmp1);									GOTO_END;
	ret = BN_ModRed(BN_Tmp1, BN_Tmp1, KCDSA_PubKey->KCDSA_q);		GOTO_END;

//EnterCriticalSection(&crsec);


	//	step 4. Y를 이용하여 증거값 W'=Y^{S'} G^{E'} mod P를 계산한다.
//	ret = BN_DoubleModExp(BN_Tmp1, KCDSA_PubKey->KCDSA_y, KCDSA_s,
//			KCDSA_PubKey->KCDSA_g, BN_Tmp1, KCDSA_PubKey->KCDSA_p);	GOTO_END;
	ret = BN_ModExp(BN_Tmp2, KCDSA_PubKey->KCDSA_y,
					KCDSA_s, KCDSA_PubKey->KCDSA_p);
				GOTO_END;

//TRACE("[3]0x%X	", AfxGetThread()->m_nThreadID);

	ret = BN_ModExp(BN_Tmp3, KCDSA_PubKey->KCDSA_g,
					BN_Tmp1, KCDSA_PubKey->KCDSA_p);				GOTO_END;

//TRACE("[4]0x%X	", AfxGetThread()->m_nThreadID);

	ret = BN_ModMul(BN_Tmp1, BN_Tmp2, BN_Tmp3,
					KCDSA_PubKey->KCDSA_p);							GOTO_END;


	//	step 5. h(W') = R'이 성립하는지 확인한다.
	i = DIGITSIZE * KCDSA_PubKey->KCDSA_p->Length;
	ret = BN2OS(BN_Tmp1, i, bzTmp);									GOTO_END;
	j = i;
	i = 0;

	
	HAS160_Init(&AlgInfo);
	HAS160_Update(&AlgInfo, bzTmp, j);
//TRACE("[5]0x%X	", AfxGetThread()->m_nThreadID);
	HAS160_Final(&AlgInfo, bzTmp);

//LeaveCriticalSection(&crsec);
	ret = CTR_VERIFY_FAIL;
	if( memcmp(bzTmp, Signature, HAS160_DIGEST_VALUELEN)!=0 )
		goto LABEL_END;

	//
	ret = CTR_SUCCESS;
LABEL_END:
	if( BN_Tmp1!=NULL )	DestroyBigNum(BN_Tmp1);
	if( BN_Tmp2!=NULL )	DestroyBigNum(BN_Tmp2);
	if( BN_Tmp3!=NULL )	DestroyBigNum(BN_Tmp3);
	if( KCDSA_s!=NULL )	DestroyBigNum(KCDSA_s);
	
	return ret;
}


// ct.kcdsa

/**************************************************************************
* CT_KCDSA_CreateKeyObject
*	BN_KCDSA_KEY 데이타의 메모리 할당.
*
* Return values:
*	- CTR_SUCCESS					함수가 성공적으로 수행됨.
*	- CTR_INVALID_POINTER			KCDSA_Params값이 NULL임.
*	- CTR_MEMORY_ALLOC_ERROR		메모리 할당에서 문제가 있음.
**************************************************************************/
RET_VAL KCDSA_CLASS::CT_KCDSA_CreateKeyObject(BN_KCDSA_KEY **KCDSA_Params)
{
	return KCDSA_CreateKeyObject(KCDSA_Params);
}

/**************************************************************************
* CT_KCDSA_DestroyKeyObject
*	BN_KCDSA_KEY 데이타의 메모리 소거.
*
* Return values:
*	- CTR_SUCCESS					함수가 성공적으로 수행됨.
*	- CTR_INVALID_POINTER			KCDSA_Params값이 NULL임.
**************************************************************************/
RET_VAL KCDSA_CLASS::CT_KCDSA_DestroyKeyObject(BN_KCDSA_KEY **KCDSA_Params)
{
	return KCDSA_DestroyKeyObject(KCDSA_Params);
}

/**************************************************************************
* CT_KCDSA_GetDomainParameter
*	Domain Parameter를 BN_KCDSA_KEY 구조체에 입력. 
*   미리 생성한 값을 그대로 사용
*
* Return values:
*
**************************************************************************/

void KCDSA_CLASS::CT_KCDSA_GetDomainParameter(BN_KCDSA_KEY *KCDSA_Key)
{
	int i;
	
	/* KCDSA_p */
	KCDSA_Key->KCDSA_p->Length = DOMAIN_KCDSA_P_LEN;
	for ( i=0; i<DOMAIN_KCDSA_P_LEN; i++)
		KCDSA_Key->KCDSA_p->pData[i] = DOMAIN_KCDSA_P[DOMAIN_KCDSA_P_LEN-i-1];
	
	/* KCDSA_q */
	KCDSA_Key->KCDSA_q->Length = DOMAIN_KCDSA_Q_LEN;
	for ( i=0; i<DOMAIN_KCDSA_Q_LEN; i++)
		KCDSA_Key->KCDSA_q->pData[i] = DOMAIN_KCDSA_Q[DOMAIN_KCDSA_Q_LEN-i-1];
	
	/* KCDSA_g */
	KCDSA_Key->KCDSA_g->Length = DOMAIN_KCDSA_G_LEN;
	for ( i=0; i<DOMAIN_KCDSA_G_LEN; i++)
		KCDSA_Key->KCDSA_g->pData[i] = DOMAIN_KCDSA_G[DOMAIN_KCDSA_G_LEN-i-1];
	
	/* Seed */
	KCDSA_Key->SeedLen = DOMAIN_KCDSA_SEED_LEN;
	for ( i=0; i<DOMAIN_KCDSA_SEED_LEN; i++)
		KCDSA_Key->Seed[i] = DOMAIN_KCDSA_SEED[i];

	/* Count */
	KCDSA_Key->Count = DOMAIN_KCDSA_COUNT;
}

/*************************************************************************
* CT_KCDSA_GenerateKeyPair
*	public/private key pair를 생성
*
* Return values
*	- CTR_SUCCESS					함수가 성공적으로 수행됨.
*	- CTR_INVALID_POINTER			KCDSA_Params값이 NULL임.
*	- CTR_MEMORY_ALLOC_ERROR		메모리 할당에서 문제가 있음.
**************************************************************************/		
RET_VAL KCDSA_CLASS::CT_KCDSA_GenerateKeyPair(BN_KCDSA_KEY *KCDSA_Key)
{
	CT_KCDSA_GetDomainParameter(KCDSA_Key);
	
	return KCDSA_GenerateKeyPair(KCDSA_Key);
}

/*************************************************************************
* CT_KCDSA_Signature
*	public/private key pair를 생성
*
* Return values
*	- CTR_SUCCESS					함수가 성공적으로 수행됨.
*	- CTR_INVALID_POINTER			KCDSA_Params값이 NULL임.
*	- CTR_BUFFER_TOO_SMALL			출력용 Signature의 메모리가 부족함.
*	- CTR_INVALID_DATA_LEN			입력값 MsgDigest의 크기가 부적절함.
*	- CTR_MEMORY_ALLOC_ERROR		메모리 할당에서 문제가 있음.
**************************************************************************/	
RET_VAL KCDSA_CLASS::CT_KCDSA_Sign(BN_KCDSA_KEY *KCDSA_PrivKey, 
					  BYTE *pbSrcData,
					  int srcDataLen,
					  BYTE *pbSign,
					  unsigned long *signLen)
{
	RET_VAL ret;
	BYTE DigestValue[HAS160_DIGEST_VALUELEN];
	int DigestValueLen = 0;

	CT_HAS160_Hash(pbSrcData, srcDataLen, DigestValue, &DigestValueLen);

	*signLen = 1024/8;
	ret = KCDSA_Sign(
		KCDSA_PrivKey, 
		DigestValue, 
		DigestValueLen,
		pbSign, 
		signLen
		);

	return ret;
}


/*************************************************************************
* CT_KCDSA_Signature
*	public/private key pair를 생성
*
* Return values
*	- CTR_SUCCESS					함수가 성공적으로 수행됨.
*	- CTR_VERIFY_FAIL				서명 검증에 실패함.
*	- CTR_INVALID_POINTER			KCDSA_Params값이 NULL임.
*	- CTR_INVALID_DATA_LEN			입력값 MsgDigest의 크기가 부적절함.
*	- CTR_INVALID_SIGNATURE_LEN		입력된 Signature의 크기가 부적절함.
*	- CTR_MEMORY_ALLOC_ERROR		메모리 할당에서 문제가 있음.
**************************************************************************/	
RET_VAL KCDSA_CLASS::CT_KCDSA_Verify(BN_KCDSA_KEY *KCDSA_PubKey, 
					    BYTE *pbSrcData,
						int srcDataLen,
						BYTE *pbSign,
						int signLen)
{
	RET_VAL ret;
	BYTE DigestValue[HAS160_DIGEST_VALUELEN];
	int DigestValueLen = 0;
	
	CT_HAS160_Hash(pbSrcData, srcDataLen, DigestValue, &DigestValueLen);
	
	ret = KCDSA_Verify(
		KCDSA_PubKey, 
		DigestValue, 
		DigestValueLen,
		pbSign, 
		signLen
		);

	return ret;
}


/*************************************************************************
* CT_HAS160_Hash
*	HAS160 hash
*
* Return values
*
**************************************************************************/	
void KCDSA_CLASS::CT_HAS160_Hash(BYTE *pbSrcData,
					int srcDataLen,
					BYTE *pbHashValue,
					int *hashValueLen)
{
	BYTE readBuf[1024+32];
	HAS160_ALG_INFO	AlgInfo;
	int  cpySize;
	int  srcDataPtr=0;

	HAS160_Init(&AlgInfo);
	
	while ( srcDataPtr < srcDataLen )
	{
		if ( srcDataLen-srcDataPtr >= 1024 ) 
			cpySize = 1024;
		else
			cpySize = srcDataLen - srcDataPtr;
		
		memset(readBuf, 0, 1024+32);
		memcpy(readBuf, pbSrcData + srcDataPtr, cpySize);
		
		HAS160_Update(&AlgInfo, readBuf, cpySize);
		
		srcDataPtr += cpySize;
	}
	
	HAS160_Final(&AlgInfo, pbHashValue);
	*hashValueLen = HAS160_DIGEST_VALUELEN;
}

/*************************************************************************
* CT_KCDSA_GetPrivKeyStringLen
*	Private key를 HEX로 저장할 경우 길이를 구한다.
*
* Return values
*
**************************************************************************/	
int KCDSA_CLASS::CT_KCDSA_GetPrivKeyStringLen(BN_KCDSA_KEY *KCDSA_PrivKey)
{
	/* KCDSA_x, KCDSA_Z만 문자열로 변환한다. */
	/* KCDSA_z는 길이가 64 byte 고정이다.    */
	int len;

	len  = sizeof(DWORD); /* x length */
	len += (KCDSA_PrivKey->KCDSA_x->Length * sizeof(DWORD));
	len += sizeof(DWORD); /* z length : 64 */
	len += 64;
	len = len * 2;

	return len;
}

/*************************************************************************
* CT_KCDSA_GetPubKeyStringLen
*	Public key를 HEX로 저장할 경우 길이를 구한다.
*
* Return values
*
**************************************************************************/	
int KCDSA_CLASS::CT_KCDSA_GetPubKeyStringLen(BN_KCDSA_KEY *KCDSA_PubKey)
{
	/* KCDSA_y, KCDSA_Z만 문자열로 변환한다. */
	/* KCDSA_z는 길이가 64 byte 고정이다.    */
	int len;

	len  = sizeof(DWORD); /* y length */
	len += KCDSA_PubKey->KCDSA_y->Length * sizeof(DWORD) ;
	len += sizeof(DWORD); /* z length : 64 */
	len += 64;
	len = len * 2;

	return len;
}

/**********************************************************************************************
* AES_BinToHex
*	  :	Bin -> Hex(2진수 -> 16진수)
*  - Visibility : public 
*	- parameters
*	  : hexBlock - hex byte 배열 
*		binBlock - bin byte 배열
*		blockLen - 변환할 블력 길이
**********************************************************************************************/
void KCDSA_CLASS::CT_KCDSA_BinToHex(char *hex, const unsigned char *bin, int binLength)
{
	int i;
	
	for (i = 0; i < binLength; i++) {
		sprintf ((char*)hex+2*i, "%02X", (unsigned char)bin[i]);
	}
	
	hex[binLength*2] = '\0';
}

/**********************************************************************************************
* AES_Char2Int
*	  :	Char -> Int(16진수 -> 10진수)
*  - Visibility : public 
*	- parameters
*	  : ch - 변환할 문자 
*	- return value
*	  : int - 0~15값 
**********************************************************************************************/
int KCDSA_CLASS::CT_KCDSA_CharToInt(char ch)
{
	switch(ch) {
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

/**********************************************************************************************
* AES_HexToBin
*	  :	Hex -> Binary
*  - Visibility : public 
*	- parameters
*	  : bin - 변환하여 저장할 문자열 공간  
*		hex	- 변환할 문자열
*		binlength - 변환할 개수(변환할 Hex 길이의 2배)
*	- return value
*	  : int - 0
**********************************************************************************************/
void KCDSA_CLASS::CT_KCDSA_HexToBin(unsigned char *bin, const char *hex, int binLength)
{
	int i;
	
	for(i = 0; i < binLength; i++)
		bin[i] = 16 * CT_KCDSA_CharToInt(hex[2 * i]) + CT_KCDSA_CharToInt(hex[2 * i + 1]);
}


/*************************************************************************
* CT_KCDSA_WritePrivKeyToString
*	Private key를 hex로 변환한다.
*
* Return values
*
**************************************************************************/	
int KCDSA_CLASS::CT_KCDSA_PrivKeyToHex(BN_KCDSA_KEY *KCDSA_PrivKey, BYTE *hexPrivKey)
{
	BYTE *tmp;
	int binLen, ptr = 0;
	DWORD zLen = 64;

	binLen = CT_KCDSA_GetPrivKeyStringLen(KCDSA_PrivKey) / 2;
	tmp = (BYTE *)malloc(binLen);

	memcpy(tmp, &(KCDSA_PrivKey->KCDSA_x->Length), sizeof(DWORD));
	ptr += sizeof(DWORD);
	memcpy(tmp+ptr, (KCDSA_PrivKey->KCDSA_x->pData), (KCDSA_PrivKey->KCDSA_x->Length) * sizeof(DWORD));
	ptr += KCDSA_PrivKey->KCDSA_x->Length * sizeof(DWORD);
	memcpy(tmp+ptr, &zLen, sizeof(DWORD));
	ptr += sizeof(DWORD);
	memcpy(tmp+ptr, KCDSA_PrivKey->KCDSA_z, zLen);
	ptr += 64;

	CT_KCDSA_BinToHex((char *)hexPrivKey, tmp, binLen);

	free(tmp);

	return 1;
}

/*************************************************************************
* CT_KCDSA_WritePrivKeyToString
*	Public key를 hex로 변환한다.
*
* Return values
*
**************************************************************************/	
int KCDSA_CLASS::CT_KCDSA_PubKeyToHex(BN_KCDSA_KEY *KCDSA_PubKey, BYTE *hexPubKey)
{
	BYTE *tmp;
	int binLen, ptr = 0;
	DWORD zLen = 64;
	
	binLen = CT_KCDSA_GetPubKeyStringLen(KCDSA_PubKey) / 2;
	tmp = (BYTE *)malloc(binLen);
	
	memcpy(tmp, &(KCDSA_PubKey->KCDSA_y->Length), sizeof(DWORD));
	ptr += sizeof(DWORD);
	memcpy(tmp+ptr, (KCDSA_PubKey->KCDSA_y->pData), (KCDSA_PubKey->KCDSA_y->Length) * sizeof(DWORD));
	ptr += KCDSA_PubKey->KCDSA_y->Length * sizeof(DWORD);
	memcpy(tmp+ptr, &zLen, sizeof(DWORD));
	ptr += sizeof(DWORD);
	memcpy(tmp+ptr, KCDSA_PubKey->KCDSA_z, zLen);
	ptr += 64;
	
	CT_KCDSA_BinToHex((char *)hexPubKey, tmp, binLen);
	
	free(tmp);
	return 1;
}

/*************************************************************************
* CT_KCDSA_HexToPrivKey
*
* Return values
*
**************************************************************************/	
int KCDSA_CLASS::CT_KCDSA_HexToPrivKey(BN_KCDSA_KEY *KCDSA_PrivKey, char *hexPrivKey)
{
	int hexPrivKeyLen;
	int ptr = 0;
	DWORD zLen = 0;
	BYTE *tmp;
	
	CT_KCDSA_GetDomainParameter(KCDSA_PrivKey);
	
	hexPrivKeyLen = strlen(hexPrivKey);
	tmp = (BYTE *)malloc(hexPrivKeyLen/2);
	
	CT_KCDSA_HexToBin(tmp, hexPrivKey, hexPrivKeyLen/2);
	
	memcpy(&(KCDSA_PrivKey->KCDSA_x->Length), tmp, sizeof(DWORD));
	ptr += sizeof(DWORD);
	memcpy(KCDSA_PrivKey->KCDSA_x->pData, tmp+ptr, KCDSA_PrivKey->KCDSA_x->Length * sizeof(DWORD));
	ptr += KCDSA_PrivKey->KCDSA_x->Length * sizeof(DWORD);
	memcpy(&zLen, tmp+ptr, sizeof(DWORD));
	ptr += sizeof(DWORD);
	memcpy(KCDSA_PrivKey->KCDSA_z, tmp+ptr, zLen);
	
	free(tmp);
	return 1;
}

/*************************************************************************
* CT_KCDSA_HexToPubKey
*
* Return values
*
**************************************************************************/	
int KCDSA_CLASS::CT_KCDSA_HexToPubKey(BN_KCDSA_KEY *KCDSA_PubKey, char *hexPubKey)
{
	int hexPubKeyLen;
	int ptr = 0;
	DWORD zLen = 0;
	BYTE *tmp;
	
	CT_KCDSA_GetDomainParameter(KCDSA_PubKey);

	
	hexPubKeyLen = strlen(hexPubKey);
	tmp = (BYTE *)malloc(hexPubKeyLen/2);
	CT_KCDSA_HexToBin(tmp, hexPubKey, hexPubKeyLen/2);

	memcpy(&(KCDSA_PubKey->KCDSA_y->Length), tmp, sizeof(DWORD));
	ptr += sizeof(DWORD);
	memcpy(KCDSA_PubKey->KCDSA_y->pData, tmp+ptr, KCDSA_PubKey->KCDSA_y->Length * sizeof(DWORD));
	ptr += KCDSA_PubKey->KCDSA_y->Length * sizeof(DWORD);
	memcpy(&zLen, tmp+ptr, sizeof(DWORD));
	ptr += sizeof(DWORD);
	memcpy(KCDSA_PubKey->KCDSA_z, tmp+ptr, zLen);
	
	free(tmp);
	return 1;
}

BOOL KCDSA_CLASS::CertificationPKS(unsigned char *key, char *sz_eTerPublicket, unsigned char *sz_eSignature, long l_signsize)
{
	BN_KCDSA_KEY *KCDSA_Key;
	RET_VAL RetVal;

	CT_KCDSA_CreateKeyObject(&KCDSA_Key);
	CT_KCDSA_HexToPubKey(KCDSA_Key, sz_eTerPublicket);

	RetVal = CT_KCDSA_Verify(
		KCDSA_Key,
		key,
		16,
		sz_eSignature,
		l_signsize);


	CT_KCDSA_DestroyKeyObject(&KCDSA_Key);

	if (RetVal == 0)
	{	
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
