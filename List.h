/******************************************************************************
Module:  List.h
Notices: Copyright (C) 2016 
Author: Krxk
******************************************************************************/

#pragma once

#include"Heap.h"
#include"CmnHdr.h"

//调用 RemoveAll()将引用Heap.h全局变量
//此时本文件只能被包含一次

#define EXCEPTION_ARRAY_OVERLOW MAKESOFTWAREEXCEPTION(3,FACILITY_NULL,1)//越界错误码
//请使用
//try {...} 
//catch(DWORD dwExceptionCode) {
//		if(dwException==EXCEPTION_ARRAY_OVERLOW) {
//			/*你的处理代码*/
//		}
//捕获越界异常
//如果调用GetData(...,BOOL bThrowException)并为bThrowException传FALSE，越界将调用Debugbreak，在x86上将触发int 3 汇编指令
//警告：GetData与operator[]函数越界检查功能基于只有一个链表对象存在，并且不存在其他CHeap对象或CHeap派生类的对象
//如果不符合以上情况，越界检查功能将不可用，此时请保证索引合法
//或是换用GetFirst()与GetNext()

//注意：Remove()与RemoveAll()操作仅删除链表记录，不删除元素
//AddTo*函数仅是将元素在链表中增添记录
//若需删除元素（在内存中删除）,请调用Empty*函数

//警告：Empty*函数仅适用于使用new操作符申请的元素，如将堆栈中的变量加入链表，请勿调用该系列函数，否则将发生错误
//使用Empty*函数请确保一个元素是一个new操作，不能【通过new一个数组，并为数组每一个元素调用AddTo*】
//将C++类加入链表，如需特需操作（如不使用默认堆），请重载new与delete操作符
//Empty*函数注意事项较多，如不能保证以上任意一点，请勿调用Empty*函数

//Remove*与Empty*函数都依据 TYPE指针值 查找元素(即同一个变量【内容相同的不同变量不会混淆】)，这样可以定位到唯一的元素。
//GetData()与operator[]函数需要通过遍历链表来查找元素，速度较慢，如需遍历链表，请换用GetFirst()与GetNext()

template<class TYPE>
class CList:public CHeap
{
protected:
	struct _structObject {
		TYPE* m_pObject;
		_structObject* m_previous;
		_structObject* m_Next;
	};
	_structObject* m_Head;
	_structObject* m_Tail;
private:
	_structObject* m_pCurrent;
	BOOL m_PrintFromHead;
public:
	CList();
	~CList();
	BOOL AddToHead(TYPE* pObject);
	BOOL AddToTail(TYPE* pObject);
	BOOL Remove(TYPE* pObject);
	TYPE* GetFirst(BOOL bFromHead=TRUE);
	TYPE* GetNext();
	TYPE& GetData(UINT pos,BOOL bFromHead=TRUE,BOOL bThrowException=TRUE);
	TYPE& operator[](UINT);
	VOID RemoveAll();
	VOID EmptyList();
	BOOL EmptyNode(TYPE* pObject);
};

template<class TYPE>
CList<TYPE>::CList()
{
	m_Head=NULL;
	m_Tail=NULL;
	m_pCurrent=NULL;
	m_PrintFromHead = TRUE;
}

template<class TYPE>
CList<TYPE>::~CList()
{
	RemoveAll();
}

template<class TYPE>
BOOL CList<TYPE>::AddToHead(TYPE* pObject)
{
	_structObject* p=(_structObject*)operator new(sizeof(_structObject));
	p->m_pObject=pObject;

	if(m_Head == NULL) {//第一个元素
		m_Tail=p;
		m_Head=p;
		p->m_previous = NULL;
		p->m_Next = NULL;
		return TRUE;
	}
	m_Head->m_previous=p;
	p->m_Next=m_Head;
	p->m_previous=NULL;
	m_Head=p;
	return TRUE;
}

template<class TYPE>
BOOL CList<TYPE>::AddToTail(TYPE* pObject)
{
	_structObject* p=(_structObject*)operator new(sizeof(_structObject));
	p->m_pObject=pObject;

	if(m_Tail == NULL) {//第一个元素
		m_Head=p;
		m_Tail=p;
		p->m_previous = NULL;
		p->m_Next = NULL;
		return TRUE;
	}
	m_Tail->m_Next=p;
	p->m_previous=m_Tail;
	m_Tail=p;
	p->m_Next=NULL;
	return TRUE;
}

template<class TYPE>
VOID CList<TYPE>::RemoveAll()
{
	_structObject* pCurrent = m_Head;
	_structObject* pTemp=NULL;
	while(pCurrent!=NULL) {
		pTemp = pCurrent->m_Next;
		operator delete(pCurrent);
		pCurrent=pTemp;
	}
	m_Head=m_Tail=NULL;
	m_pCurrent=NULL;
}

template<class TYPE>
TYPE& CList<TYPE>::GetData(UINT pos,BOOL FromHead,BOOL bThrowException)
{
	_structObject* p=NULL;
	if(pos>sm_nCount) {
		switch (bThrowException)
		{
		case TRUE:
			throw EXCEPTION_ARRAY_OVERLOW;
			break;
		case FALSE:
			DebugBreak();//若不检查返回值，有可能导致失败
			break;
		}
	}
	if(FromHead) {
		p=m_Head;
		for(;pos;pos--) {
			p=p->m_Next;
		}
	}
	else {
		p=m_Tail;
		for(;pos;pos--) {
			p=p->m_previous;
		}
	}
	return *(p->m_pObject);
}

template<class TYPE>
TYPE& CList<TYPE>::operator[](UINT pos)
{
	return GetData(pos,TRUE,TRUE);
}

template<class TYPE>
BOOL CList<TYPE>::Remove(TYPE* pObject)
{
	for(_structObject*p=m_Head;p!=NULL;p=p->m_Next) {
		if(p->m_pObject == pObject) {
			if(p==m_Head && p==m_Tail) {//只有一个元素的链表
				RemoveAll();
				return TRUE;
			}
			else if(p == m_Head) {
				p->m_Next->m_previous=NULL;
				m_Head=p->m_Next;
			}
			else if(p == m_Tail) {
				p->m_previous->m_Next=NULL;
				m_Tail=p->m_previous;
			}
			else {
				p->m_previous->m_Next=p->m_Next;
				p->m_Next->m_previous=p->m_previous;
			}
			operator delete(p);
			return TRUE;
		}
	}
	return FALSE;
}

template<class TYPE>
TYPE* CList<TYPE>::GetFirst(BOOL bFromHead)
{
	if(m_Head == NULL && m_Tail == NULL) {
		return NULL;
	}
	if(bFromHead) {//从头遍历
		m_PrintFromHead = TRUE;
		m_pCurrent=m_Head;
		return m_Head->m_pObject;
	}
	//从末尾遍历
	m_PrintFromHead = FALSE;
	m_pCurrent=m_Tail;
	return m_Tail->m_pObject;

}

template<class TYPE>
TYPE* CList<TYPE>::GetNext()
{
	if(m_Head == NULL && m_Tail == NULL) {//空链表
		return NULL;
	}
	if(m_PrintFromHead) {
		m_pCurrent = m_pCurrent->m_Next;
	}
	else {
		m_pCurrent = m_pCurrent->m_previous;
	}
	if(m_pCurrent == NULL) {
		return NULL;
	}
	return m_pCurrent->m_pObject;
}

template<class TYPE>
VOID CList<TYPE>::EmptyList()
{
	for(_structObject* p=m_Head;p!=NULL;p=p->m_Next) {
		delete p->m_pObject;
		p->m_pObject=NULL;
	}
	RemoveAll();
}

template<class TYPE>
BOOL CList<TYPE>::EmptyNode(TYPE* pObject)
{
	for(TYPE* p=GetFirst();p!=NULL;p=GetNext()) {
		if(p == pObject) {
			Remove(p);
			delete p;
			return TRUE;
		}
	}
	return FALSE;
}