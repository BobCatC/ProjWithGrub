#ifndef _CACHE_H
#define _CACHE_H

#include "types.h"
#include "memory.h"
#include "as_func.h"

#define N_SETS				   	8
#define N_LINES_IN_SET		   	64
#define CB_CACHE_LINE_SIZE	   	64
#define CB_SET_SIZE			  	(N_LINES_IN_SET * CB_CACHE_LINE_SIZE)

struct XCacheLineInfo{
public:
	u32 StartAddr;
	u32 cbSize;
	u32 TimeOfWriteInCache;
	u32 TimeOfLastModification;
	
	bool fModified;
	
	int iSet;
	int iLine;
	
	bool fIsEmpty;
	
	XCacheLineInfo();
	XCacheLineInfo(const XCacheLineInfo& i);
	
	void operator = (const XCacheLineInfo& i);
};

class CCacheLine{
private:
	int _iCrtLine;
	XCacheLineInfo _Info;
	
public:
	CCacheLine();
	
	void SetCacheLineIndex(int iCrtLine);
	
	void SetCacheLineConstInfo(int iFrame, int iSet, u32 cbSize);
	
	void GetCacheLineInfo(XCacheLineInfo* pInfo);
	
	void GetStringOfCacheLine(char* p64BytesBuf);
	
	void FlushLine();
	
	bool fEmpty() const;
	bool fModified() const;
	
	u32 GetAddr() const;
	u32 GetTimeOfWriteInCache() const;
	u32 GetTimeOfLastModified() const;
	
	void AddNewAddress(u32 Addr);
	void ValueChanged();
};

class CCacheFrame{
private:
	int _iCrtFrame;
	CCacheLine _CacheLines[N_SETS];
	
	int FindNextSet();
	
public:
	CCacheFrame();
	
	void SetFrameIndex(int iCrtFrame);
	
	void GetCacheLineInfo(XCacheLineInfo* pInfo, int iSet);
	
	void GetStringOfCacheLine(char* p64BytesBuf, int iSet);
	
	void AddNewAddress(u32 Addr);
	void ValueChanged(u32 Addr);
	
	int GetSetOfAddr(u32 Addr);
	
	void FlushFrame();
};

class CCacheVisual{
private:
	CCacheFrame _Frames[N_LINES_IN_SET];
	
	int GetFrameOfAddr(u32 Addr);
	
public:
	CCacheVisual();
	
	void GetCacheLineInfo(XCacheLineInfo* pInfo, int iSet, int iLine);
	bool GetCacheLineInfo(XCacheLineInfo* pInfo, u32 Addr);
	
	void GetStringOfCacheLine(char* p64BytesBuf, int iSet, int iLine);
	
	void AddNewAddress(u32 Addr);
	void ValueChanged(u32 Addr);
	
	void FlushCache();
};

#endif
