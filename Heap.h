/******************************************************************************
Module:  Heap.h
Notices: Copyright (C) 2016 
Author: Krxk
******************************************************************************/

#pragma once

//本文件含有类定义，不能重复包含本文件
//继承CHeap的类自动重载new与delete运算符，能快速释放堆，非常适合于链表
//GetCount()函数将返回 继承自 CHeap的类(调用operator new的次数-调用operator delete的次数)

class CHeap
{
public:
	LPVOID operator new(size_t);
	VOID operator delete(LPVOID);
	UINT GetCount() {return sm_nCount;}
protected:
	static UINT sm_nCount;
	static HANDLE sm_hHeap;
};


inline LPVOID CHeap::operator new(size_t size)
{
	if(sm_nCount == 0) {
		sm_hHeap=HeapCreate(0,0,0);
	}
	LPVOID pClass = HeapAlloc(sm_hHeap,0,size);
	if(pClass != NULL) {
		sm_nCount++;
		return pClass;
	}
	return NULL;
}

inline VOID CHeap::operator delete(LPVOID p)
{
	HeapFree(sm_hHeap,0,p);
	sm_nCount--;
	if(sm_nCount == 0) {
		HeapDestroy(sm_hHeap);
	}
}

UINT CHeap::sm_nCount=0;
HANDLE CHeap::sm_hHeap=NULL;