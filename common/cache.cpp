#include "cache.h"
#include "screen.h"
int g_TimeWriteCounter = 0;

/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */
/*						   CCacheVisual							 */
/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */

CCacheVisual::CCacheVisual(){
	for(int FrameIndex = 0; FrameIndex <  N_LINES_IN_SET; ++FrameIndex)
		_Frames[FrameIndex].SetFrameIndex(FrameIndex);
	
	FlushCache();
	
	_start_up();
	_flush_cache();
}

int CCacheVisual::GetFrameOfAddr(u32 Addr){
	return ((Addr % (CB_SET_SIZE)) / CB_CACHE_LINE_SIZE);
}

void CCacheVisual::GetCacheLineInfo(XCacheLineInfo *pInfo, int iSet, int iLine){
	_Frames[iLine].GetCacheLineInfo(pInfo, iSet);
}

bool CCacheVisual::GetCacheLineInfo(XCacheLineInfo *pInfo, u32 Addr){
	int iFrame, iSet;
	iFrame = GetFrameOfAddr(Addr);
	
	iSet = _Frames[iFrame].GetSetOfAddr(Addr);
	
	if(iSet == -1){
		return false;
	}
	
	GetCacheLineInfo(pInfo, iSet, iFrame);
	return true;
}

void CCacheVisual::GetStringOfCacheLine(char *p64BytesBuf, int iSet, int iLine){
	_Frames[iLine].GetStringOfCacheLine(p64BytesBuf, iSet);
}

void CCacheVisual::AddNewAddress(u32 Addr){
	int iFrame;
	XCacheLineInfo Info;
	if(GetCacheLineInfo(&Info, Addr) == true){
		return;
	}
	
	iFrame = GetFrameOfAddr(Addr);
	
	_Frames[iFrame].AddNewAddress(Addr);
}

void CCacheVisual::ValueChanged(u32 Addr){
	int iFrame;
	iFrame = GetFrameOfAddr(Addr);
	_Frames[iFrame].ValueChanged(Addr);
}

void CCacheVisual::FlushCache(){
	for(int iFrame = 0; iFrame < N_LINES_IN_SET; ++iFrame)
		_Frames[iFrame].FlushFrame();
}

/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */
/*							CCacheFrame							 */
/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */

int CCacheFrame::FindNextSet(){
	for(int iSet = 0; iSet < N_SETS; ++iSet){
		if(_CacheLines[iSet].fEmpty())
			return iSet;
	}
	
	int iLessTime = -1;
	u32 LessTime = __INT32_MAX__;
	for(int iSet = 0; iSet < N_SETS; ++iSet){
		if(!_CacheLines[iSet].fModified()){
			if(_CacheLines[iSet].GetTimeOfWriteInCache() < LessTime){
				LessTime = _CacheLines[iSet].GetTimeOfWriteInCache();
				iLessTime = iSet;
			}
		}
	}
	if(iLessTime != -1)
		return iLessTime;
	
	int Min, iMin;
	Min = __INT32_MAX__;
	iMin = -1;
	for(int iSet = 0; iSet < N_SETS; ++iSet){
		if(_CacheLines[iSet].fModified()){
			if(_CacheLines[iSet].GetTimeOfLastModified() < Min){
				Min = _CacheLines[iSet].GetTimeOfLastModified();
				iMin = iSet;
			}
		}
	}
	return iMin;
}

CCacheFrame::CCacheFrame(){
	_iCrtFrame = -1;
}

void CCacheFrame::SetFrameIndex(int iCrtFrame){
	_iCrtFrame = iCrtFrame;
	
	for(int iSet = 0; iSet < N_SETS; ++iSet){
		_CacheLines[iSet].SetCacheLineIndex(iSet);
		_CacheLines[iSet].SetCacheLineConstInfo(iCrtFrame, iSet, CB_CACHE_LINE_SIZE);
	}
}

void CCacheFrame::GetCacheLineInfo(XCacheLineInfo* pInfo, int iSet){
	_CacheLines[iSet].GetCacheLineInfo(pInfo);
}

void CCacheFrame::GetStringOfCacheLine(char* p64BytesBuf, int iSet){
	_CacheLines[iSet].GetStringOfCacheLine(p64BytesBuf);
}

void CCacheFrame::AddNewAddress(u32 Addr){
	int iSet;
	iSet = FindNextSet();
	
	_CacheLines[iSet].AddNewAddress(Addr);
}

void CCacheFrame::ValueChanged(u32 Addr){
	int iSet;
	u32 StartAddr;
	StartAddr = (Addr / 64) * 64;
	// Cache line with address MUST be in cache
	for(iSet = 0; iSet < N_SETS; ++iSet){
		if(!_CacheLines[iSet].fEmpty() && _CacheLines[iSet].GetAddr() == StartAddr)
			break;
	}
	
	_CacheLines[iSet].ValueChanged();
}

int CCacheFrame::GetSetOfAddr(u32 Addr){
	int iSet;
	u32 StartAddr;
	StartAddr = (Addr / 64) * 64;
	
	for(iSet = 0; iSet < N_SETS; ++iSet){
		if(!_CacheLines[iSet].fEmpty() && _CacheLines[iSet].GetAddr() == StartAddr)
			break;
	}
	
	return iSet == N_SETS ? -1 : iSet;
}

void CCacheFrame::FlushFrame(){
	for(int iSet = 0; iSet < N_SETS; ++iSet)
		_CacheLines[iSet].FlushLine();
}

/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */
/*							 CCacheLine							 */
/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */

CCacheLine::CCacheLine(){
	_iCrtLine = 0;
}

void CCacheLine::SetCacheLineIndex(int iCrtLine){
	_iCrtLine = iCrtLine;
}

void CCacheLine::SetCacheLineConstInfo(int iFrame, int iSet, u32 cbSize){
	_Info.iSet = iSet;
	_Info.iLine = iFrame;
	_Info.cbSize = cbSize;
}

void CCacheLine::GetCacheLineInfo(XCacheLineInfo* pInfo){
	memcpy(pInfo, &_Info, sizeof(_Info));
}

void CCacheLine::GetStringOfCacheLine(char* p64BytesBuf){
	memcpy(p64BytesBuf, (void*)_Info.StartAddr, CB_CACHE_LINE_SIZE);
}

void CCacheLine::FlushLine(){
	_Info.fIsEmpty = true;
	_Info.fModified = false;
	_Info.StartAddr = -1;
	_Info.TimeOfWriteInCache = -1;
}

bool CCacheLine::fEmpty() const{
	return _Info.fIsEmpty;
}

bool CCacheLine::fModified() const{
	return _Info.fModified;
}

u32 CCacheLine::GetAddr() const{
	return _Info.StartAddr;
}
u32 CCacheLine::GetTimeOfWriteInCache() const{
	return _Info.TimeOfWriteInCache;
}

u32 CCacheLine::GetTimeOfLastModified() const{
	return _Info.TimeOfLastModification;
}

void CCacheLine::AddNewAddress(u32 Addr){
	_Info.fIsEmpty = false;
	_Info.StartAddr = (Addr / 0x40) * 0x40;
	_Info.TimeOfWriteInCache = ++g_TimeWriteCounter;
	
	_add_new_address(Addr);
}

void CCacheLine::ValueChanged(){
	_Info.fModified = true;
}

/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */
/*						  XCacheLineInfo							*/
/*  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  =  */

XCacheLineInfo::XCacheLineInfo(){
	StartAddr = -1;
	TimeOfWriteInCache = -1;
	cbSize = -1;
	
	iSet = -1;
	iLine = -1;
}

XCacheLineInfo::XCacheLineInfo(const XCacheLineInfo& i){
	*this = i;
}

void XCacheLineInfo::operator=(const XCacheLineInfo &i){
	StartAddr = i.StartAddr;
	cbSize = i.cbSize;
	TimeOfWriteInCache = i.TimeOfWriteInCache;
	
	iSet = i.iSet;
	iLine = i.iLine;
}






















