#ifndef _SCREEN_H
#define _SCREEN_H

#include "types.h"
#include "stdarg.h"
#include "printf.h"
#include "memory.h"

#define BLACK		  	0x0
#define BLUE			0x1
#define GREEN			0x2
#define CYAN			0x3
#define RED			 	0x4
#define MAGNETA		 	0x5
#define BROWN		   	0x6
#define GREY			0x7
#define DARK_GREN		0x8
#define BRIGHT_BLUE	 	0x9
#define BRIGHT_GREEN	0xA
#define BRIGHT_CYAN	 	0xB
#define BRIGHT_RED	  	0xC
#define BRIGHT_MAGNETA  0xD
#define BRIGHT_YELLOW   0xE
#define WHITE		   	0xF

#define ANGLE_RD		((BYTE)218)
#define ANGLE_LD		((BYTE)191)
#define ANGLE_LT		((BYTE)217)
#define ANGLE_RT		((BYTE)192)

#define LINE_HOR		((BYTE)196)
#define LINE_VER		((BYTE)179)
  

#define MAX_COL  80		// Maximum number of columns
#define MAX_ROW  25		// Maximum number of rows

#define N_COL	(MAX_COL)
#define N_ROW	(MAX_ROW)

#define CX_SCREEN   N_COL
#define CY_SCREEN   N_ROW

BYTE CreateColour(BYTE background_colour, BYTE print_colour);

class XScreen{
private:
	u16* _DefVRamBase;
	int _xMin, _yMin;
	int _xMax, _yMax;
	int _cxScreenSize, _cyScreenSize;
	u32 _cbScreenPointSize;
	bool _fSetupDone;
	
	inline bool fPointInScreen(int x, int y){
		if(x >= _xMin && x <= _xMax &&
		   y >= _yMin && y <= _yMax)
			return true;
		return false;
	}
	
	inline unsigned int GetOffsetInVRam(int x, int y){
		return _cbScreenPointSize * ((y * _cxScreenSize) + x);
	}
	
public:
	void PutChar(const int x, const int y, const BYTE c, const TCOLOUR Colour){
		if(_fSetupDone){
			if(fPointInScreen(x, y)){
				_DefVRamBase[GetOffsetInVRam(x, y)] = ((Colour << 8) | (c));
			}
		}
	}
	void Setup(u16* DefVRamBase, u32 cbScreenPointSize, int xMin, int xMax, int yMin, int yMax){
		_DefVRamBase = DefVRamBase;
		_xMin = xMin;
		_xMax = xMax;
		_yMin = yMin;
		_yMax = yMax;

	_cxScreenSize = _xMax - _xMin + 1;
	_cyScreenSize = _yMax - _yMin + 1;

	_cbScreenPointSize = 1;

	_fSetupDone = true;
	}
};


class XView{
protected:
	XScreen _Screen;
	XPOINT _PrintPoint;
	XRECT _PrintView;
	XPOINT _ViewMetrics;
	TCOLOUR _PrintColour;
	bool _fInitDone;
public:
	
	XView();
	
	void PutC(const BYTE c);
	int Printf(const char* format, ...);

	bool fCrtPointInView() const;
	int GetCrtX() const;
	int GetCrtY() const;
	int GetCrtXInScreen() const;
	int GetCrtYInScreen() const;
	
	void MoveToStart();
	void IncRaw();
	void IncCol();
	
	int PutNSymbols(const char Symbol, const u16 nSymbolsToWrite);
	int PutNEndls(const u16 nEndls);
	
	XRECT ChangePrintView(const int x0, const int y0, const int x1, const int y1);
	XRECT ChangePrintView(const XRECT& NewRect);
	XPOINT ChangePrintPoint(const int x, const int y);
	XPOINT ChangePrintPoint(const XPOINT& NewPoint);
	
	int PrintRect(int x0, int y0, int x1, int y1, TCOLOUR BackgroundColour, TCOLOUR SymbColour, char Symbol);
	int PrintRect(XRECT& XRECT, TCOLOUR BackgroundColour, TCOLOUR SymbColour, char Symbol);

	XRECT& GetPrintRect();
	XPOINT& GetPrintPoint();
	XPOINT& GetViewMetrics();
	
	int ClearView();
	
	TCOLOUR GetCrtColour() const;
	TCOLOUR SetColour(TCOLOUR NewColour);
	
	int FrameView(TCOLOUR BackgroundColour, TCOLOUR SymbColour);
};


void putchar(__IN__ int c);
BYTE CREATE_COLOUR(BYTE, BYTE);
void PUT_C1(char c);


#endif















