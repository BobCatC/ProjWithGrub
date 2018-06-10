#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "types.h"
#include "as_func.h"
#include "std_def.h"

#define KEYBOARD_PORT			   	0x60

#define TYPE_ACTION_NO_ACTION	   	0x0
#define TYPE_ACTION_PRESSED		 	0x1
#define TYPE_ACTION_RELEASED		0x2

#define BUTTON_TYPE_NOTH			0x0
#define BUTTON_TYPE_ASCII_LETTER	0x01
#define BUTTON_TYPE_NUM			 	0x02
#define BUTTON_TYPE_F			   	0x04
#define BUTTON_TYPE_CONTROL		 	0x05
#define BUTTON_TYPE_ASCII_SPECIAL   0x06

#define KB_ESCAPE				   	0x01
#define KB_BACKSPACE				0x0E
#define KB_TAB					 	0x0F
#define KB_ENTER					0x1C
#define KB_L_CONTROL				0x1D
#define KB_L_SHIFT				  	0x2A
#define KB_R_SHIFT				  	0x36
#define KB_L_ALT					0x38
#define KB_CAPS_LOCK				0x3A
#define KB_NUM_LOCK				 	0x45
#define KB_SCROLL_LOCK			  	0x46

#define KB_ARROW_UP				 	0xE048
#define KB_ARROW_DOWN			   	0xE050
#define KB_ARROW_LEFT			   	0xE04B
#define KB_ARROW_RIGHT			  	0xE04D

struct KeyboardAction{
public:
	WORD ButtonType;
	WORD IDButton;
	BYTE iTypeOfAction;
private:
	BYTE GetButtonType();
	void ProcessScanCode(WORD ScanCode);
public:
	KeyboardAction();
	
	KeyboardAction(WORD ScanCode);
	
	int Init(WORD ScanCode);
	
	KeyboardAction(const KeyboardAction& ka);
	
	bool fHasAction();
	
	bool fButtonPressed();
	bool fButtonReleased();
	
	char GetASCII_LETTER();
	char GetASCII_NUM_inchar();
	BYTE GetASCII_NUM();
	char GetASCII_SPECIAL();
	
	void ReleaseAction();
};

class XInterface{
	BYTE _iPort;
public:
	void Setup(BYTE iPort);
	KeyboardAction GetUsersActions();
};


class CKeyboard : public XInterface{
private:
	bool _fRegisterInverts;
	KeyboardAction Action;
public:
	CKeyboard();
	
	void RecieveUsersActions();
	
	KeyboardAction GetAction() const;
	
	void Invert();
	bool GetInvert() const;
	
	char GetAsciiLetter();
	
	void ReleaseAction();
};


#endif
