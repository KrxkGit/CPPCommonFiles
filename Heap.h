/******************************************************************************
Module:  Heap.h
Notices: Copyright (C) 2016 
Author: Krxk
******************************************************************************/

#pragma once

//���ļ������ඨ�壬�����ظ��������ļ�
//�̳�CHeap�����Զ�����new��delete��������ܿ����ͷŶѣ��ǳ��ʺ�������
//GetCount()���������� �̳��� CHeap����(����operator new�Ĵ���-����operator delete�Ĵ���)

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