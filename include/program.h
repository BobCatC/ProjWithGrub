#ifndef _RENDER_H
#define _RENDER_H

#include "screen.h"
#include "console_view.h"
#include "interface.h"
#include "std_def.h"
#include "memory.h"
#include "cache.h"
#include "types.h"

#define CMAX_COMMAND_LEN					0x70
#define CMAX_COMMAND_REMEMBER			   	0x20

#define CY_BEF_TAB_VIEW					 	0x0
#define CX_BEF_TAB_VIEW						0x0
#define CX_AFT_TAB_VIEW					 	0x0
#define CY_TAB_VIEW_SIZE					0x1

#define CY_BETWEEN_TAB_AND_CONSOLE		  	0x0

#define CY_BEF_CONSOLE_VIEW				 	(CY_BEF_TAB_VIEW + CY_TAB_VIEW_SIZE + CY_BETWEEN_TAB_AND_CONSOLE)
#define CX_BEF_CONSOLE_VIEW				 	0x1
#define CX_AFT_CONSOLE_VIEW				 	0x1
#define CY_CONSOLE_VIEW_SIZE				0x4

#define CY_BETWEEN_CONSOLE_AND_MEM		  	0x0

#define CY_BEF_MEM_VIEW					 	(CY_BEF_CONSOLE_VIEW + CY_CONSOLE_VIEW_SIZE + CY_BETWEEN_CONSOLE_AND_MEM)
#define CX_BEF_MEM_VIEW					 	0x1
#define CX_AFT_MEM_VIEW					 	0x1
#define N_CACHE_LINES_IN_SCREEN			 	(MAX_ROW - I_START_CACHE_VIEW)



class CProgramTab{
private:
	XView _TabView;
	
	int _iCrtTab;
	int _iCrtTabWithAction;
public:
	CProgramTab();
	
	int GetCrtSet() const;
	int SetCrtSet(int iSet);
	
	void SetNewAction(int iSet);
	int GetSetWithAction() const;
	bool fIsActionInAnySet() const;
	
	void Left();
	void Right();
	
	void Render();
};


class CProgramCache{
private:
	XView _CacheView;
	CCacheVisual* _pCacheVisual;
	CProgramTab* _pTab;
	
	int _iSetWithAction, _iLineWithAction;
	
	int _cLinesOffsetUp[N_SETS];
	
public:
	CProgramCache();
	void SetOtherModules(CCacheVisual* pCacheVisual, CProgramTab* pTab);
	
	void SetNewAction(int iSet, int iLine);
	
	void Up(const bool fFast);
	void Down(const bool fFast);
	void SetCrtLine(int iLine);
	
	void Render();
};


class CNewInstruction{
public:
	char szInstruction[CMAX_COMMAND_LEN + 1];
	size_t cInstructionLen;
	
	CNewInstruction();
};

class CProgramConsole{
private:
	XConsoleView _ConsoleView;
	CNewInstruction _NewInstruction;
	
	CCacheVisual* _pCacheVisual;
	CProgramTab* _pTab;
	CProgramCache* _pCache;
	
	
	void ProcessNewInstruction();
public:
	CProgramConsole();
	void SetOtherModules(CCacheVisual* pCacheVisual, CProgramTab* pTab, CProgramCache* pCache);
	
	void AddNewLetter(char c);
	
	void Left();
	void Right();
	
	void Backspace();
	void Enter();
	
	void Render(const bool fIsConsoleActive);
};


enum EFocus{
	TAB, CONSOLE
};

class CFocus{
private:
	EFocus _CrtFocus;
public:
	CFocus();
	
	bool fCrtFocusIsTab() const;
	bool fCrtFocusIsConsole() const;
	
	void SetNextFocus();
};

class CProgram{
private:
	CKeyboard _Keyboard;
	
	XView _BGView;
	CProgramTab _Tab;
	CProgramConsole _Console;
	CProgramCache _Cache;
	
	CFocus _Focus;
	
	CCacheVisual _CacheVisual;
	
	
	void RecieveUsersActions();
	void ProcessUsersActions();
	void Render();
	
	void DestributeAsciiLetter();
	void DestributeAsciiSpecial();
	void DestributeNum();
	void DestributeControl();

public:
	CProgram();
	
	bool ProgramLoop();

};

#endif


































