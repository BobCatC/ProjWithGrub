#include "memory.h"

void memcpy(__IN__ void* pDist, __IN__ const void* pScr, __IN__ size_t cbSize){
	u8* pD, *pS;
	pD = (u8*)pDist;
	pS = (u8*)pScr;
	for(size_t i = 0; i != cbSize; ++i){
		pD[i] = pS[i];
	}
}

size_t strlen(const char* szString){
	size_t i = 0;
	while(szString[i] != '\0')
		++i;
	return i;
}

bool fEqualStrings(const char* s1, const char* s2, size_t c){
	int i = 0;
	while(i < c){
		if(s1[i] != s2[i])
			return false;
		if(s1[i] == 0)
			break;
		++i;
	}
	return true;
}

bool atouh(const char* s, u32* pnum){
	int i = 2;
	u32 num = 0;
	
	if(s[0] != '0' || s[1] != 'x' || s[2] == '\0')
		return false;
	
	while(s[i] != '\0'){
		if(s[i] < '0' || s[i] > '9'){
			if(s[i] < 'A' || s[i] > 'F')
				return false;
			num = (num << 4) + (s[i] - 'A' + 10);
		}
		else{
			num = (num << 4) + (s[i] - '0');
		}
		++i;
	}
	*pnum = num;
	return true;
}
