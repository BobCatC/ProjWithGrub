#include "types.h"

/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */
/*							   XRECT								*/
/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */

XRECT::XRECT(){
	left = top = right = bottom = 0;
}

XRECT::XRECT(const XRECT& r){
	left = r.left;
	top = r.top;
	right = r.right;
	bottom = r.bottom;
}

XRECT& XRECT::operator=(const XRECT& r){
	left = r.left;
	top = r.top;
	right = r.right;
	bottom = r.bottom;
	return *this;
}

void XRECT::Init(const int l, const int t, const int r, const int b){
	left = l;
	top = t;
	right = r;
	bottom = b;
}

void XRECT::Init(const XRECT& r){
	left = r.left;
	top = r.top;
	right = r.right;
	bottom = r.bottom;
}

/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */
/*							  XPOINT								*/
/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */

XPOINT::XPOINT(){
	x = y = 0;
}

XPOINT::XPOINT(const XPOINT& p){
	x = p.x;
	y = p.y;
}

XPOINT& XPOINT::operator=(const XPOINT& p){
	x = p.x;
	y = p.y;
	return *this;
}

void XPOINT::Init(const int xNew, const int yNew){
	x = xNew;
	y = yNew;
}

void XPOINT::Init(const XPOINT& p){
	x = p.x;
	y = p.y;
}



















