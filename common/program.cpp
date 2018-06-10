#include "program.h"

/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */
/*							  CProgram							  */
/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */

CProgram::CProgram(){
	_Console.SetOtherModules(&_CacheVisual, &_Tab, &_Cache);
	_Cache.SetOtherModules(&_CacheVisual, &_Tab);
	_BGView.ChangePrintView(0, 0, N_COL - 1, N_ROW - 1);
}

bool CProgram::ProgramLoop(){
	_BGView.PrintRect(0, 0, N_COL, N_ROW, BLUE, BLUE, ' ');
	
	Render();
	RecieveUsersActions();
	ProcessUsersActions();
	return true;
}


void CProgram::RecieveUsersActions(){
	_Keyboard.RecieveUsersActions();
}

void CProgram::ProcessUsersActions(){
	KeyboardAction Action = _Keyboard.GetAction();
	
	if(!Action.fHasAction())
		return;
	
	if(Action.fButtonReleased()){
		if(Action.ButtonType == BUTTON_TYPE_CONTROL){
			if(Action.IDButton == KB_L_SHIFT || Action.IDButton == KB_R_SHIFT){
				_Keyboard.Invert();
			}
		}
		return;
	}
	
	
	if(Action.fButtonPressed()){
		switch (Action.ButtonType) {
			case BUTTON_TYPE_ASCII_LETTER:
			DestributeAsciiLetter();
			break;
			case BUTTON_TYPE_ASCII_SPECIAL:
			DestributeAsciiSpecial();
			break;
			case BUTTON_TYPE_NUM:
			DestributeNum();
			break;
			case BUTTON_TYPE_CONTROL:
			DestributeControl();
			break;
		}
		_Keyboard.ReleaseAction();
	}
}

void CProgram::Render(){
	_Tab.Render();

	_Console.Render(_Focus.fCrtFocusIsConsole());

	_Cache.Render();
}

void CProgram::DestributeAsciiLetter(){
	char c;
	c = _Keyboard.GetAsciiLetter();
	
	if(_Focus.fCrtFocusIsConsole()){
		_Console.AddNewLetter(c);
	}
}

void CProgram::DestributeAsciiSpecial(){
	char c;
	KeyboardAction Action;
	Action = _Keyboard.GetAction();
	
	c = Action.GetASCII_SPECIAL();

	if(_Focus.fCrtFocusIsConsole()){
		_Console.AddNewLetter(c);
	}
}

void CProgram::DestributeNum(){
	char c;
	KeyboardAction Action;
	Action = _Keyboard.GetAction();
	
	c = Action.GetASCII_NUM_inchar();
	
	if(_Focus.fCrtFocusIsConsole()){
		_Console.AddNewLetter(c);
	}
}

void CProgram::DestributeControl(){
	WORD IDButton;
	KeyboardAction Action;
	Action = _Keyboard.GetAction();
	IDButton = Action.IDButton;
	
	switch (IDButton) {
		case KB_TAB:
		_Focus.SetNextFocus();
		break;
		case KB_ENTER:
		if(_Focus.fCrtFocusIsConsole()){
			_Console.Enter();
		}
		break;
		case KB_L_ALT:break;
		case KB_ESCAPE:break;
		case KB_L_SHIFT:
		_Keyboard.Invert();
		break;
		case KB_R_SHIFT:
		_Keyboard.Invert();
		break;
		case KB_ARROW_UP:
		if(_Focus.fCrtFocusIsTab()){
			_Cache.Up(_Keyboard.GetInvert());
		}
		if(_Focus.fCrtFocusIsConsole()){
			_Cache.Up(_Keyboard.GetInvert());
		}
		break;
		case KB_NUM_LOCK:break;
		case KB_BACKSPACE:
		if(_Focus.fCrtFocusIsConsole()){
			_Console.Backspace();
		}
		break;
		case KB_CAPS_LOCK:
		_Keyboard.Invert();
		break;
		case KB_L_CONTROL:break;
		case KB_ARROW_DOWN:
		if(_Focus.fCrtFocusIsTab()){
			_Cache.Down(_Keyboard.GetInvert());
		}
		if(_Focus.fCrtFocusIsConsole()){
			_Cache.Down(_Keyboard.GetInvert());
		}
		break;
		case KB_ARROW_LEFT:
		if(_Focus.fCrtFocusIsTab()){
			_Tab.Left();
		}
		if(_Focus.fCrtFocusIsConsole()){
			_Console.Left();
		}
		break;
		case KB_ARROW_RIGHT:
		if(_Focus.fCrtFocusIsTab()){
			_Tab.Right();
		}
		if(_Focus.fCrtFocusIsConsole()){
			_Console.Right();
		}
		break;
		case KB_SCROLL_LOCK:break;
	}
}

/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */
/*						   CProgramCache							*/
/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */


CProgramCache::CProgramCache(){
	_CacheView.ChangePrintView(CX_BEF_MEM_VIEW, CY_BEF_MEM_VIEW,
							   N_COL - CX_AFT_MEM_VIEW - 1, N_ROW - 1);
	
	_iSetWithAction = -1;
	_iLineWithAction = -1;
	
	for(int iSet = 0; iSet < N_SETS; ++iSet){
		_cLinesOffsetUp[iSet] = 0;
	}
}

void CProgramCache::SetOtherModules(CCacheVisual *pCacheVisual, CProgramTab *pTab){
	_pCacheVisual = pCacheVisual;
	_pTab = pTab;
}

void CProgramCache::SetNewAction(int iSet, int iLine){
	_iSetWithAction = iSet;
	_iLineWithAction = iLine;
}

void CProgramCache::Up(const bool fFast){
	int iSet = _pTab->GetCrtSet();
	
	if(_cLinesOffsetUp[iSet] > 0){
		if(fFast){
			_cLinesOffsetUp[iSet] -= 2;
			if(_cLinesOffsetUp[iSet] < 0){
				_cLinesOffsetUp[iSet] = 0;
			}
		}
		else{
			--_cLinesOffsetUp[iSet];
		}
	}
}

void CProgramCache::Down(const bool fFast){
	int iSet = _pTab->GetCrtSet();
	
	if(_cLinesOffsetUp[iSet] / 3 < N_LINES_IN_SET - 1){
		if(fFast){
			_cLinesOffsetUp[iSet] += 2;
			if(_cLinesOffsetUp[iSet] / 3 >= N_LINES_IN_SET - 1){
				_cLinesOffsetUp[iSet] = (N_LINES_IN_SET - 1) * 3 - 1;
			}
		}
		else{
			++_cLinesOffsetUp[iSet];
		}
	}
}

void CProgramCache::SetCrtLine(int iLine){
	_cLinesOffsetUp[_pTab->GetCrtSet()] = 3 * iLine;
}

void CProgramCache::Render(){
	char szBuf[CB_CACHE_LINE_SIZE + 1] = {0};
	XCacheLineInfo Info;
	int iSet;
	//
	const BYTE BGColour = BLUE;
	const TCOLOUR ColourOfLineWithAction = CreateColour(CYAN, BRIGHT_YELLOW),
					StdColour = CreateColour(BGColour, WHITE);
	//
	iSet = _pTab->GetCrtSet();
	
	_CacheView.ClearView();
	_CacheView.PrintRect(0, 0, _CacheView.GetViewMetrics().x, _CacheView.GetViewMetrics().y,
						 BGColour, BGColour, ' ');
	
	_CacheView.ChangePrintPoint(1, -_cLinesOffsetUp[iSet] + 1);
	_CacheView.SetColour(StdColour);
	
	for(int iLine = 0; iLine < N_LINES_IN_SET; ++iLine){
		_CacheView.SetColour(StdColour);
		if(iSet == _iSetWithAction && iLine == _iLineWithAction){
			_CacheView.SetColour(ColourOfLineWithAction);
			_CacheView.PrintRect(0, _CacheView.GetCrtY(),
								 _CacheView.GetViewMetrics().x, _CacheView.GetCrtY() + 2,
								 CYAN, CYAN, ' ');
		}
		
		_pCacheVisual->GetCacheLineInfo(&Info, iSet, iLine);
		
		if(Info.fIsEmpty){
			_CacheView.Printf("   # 0x%X Line\tEMPTY\n-\n-\n-", iLine);
		}
		else{
			_pCacheVisual->GetStringOfCacheLine(szBuf, iSet, iLine);
			_CacheView.Printf("   # 0x%X Line\tAddress == %X\tModified == %s\n ",
							  iLine, Info.StartAddr, Info.fModified ? "True" : "False");
			
			for(int iPart = 0; iPart < 2; ++iPart){
				_CacheView.Printf("  ");
				for(int iQuart = 0; iQuart < ((CB_CACHE_LINE_SIZE / 2) / 4); ++iQuart){
					for(int iByte = 0; iByte < 4; ++iByte){
						char cHigh, cLow;
						BYTE Byte = szBuf[iPart * 32 + iQuart * 4 + iByte];

						cHigh = Byte >> 4;
						cLow = Byte & 0xF;
						
						cHigh = cHigh >= 10 ? (cHigh - 10 + 'A') : (cHigh + '0');
						cLow =  cLow >=  10 ? (cLow - 10 + 'A') : (cLow + '0');
						
						_CacheView.Printf("%c%c", cHigh, cLow);
					}
					_CacheView.Printf(" ");
				}
				
				_CacheView.Printf("\t\t\n ");
			}
		}
		
	}
	
	_CacheView.FrameView(BGColour, WHITE);
}




/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */
/*						   CProgramTab							  */
/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */

CProgramTab::CProgramTab(){
	_iCrtTab = 0;
	_iCrtTabWithAction = -1;
	
	_TabView.ChangePrintView(CX_BEF_TAB_VIEW, CY_BEF_TAB_VIEW,
							 N_COL - CX_AFT_TAB_VIEW - 1, CY_BEF_TAB_VIEW + CY_TAB_VIEW_SIZE - 1);
	_TabView.ChangePrintPoint(0, 0);
}

int CProgramTab::GetCrtSet() const{
	return _iCrtTab;
}

void CProgramTab::SetNewAction(int iSet){
	_iCrtTabWithAction = iSet;
}

int CProgramTab::GetSetWithAction() const{
	return _iCrtTabWithAction;
}

bool CProgramTab::fIsActionInAnySet() const{
	return (_iCrtTabWithAction == -1);
}

void CProgramTab::Left(){
	if(_iCrtTab > 0){
		--_iCrtTab;
	}
}

void CProgramTab::Right(){
	if(_iCrtTab < N_SETS - 1){
		++_iCrtTab;
	}
}

int CProgramTab::SetCrtSet(int iSet){
	return _iCrtTab = iSet;
}

void CProgramTab::Render(){
	const int cxPrintLen = 5;
	const int cxBetweenPrints = (N_COL - cxPrintLen * N_SETS) / (N_SETS - 1);
	//
	const BYTE BGColour = GREY;
	const TCOLOUR ColourOfCrtSet = CreateColour(BLUE, WHITE),
	StdColour = CreateColour(BGColour, BLACK);
	//
	_TabView.PrintRect(0, 0, _TabView.GetViewMetrics().x, _TabView.GetViewMetrics().y,
					   BGColour, BGColour, ' ');
	
	_TabView.SetColour(StdColour);
	
	if(0 == _iCrtTabWithAction){
		_TabView.ChangePrintPoint(2, 0);
	}
	else{
		_TabView.ChangePrintPoint(3, 0);
	}
	
	for(int iSet = 0; iSet < N_SETS; ++iSet){
		if(iSet == _iCrtTab){
			_TabView.SetColour(ColourOfCrtSet);
		}
		
		if(iSet == _iCrtTabWithAction)
			_TabView.Printf("*Set*%d*", iSet);
		else
			_TabView.Printf("Set %d", iSet);

		_TabView.SetColour(StdColour);
		if(iSet == _iCrtTabWithAction || iSet + 1 == _iCrtTabWithAction){
			_TabView.PutNSymbols(' ', cxBetweenPrints - 1);
		}
		else{
			_TabView.PutNSymbols(' ', cxBetweenPrints);
		}
		
	}
}





/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */
/*						 CNewInstruction							*/
/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */

CNewInstruction::CNewInstruction(){
	cInstructionLen = 0;
}

/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */
/*						  CProgramConsole						   */
/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */

CProgramConsole::CProgramConsole(){
	_ConsoleView.ChangePrintView(CX_BEF_CONSOLE_VIEW, CY_BEF_CONSOLE_VIEW,
								 N_COL - CX_AFT_CONSOLE_VIEW - 1, CY_BEF_CONSOLE_VIEW + CY_CONSOLE_VIEW_SIZE - 1);
	_ConsoleView.ChangePrintPoint(0, 0);
}

void CProgramConsole::SetOtherModules(CCacheVisual *pCacheVisual, CProgramTab *pTab, CProgramCache *pCache){
	_pCacheVisual = pCacheVisual;
	_pTab = pTab;
	_pCache = pCache;
}

void CProgramConsole::AddNewLetter(char c){
	_ConsoleView.AddNewLetter(c);
}

void CProgramConsole::Left(){
	_ConsoleView.Left();
}

void CProgramConsole::Right(){
	_ConsoleView.Right();
}

void CProgramConsole::Backspace(){
	_ConsoleView.BackSpace();
}

void CProgramConsole::Enter(){
	_ConsoleView.CommandWasDone(_NewInstruction.szInstruction);
	_NewInstruction.cInstructionLen = strlen(_NewInstruction.szInstruction);
	ProcessNewInstruction();
}

void CProgramConsole::Render(const bool fIsConsoleActive){
	_ConsoleView.PrintRect(_ConsoleView.GetPrintRect(), GREY, BLACK, ' ');
	_ConsoleView.RenderConsole(fIsConsoleActive);
}

void CProgramConsole::ProcessNewInstruction(){
	u32 NewAddr;
	char* szInstruction;
	size_t cInstructionLen;
	XCacheLineInfo Info;
	
	const char szInstructionAddNewAddress[] = "read";
	const size_t cAddNewAddressInstructionLen = strlen(szInstructionAddNewAddress);
	
	const char szInstructionSetValute[] = "write";
	const size_t cSetValueInstructionLen = strlen(szInstructionSetValute);
	
	const char szInstructionFlush[] = "flush";
	const size_t cFlushInstructionLen = strlen(szInstructionFlush);
	
	char szAnswerUnknownInstruction[] = "Unknown instruction";
	const size_t cUnknownInstructionAnswerLen = strlen(szAnswerUnknownInstruction);
	
	const char szAnswerWrongAddress[] = "Wrong address";
	const size_t cWrongAddressAnswerLen = strlen(szAnswerWrongAddress);
	
	const char szAnswerDone[] = "Done";
	const size_t cDoneAnswerLen = strlen(szAnswerDone);
	//
	szInstruction = _NewInstruction.szInstruction;
	cInstructionLen = _NewInstruction.cInstructionLen;
	
	
	if(cInstructionLen >= cAddNewAddressInstructionLen &&
	   fEqualStrings(szInstructionAddNewAddress, szInstruction, cAddNewAddressInstructionLen)){
		if(atouh(szInstruction + cAddNewAddressInstructionLen + 1, &NewAddr)){
			_pCacheVisual->AddNewAddress(NewAddr);
			_pCacheVisual->GetCacheLineInfo(&Info, NewAddr);
			
			_pTab->SetNewAction(Info.iSet);
			_pCache->SetNewAction(Info.iSet, Info.iLine);
			
			_pTab->SetCrtSet(Info.iSet);
			_pCache->SetCrtLine(Info.iLine);
			
			_ConsoleView.AddAnswer(szAnswerDone, cDoneAnswerLen);
		}
		else{
			_ConsoleView.AddAnswer(szAnswerWrongAddress, cWrongAddressAnswerLen);
		}
		
		return;
	}
	if(cInstructionLen >= cSetValueInstructionLen &&
	   fEqualStrings(szInstructionSetValute, szInstruction, cSetValueInstructionLen)){
		if(atouh(szInstruction + cSetValueInstructionLen + 1, &NewAddr)){
			*((BYTE*)NewAddr) = ~*((BYTE*)NewAddr);
			_pCacheVisual->AddNewAddress(NewAddr);
						_pCacheVisual->ValueChanged(NewAddr);
			_pCacheVisual->GetCacheLineInfo(&Info, NewAddr);
			
			_pTab->SetNewAction(Info.iSet);
			_pCache->SetNewAction(Info.iSet, Info.iLine);
			_pTab->SetCrtSet(Info.iSet);
			_pCache->SetCrtLine(Info.iLine);
			
			_ConsoleView.AddAnswer(szAnswerDone, cDoneAnswerLen);
		}
		else{
			_ConsoleView.AddAnswer(szAnswerWrongAddress, cWrongAddressAnswerLen);
		}
		return;
	}
	if(cInstructionLen >= cFlushInstructionLen &&
	   fEqualStrings(szInstructionFlush, szInstruction, cFlushInstructionLen)){
		_pCacheVisual->FlushCache();
		
		_pTab->SetNewAction(-1);
		_pCache->SetNewAction(-1, -1);
		
		_ConsoleView.AddAnswer(szAnswerDone, cDoneAnswerLen);
		return;
	}

	_ConsoleView.AddAnswer(szAnswerUnknownInstruction, cUnknownInstructionAnswerLen + 20);
	return;
}

/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */
/*							   CFocus							   */
/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */

CFocus::CFocus(){
	_CrtFocus = TAB;
}

bool CFocus::fCrtFocusIsTab() const{
	return (_CrtFocus == TAB);
}

bool CFocus::fCrtFocusIsConsole() const{
	return (_CrtFocus == CONSOLE);
}

void CFocus::SetNextFocus(){
	if(_CrtFocus == TAB){
		_CrtFocus = CONSOLE;
		return;
	}
	if(_CrtFocus == CONSOLE){
		_CrtFocus = TAB;
		return;
	}
}







































