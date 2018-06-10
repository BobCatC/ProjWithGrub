#ifndef _CONSOLE_H
#define _CONSOLE_H

#include "types.h"
#include "stdarg.h"
#include "printf.h"
#include "screen.h"
#include "memory.h"

#define CMAX_COMMAND_LEN_NO_ZERO	   	63
#define CMAX_ANSWER_LEN_NO_ZERO			31
#define NMAX_COMMANDS_REMEMBER		 	24
#define NMAX_RECORDS_REMEMBER		  	6

class XConsoleData{
private:
	char _szRequest[CMAX_COMMAND_LEN_NO_ZERO + 1];
	int _cRequest;
	char _szPrevAnswer[CMAX_ANSWER_LEN_NO_ZERO + 1];
	int _cAnswer;
	
	int _iCrtSymbol;
	
	void StartUp(){
		_cAnswer = _cRequest = 0;
		_szRequest[0] = _szPrevAnswer[0] = '\0';
		_iCrtSymbol = 0;
	}
	void Refresh(){
		_szRequest[0] = '\0';
		_cRequest = 0;
		_iCrtSymbol = 0;
	}
public:
	XConsoleData(){
		StartUp();
	}
	
	void AddNewLetter(const char c){
	
		for(int i = _cRequest + 1; i > _iCrtSymbol; --i)
			_szRequest[i] = _szRequest[i - 1];

		_szRequest[_iCrtSymbol] = c;
		if(_cRequest < CMAX_COMMAND_LEN_NO_ZERO - 1)
			++_cRequest;
		if(_iCrtSymbol < CMAX_COMMAND_LEN_NO_ZERO - 1)
			++_iCrtSymbol;
	}
	
	void BackSpace(){
		if(_iCrtSymbol > 0){
			for(size_t i = _iCrtSymbol; i <= _cRequest; ++i)
				_szRequest[i - 1] = _szRequest[i];
			--_cRequest;
			--_iCrtSymbol;
		}
	}
	
	void Left(){
		if(_iCrtSymbol > 0)
			--_iCrtSymbol;
	}
	void Right(){
		if(_iCrtSymbol < _cRequest)
			++_iCrtSymbol;
	}
	
	void InstructionDone(__OUT__ char* szDoneInstruction){
		memcpy(szDoneInstruction, _szRequest, sizeof(char) * (_cRequest + 1));
	}
	void SetPrevAnswer(__IN__ const char* szAnswer, size_t cAnswerSize){
		memcpy(_szPrevAnswer, szAnswer, sizeof(char) * (cAnswerSize + 1));
		_cAnswer = cAnswerSize;
		_szPrevAnswer[_cAnswer] = '\0';
	}
	size_t GetPrevAnswer(char** pszAnswer){
		*pszAnswer = _szPrevAnswer;
		return _cAnswer;
	}
	size_t GetCrtInstruction(char** pszInstruction){
		*pszInstruction = _szRequest;
		return _cRequest;
	}
	size_t GetCrtSymbol()const{
		return _iCrtSymbol;
	}
};


class XConsoleView : public XView{
private:
	XConsoleData _Data;
	
protected:
	//void PutC1(const char c);
public:
	void AddNewLetter(const char c);
	void CommandWasDone(__OUT__ char* szDoneCommand);
	void AddAnswer(__IN__ const char* szAnswer, size_t cAnswerLen);
	void BackSpace();
	void Left();
	void Right();
	
	XConsoleView();
	
	void RenderConsole(const bool fIsConsoleActive);
};


#endif















