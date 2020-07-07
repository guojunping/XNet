///////////////////////////////////////
// Author : Mr Kim
// Name : 
///////////////////////////////////////

#include "Random.h"

namespace XNetFrame{

CRandom::CRandom(void){
	reset();
	m_uiMax = 255;
}

CRandom::CRandom(uint32 uiMax){
	reset();
	m_uiMax = uiMax;
}

CRandom::~CRandom(void){
}

uint32 CRandom::next(void){
	uint32 uiRandom;

	uiRandom = (uint32)::rand();
	return m_uiMax?uiRandom % m_uiMax:0xffffffff;
}

void CRandom::reset(void){
	::srand((uint32)time(NULL));
}

bool CRandom::reset(uint32 uiMax){
	//if(uiMax == 0) return false;

	m_uiMax = uiMax;
	::srand((uint32)time(NULL));

	return true;
}

}
