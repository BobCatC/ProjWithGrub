#ifndef _TYPES_H
#define _TYPES_H

#include "ctypes.h"

class XRECT{
public:
	int left, top, right, bottom;
	
	XRECT();
	XRECT(const XRECT& r);
	
	XRECT& operator=(const XRECT& r);
	
	void Init(const int l, const int t, const int r, const int b);
	void Init(const XRECT& r);
};


class XPOINT{
public:
	int x, y;
	
	XPOINT();
	XPOINT(const XPOINT& p);
	
	XPOINT& operator=(const XPOINT& p);
	
	void Init(const int xNew, const int yNew);
	void Init(const XPOINT& p);
};


#endif
