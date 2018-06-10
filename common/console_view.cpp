#include "console_view.h"
static u16* pVRam = (u16*)0xB8000;
static int Counter = 0;
/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */
/*						   XConsoleView							 */
/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */

void XConsoleView::AddAnswer(const char *szAnswer, size_t cAnswerLen){
	_Data.SetPrevAnswer(szAnswer, strlen(szAnswer));
}



void XConsoleView::AddNewLetter(const char c){
	_Data.AddNewLetter(c);
}

void XConsoleView::CommandWasDone(__OUT__ char *szDoneCommand){
	_Data.InstructionDone(szDoneCommand);
}

void XConsoleView::BackSpace(){
	_Data.BackSpace();
}

void XConsoleView::Left(){
	_Data.Left();
}

void XConsoleView::Right(){
	_Data.Right();
}

XConsoleView::XConsoleView(){
}


void XConsoleView::RenderConsole(const bool fIsConsoleActive){
	char* szAnswer, *szInstruction;
	size_t cAnswer, cRequest, iCrtSymbol;
	char c;
	const BYTE BGColour = BLUE;
	ClearView();

	PrintRect(0, 0, _ViewMetrics.x, _ViewMetrics.y, BGColour, BGColour, ' ');
	FrameView(BGColour, WHITE);
	
	TCOLOUR CrtSymbolBGColour = CreateColour(WHITE, BGColour);
	TCOLOUR StdColour = CreateColour(BGColour, WHITE);
	SetColour(StdColour);
	
	cAnswer = _Data.GetPrevAnswer(&szAnswer);
	cRequest = _Data.GetCrtInstruction(&szInstruction);
	iCrtSymbol = _Data.GetCrtSymbol();
	ChangePrintPoint(0, 0);

	ChangePrintPoint(_ViewMetrics.x / 4, _ViewMetrics.y / 2 - 1);
	
	szAnswer[cAnswer] = '\0';
	szInstruction[cRequest] = '\0';
	
	Printf(szAnswer);
	Printf("\n");

	ChangePrintPoint(_ViewMetrics.x / 4, _ViewMetrics.y / 2);
	
	c = szInstruction[iCrtSymbol];
	szInstruction[iCrtSymbol] = '\0';
	
	Printf(szInstruction);
	szInstruction[iCrtSymbol] = c;
	
	if(fIsConsoleActive)
		SetColour(CrtSymbolBGColour);
	
	PutC(c);
	
	if(fIsConsoleActive)
		SetColour(StdColour);

	szInstruction[iCrtSymbol] = c;
	Printf(szInstruction + iCrtSymbol + 1);
}




























