#include "screen.h"
#include "printf_putchar.h"

#define VRAM_SIZE (MAX_COL*MAX_ROW)
#define DEF_VRAM_BASE ((u16*)0xb8000)


#define PUT_C PUT_C1


BYTE CreateColour(BYTE background_colour, BYTE print_colour) {
	return (background_colour << 4 | print_colour);
}

void PUT_C1(XView* pCrtView, char c){
	pCrtView->PutC(c);
}	

static void cons_putc(XView* pCrtView, int c){
	switch (c){
		case '\t':
			for(int iS = 0; iS < 8; ++iS)
				cons_putc(pCrtView, ' ');
			break;
		case '\r':
			pCrtView->MoveToStart();
			break;
		case '\n':
			cons_putc(pCrtView, '\r');
			pCrtView->IncRaw();
			break;
		case '\b':
			break;
		default:
			PUT_C(pCrtView, c);
	};
}

void putchar(void* pCrtView, __IN__ int c){
	if (c == '\n')
		cons_putc((XView*)pCrtView, '\r');
	cons_putc((XView*)pCrtView, c);
}

/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */
/*							   XView								*/
/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */

XView::XView(){
	_PrintView.left = _PrintView.top = _PrintView.right = _PrintView.bottom = 0;
	_PrintPoint.x = _PrintPoint.y = 0;
	_fInitDone = false;
	_PrintColour = CreateColour(BLACK, GREEN);
	
	_Screen.Setup(DEF_VRAM_BASE, 2, 0, N_COL - 1, 0, N_ROW - 1);
}

void XView::PutC(const BYTE c){
	if(fCrtPointInView()){
		_Screen.PutChar(GetCrtXInScreen(), GetCrtYInScreen(), c, GetCrtColour());
	}

	IncCol();
	return;
}

bool XView::fCrtPointInView() const{
	int CrtX, CrtY;
	CrtX = GetCrtX();
	CrtY = GetCrtY();
	
	if(CrtX < 0 || CrtX >= _ViewMetrics.x ||
	   CrtY < 0 || CrtY >= _ViewMetrics.y)
		return false;
	
	return true;
}

int XView::GetCrtX() const{
	return _PrintPoint.x;
}

int XView::GetCrtY() const{
	return _PrintPoint.y;
}

int XView::GetCrtXInScreen() const{
	return (_PrintPoint.x + _PrintView.left);
}

int XView::GetCrtYInScreen() const{
	return (_PrintPoint.y + _PrintView.top);
}

void XView::MoveToStart(){
	_PrintPoint.x = 0;
}

void XView::IncRaw(){
	++_PrintPoint.y;
}

void XView::IncCol(){
	++(_PrintPoint.x);
}

int XView::Printf(const char *format, ...){
	va_list ap;
	int r;
	
	va_start (ap, format);
	r = vprintf ((void*)this, format, ap);
	va_end (ap);
	return r;
}

int XView::PutNSymbols(char Symbol, u16 nSymbolsToWrite){
	for(u16 i = 0; i < nSymbolsToWrite; ++i)
		cons_putc(this, Symbol);
	return 0;
}

int XView::PutNEndls(u16 nEndls){
	for(u16 i = 0; i < nEndls; ++i)
		cons_putc(this, '\n');
	return 0;
}

XRECT XView::ChangePrintView(int x0, int y0, int x1, int y1){
	XRECT PrevViewRect;
	if(x0 > x1 || y0 > y1)
		return PrevViewRect;
	PrevViewRect = _PrintView;
	_PrintView.Init(x0, y0, x1, y1);
	_ViewMetrics.x = _PrintView.right - _PrintView.left + 1;
	_ViewMetrics.y = _PrintView.bottom - _PrintView.top + 1;
	return PrevViewRect;
}

XRECT XView::ChangePrintView(const XRECT &NewRect){
	return ChangePrintView(NewRect.left, NewRect.top, NewRect.right, NewRect.bottom);
}

XPOINT XView::ChangePrintPoint(int x, int y){
	XPOINT PrevPrintPoint = _PrintPoint;
	_PrintPoint.Init(x, y);
	return PrevPrintPoint;
}

XPOINT XView::ChangePrintPoint(const XPOINT &NewPoint){
	return ChangePrintPoint(NewPoint.x, NewPoint.y);
}

int XView::PrintRect(int x0, int y0, int x1, int y1, TCOLOUR BackgroundColour, TCOLOUR SymbColour, char Symbol){
	TCOLOUR PrevColour = SetColour(CreateColour(BackgroundColour, SymbColour));
	XPOINT PrevPoint = ChangePrintPoint(0, 0);
	
	for(int x = x0; x < x1; ++x)
		for(int y = y0; y < y1; ++y){
			ChangePrintPoint(x, y);
			PutC(Symbol);
		}
	ChangePrintPoint(PrevPoint);
	SetColour(PrevColour);
	return 0;
}

int XView::PrintRect(XRECT &XRECT, TCOLOUR BackgroundColour, TCOLOUR SymbColour, char Symbol){
	return PrintRect(XRECT.left, XRECT.top, XRECT.right, XRECT.bottom, BackgroundColour, SymbColour, SymbColour);
}

XRECT& XView::GetPrintRect(){
	return _PrintView;
}

XPOINT& XView::GetPrintPoint(){
	return _PrintPoint;
}

XPOINT& XView::GetViewMetrics(){
	return _ViewMetrics;
}

int XView::ClearView(){
	return PrintRect(0, 0, _ViewMetrics.x, _ViewMetrics.y, BLACK, BLACK, ' ');
}

TCOLOUR XView::GetCrtColour() const{
	return _PrintColour;
}

TCOLOUR XView::SetColour(TCOLOUR NewColour){
	TCOLOUR PrevColour = _PrintColour;
	_PrintColour = NewColour;
	return PrevColour;
}

int XView::FrameView(TCOLOUR BackgroundColour, TCOLOUR SymbColour){
	int xMax, yMax;
	xMax = _ViewMetrics.x;
	yMax = _ViewMetrics.y;
	SetColour(CreateColour(BackgroundColour, SymbColour));
	
	for(int x = 0; x < xMax; ++x){
		for(int y = 0; y < yMax; ++y){
			BYTE Crt = '\0';
			if(x == 0){
				if(y == 0){
					Crt = ANGLE_RD;
				}
				else if(y == yMax - 1){
					Crt = ANGLE_RT;
				}
				else{
					Crt = LINE_VER;
				}
			}
			if(x == xMax - 1){
				if(y == 0){
					Crt = ANGLE_LD;
				}
				else if(y == yMax - 1){
					Crt = ANGLE_LT;
				}
				else{
					Crt = LINE_VER;
				}
			}
			if((y == 0 || y == yMax - 1) && x != 0 && x != xMax - 1){
				Crt = LINE_HOR;
			}
			if(Crt == '\0')
				continue;
			ChangePrintPoint(x, y);
			PutC(Crt);
		}
	}
	return 0;
}

















































