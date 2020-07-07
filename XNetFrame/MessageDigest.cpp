///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "MessageDigest.h"

namespace DnNetworkFramework{

CMessageDigest::CMessageDigest(void){
	reset(); // 
}

CMessageDigest::~CMessageDigest(void){
}

void CMessageDigest::reset(void){ // 
	m_msMd5_State.m_siCount[0] = 0;
	m_msMd5_State.m_siCount[1] = 0;

	m_msMd5_State.m_siAbcd[0] = 0x67452301;
	m_msMd5_State.m_siAbcd[1] = T_MASK ^ 0x10325476;
	m_msMd5_State.m_siAbcd[2] = T_MASK ^ 0x67452301;
	m_msMd5_State.m_siAbcd[3] = 0x10325476;
}

void CMessageDigest::digest(uchar8* szMd5){ // 
    static uchar8 szPad[64] = {
		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    uchar8 szData[8];
    sint32 siIndex;

	if(szMd5 == NULL) return;

	for(siIndex = 0; siIndex < 8; siIndex++){
		szData[siIndex] = (uchar8)(m_msMd5_State.m_siCount[siIndex >> 2] >> ((siIndex & 3) << 3));
	};

	update(szPad, ((55 - (m_msMd5_State.m_siCount[0] >> 3)) & 63) + 1);
    update(szData, 8);
    
	for(siIndex = 0; siIndex < 16; siIndex++){
		szMd5[siIndex] = (uchar8)(m_msMd5_State.m_siAbcd[siIndex >> 2] >> ((siIndex & 3) << 3));
	};
}

void CMessageDigest::update(uchar8* szData, sint32 siLen){ // 
	sint32  siLeft, siOffset, siBits, siCopy;
    uchar8* pData;

    if(szData == NULL || siLen <= 0) return;

	siLeft   = siLen;
	pData    = szData;
    siBits   = (siLen << 3); // the number of bits
	siOffset = ((m_msMd5_State.m_siCount[0] >> 3) & 63); // compute the number of bytes mod 64

    m_msMd5_State.m_siCount[0] += siBits;
    m_msMd5_State.m_siCount[1] += (siLen >> 29);

	if(m_msMd5_State.m_siCount[0] < siBits){
		m_msMd5_State.m_siCount[1]++;
	};

    if(siOffset != 0){
		siCopy = ((siOffset + siLen) > 64 ? (64 - siOffset) : siLen);

		memcpy(m_msMd5_State.m_szBuffer + siOffset, pData, siCopy);

		if((siOffset + siCopy) < 64) return;

		pData  += siCopy;
		siLeft -= siCopy;

		process(m_msMd5_State.m_szBuffer);
    };

	for(; siLeft >= 64; pData += 64, siLeft -= 64){
		process(pData);
	};

	if(siLeft != 0){
		memcpy(m_msMd5_State.m_szBuffer, pData, siLeft);
	};
}

void CMessageDigest::process(uchar8* szData){ // 
    sint32  a, b, c, d, t;
    sint32  siBuffer[16];
    sint32* X;

	a = m_msMd5_State.m_siAbcd[0];
	b = m_msMd5_State.m_siAbcd[1];
	c = m_msMd5_State.m_siAbcd[2];
	d = m_msMd5_State.m_siAbcd[3];

    if(!((szData - (uchar8*)0) & 3)){
		X = (sint32*)szData;
    }
	else{
		memcpy(siBuffer, szData, 64);
		X = siBuffer;
    };

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

#define F(x, y, z)                (((x) & (y)) | (~(x) & (z)))
#define SET(a, b, c, d, k, s, Ti) t = a + F(b, c, d) + X[k] + Ti; a = ROTATE_LEFT(t, s) + b

    SET(a, b, c, d,  0,  7,  T1);
    SET(d, a, b, c,  1, 12,  T2);
    SET(c, d, a, b,  2, 17,  T3);
    SET(b, c, d, a,  3, 22,  T4);
    SET(a, b, c, d,  4,  7,  T5);
    SET(d, a, b, c,  5, 12,  T6);
    SET(c, d, a, b,  6, 17,  T7);
    SET(b, c, d, a,  7, 22,  T8);
    SET(a, b, c, d,  8,  7,  T9);
    SET(d, a, b, c,  9, 12, T10);
    SET(c, d, a, b, 10, 17, T11);
    SET(b, c, d, a, 11, 22, T12);
    SET(a, b, c, d, 12,  7, T13);
    SET(d, a, b, c, 13, 12, T14);
    SET(c, d, a, b, 14, 17, T15);
    SET(b, c, d, a, 15, 22, T16);

#undef SET

#define G(x, y, z)                (((x) & (z)) | ((y) & ~(z)))
#define SET(a, b, c, d, k, s, Ti) t = a + G(b, c, d) + X[k] + Ti; a = ROTATE_LEFT(t, s) + b

    SET(a, b, c, d,  1,  5, T17);
    SET(d, a, b, c,  6,  9, T18);
    SET(c, d, a, b, 11, 14, T19);
    SET(b, c, d, a,  0, 20, T20);
    SET(a, b, c, d,  5,  5, T21);
    SET(d, a, b, c, 10,  9, T22);
    SET(c, d, a, b, 15, 14, T23);
    SET(b, c, d, a,  4, 20, T24);
    SET(a, b, c, d,  9,  5, T25);
    SET(d, a, b, c, 14,  9, T26);
    SET(c, d, a, b,  3, 14, T27);
    SET(b, c, d, a,  8, 20, T28);
    SET(a, b, c, d, 13,  5, T29);
    SET(d, a, b, c,  2,  9, T30);
    SET(c, d, a, b,  7, 14, T31);
    SET(b, c, d, a, 12, 20, T32);

#undef SET

#define H(x, y, z)                ((x) ^ (y) ^ (z))
#define SET(a, b, c, d, k, s, Ti) t = a + H(b, c, d) + X[k] + Ti; a = ROTATE_LEFT(t, s) + b

    SET(a, b, c, d,  5,  4, T33);
    SET(d, a, b, c,  8, 11, T34);
    SET(c, d, a, b, 11, 16, T35);
    SET(b, c, d, a, 14, 23, T36);
    SET(a, b, c, d,  1,  4, T37);
    SET(d, a, b, c,  4, 11, T38);
    SET(c, d, a, b,  7, 16, T39);
    SET(b, c, d, a, 10, 23, T40);
    SET(a, b, c, d, 13,  4, T41);
    SET(d, a, b, c,  0, 11, T42);
    SET(c, d, a, b,  3, 16, T43);
    SET(b, c, d, a,  6, 23, T44);
    SET(a, b, c, d,  9,  4, T45);
    SET(d, a, b, c, 12, 11, T46);
    SET(c, d, a, b, 15, 16, T47);
    SET(b, c, d, a,  2, 23, T48);

#undef SET

#define I(x, y, z)                ((y) ^ ((x) | ~(z)))
#define SET(a, b, c, d, k, s, Ti) t = a + I(b, c, d) + X[k] + Ti; a = ROTATE_LEFT(t, s) + b

    SET(a, b, c, d,  0,  6, T49);
    SET(d, a, b, c,  7, 10, T50);
    SET(c, d, a, b, 14, 15, T51);
    SET(b, c, d, a,  5, 21, T52);
    SET(a, b, c, d, 12,  6, T53);
    SET(d, a, b, c,  3, 10, T54);
    SET(c, d, a, b, 10, 15, T55);
    SET(b, c, d, a,  1, 21, T56);
    SET(a, b, c, d,  8,  6, T57);
    SET(d, a, b, c, 15, 10, T58);
    SET(c, d, a, b,  6, 15, T59);
    SET(b, c, d, a, 13, 21, T60);
    SET(a, b, c, d,  4,  6, T61);
    SET(d, a, b, c, 11, 10, T62);
    SET(c, d, a, b,  2, 15, T63);
    SET(b, c, d, a,  9, 21, T64);

#undef SET

    m_msMd5_State.m_siAbcd[0] += a;
    m_msMd5_State.m_siAbcd[1] += b;
    m_msMd5_State.m_siAbcd[2] += c;
    m_msMd5_State.m_siAbcd[3] += d;
}

}
