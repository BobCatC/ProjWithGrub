#include "interface.h"

char ASCII_LETTERS_TABLE[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 0, 0, 0, 0,
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0, 0, 0, 0, 0,
	'z', 'x', 'c', 'v', 'b', 'n', 'm'
}; // [0x10 ; 0x32]
const DWORD cASCII_letters_table = sizeof(ASCII_LETTERS_TABLE);

char ASCII_SPECIAL_SYMBOLS_TABLE[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', '=',
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '[', ']',
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ';', '\'', '`', 0,
	'\\', 0, 0, 0, 0, 0, 0, 0, ',', '.', '/', 0, 0, 0, ' '
};
const DWORD cASCII_special_table = sizeof(ASCII_SPECIAL_SYMBOLS_TABLE);

BYTE NUM_TABLE[] = {0xFF, 0xFF, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
BYTE NUM_TABLE_CHAR[] = {0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
const DWORD cNUM_TABLE_size = sizeof(NUM_TABLE);

const WORD F1_SCAN_CODE = 0x3B;
const WORD F10_SCAN_CODE = 0x44;
const WORD F11_SCAN_CODE = 0x57;
const WORD F12_SCAN_CODE = 0x58;

const char dBetweenRegisters = 'a' - 'A'; 


/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */
/*							 XInterface							 */
/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */

void XInterface::Setup(BYTE iPort){
	_iPort = iPort; 
}

KeyboardAction XInterface::GetUsersActions(){
	
	while((_in_b_port_64() & 0x1) == 0);

	WORD ScanCode = _in_b_port_60();
	
	if(ScanCode == 0xE0){
		ScanCode = (ScanCode << 8) | _in_b_port_60();
	}

	KeyboardAction Action(ScanCode);
	return Action;
}

/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */
/*						 KeyboardAction							 */
/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */


BYTE KeyboardAction::GetButtonType(){
	if(IDButton >> 8 == 0xE0){
		return BUTTON_TYPE_CONTROL;
	}
	if(IDButton < cASCII_letters_table && ASCII_LETTERS_TABLE[IDButton] != 0)
		return BUTTON_TYPE_ASCII_LETTER;
	if(IDButton < cASCII_special_table && ASCII_SPECIAL_SYMBOLS_TABLE[IDButton] != 0)
		return BUTTON_TYPE_ASCII_SPECIAL;
	if(IDButton < cNUM_TABLE_size && NUM_TABLE[IDButton] != 0xFF)
		return BUTTON_TYPE_NUM;
	if((IDButton >= F1_SCAN_CODE && IDButton <= F10_SCAN_CODE) ||
	   IDButton == F11_SCAN_CODE || IDButton == F12_SCAN_CODE)
		return BUTTON_TYPE_F;
	if(IDButton == KB_TAB || IDButton == KB_ENTER || IDButton == KB_L_ALT || IDButton == KB_ESCAPE ||
	   IDButton == KB_L_SHIFT || IDButton == KB_R_SHIFT || IDButton == KB_NUM_LOCK ||
	   IDButton == KB_BACKSPACE || IDButton == KB_CAPS_LOCK || IDButton == KB_L_CONTROL ||
	   IDButton == KB_SCROLL_LOCK)
		return BUTTON_TYPE_CONTROL;
	
	return BUTTON_TYPE_NOTH;
}

void KeyboardAction::ProcessScanCode(WORD ScanCode){
	IDButton = ScanCode & 0xFF7F; // without first bit (pressed/released)
	ButtonType = GetButtonType();
	iTypeOfAction = ((ScanCode & 0x0080) == 0 ? TYPE_ACTION_PRESSED : TYPE_ACTION_RELEASED);
}

KeyboardAction::KeyboardAction(){
	ButtonType = BUTTON_TYPE_NOTH;
	iTypeOfAction = TYPE_ACTION_NO_ACTION;
}

KeyboardAction::KeyboardAction(WORD ScanCode){
	Init(ScanCode);
}

int KeyboardAction::Init(WORD ScanCode){
	ProcessScanCode(ScanCode);
	return 0;
}

KeyboardAction::KeyboardAction(const KeyboardAction& ka){
	ButtonType = ka.ButtonType;
	IDButton = ka.IDButton;
	iTypeOfAction = ka.iTypeOfAction;
}

bool KeyboardAction::fHasAction(){
	return (iTypeOfAction != TYPE_ACTION_NO_ACTION && ButtonType != BUTTON_TYPE_NOTH);
}

bool KeyboardAction::fButtonPressed(){
	return (iTypeOfAction == TYPE_ACTION_PRESSED);
}
bool KeyboardAction::fButtonReleased(){
	return (iTypeOfAction == TYPE_ACTION_RELEASED);
}
char KeyboardAction::GetASCII_LETTER(){
	if(ButtonType != BUTTON_TYPE_ASCII_LETTER)
		return 0;
	return ASCII_LETTERS_TABLE[IDButton];
}
char KeyboardAction::GetASCII_NUM_inchar(){
	if(ButtonType != BUTTON_TYPE_NUM)
		return 0;
	return NUM_TABLE_CHAR[IDButton];
}
BYTE KeyboardAction::GetASCII_NUM(){
	if(ButtonType != BUTTON_TYPE_NUM)
		return 0;
	return NUM_TABLE[IDButton];
}
char KeyboardAction::GetASCII_SPECIAL(){
	if(ButtonType != BUTTON_TYPE_ASCII_SPECIAL)
		return 0;
	return ASCII_SPECIAL_SYMBOLS_TABLE[IDButton];
}

void KeyboardAction::ReleaseAction(){
	iTypeOfAction = TYPE_ACTION_NO_ACTION;
}

/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */
/*							 CKeyboard							  */
/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */

CKeyboard::CKeyboard(){
	_fRegisterInverts = false;
	Setup(KEYBOARD_PORT);
}

void CKeyboard::RecieveUsersActions(){
	Action = GetUsersActions();
}

KeyboardAction CKeyboard::GetAction() const{
	return Action;
}

void CKeyboard::Invert(){
	_fRegisterInverts = !_fRegisterInverts;
}

bool CKeyboard::GetInvert() const{
	return _fRegisterInverts;
}
char CKeyboard::GetAsciiLetter(){
	char c;
	
	c = Action.GetASCII_LETTER();
	if(!GetInvert())
		return c;
	if(c <= 'Z')
		return (c + dBetweenRegisters);
	else
		return (c - dBetweenRegisters);
}

void CKeyboard::ReleaseAction(){
	Action.ReleaseAction();
}











