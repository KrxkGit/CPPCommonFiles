/******************************************************************************
Module:  List.h
Notices: Copyright (C) 2016 
Author: Krxk
******************************************************************************/

#pragma once

#include"Heap.h"
#include"CmnHdr.h"

//���� RemoveAll()������Heap.hȫ�ֱ���
//��ʱ���ļ�ֻ�ܱ�����һ��

#define EXCEPTION_ARRAY_OVERLOW MAKESOFTWAREEXCEPTION(3,FACILITY_NULL,1)//Խ�������
//��ʹ��
//try {...} 
//catch(DWORD dwExceptionCode) {
//		if(dwException==EXCEPTION_ARRAY_OVERLOW) {
//			/*��Ĵ������*/
//		}
//����Խ���쳣
//�������GetData(...,BOOL bThrowException)��ΪbThrowException��FALSE��Խ�罫����Debugbreak����x86�Ͻ�����int 3 ���ָ��
//���棺GetData��operator[]����Խ���鹦�ܻ���ֻ��һ�����������ڣ����Ҳ���������CHeap�����CHeap������Ķ���
//������������������Խ���鹦�ܽ������ã���ʱ�뱣֤�����Ϸ�
//���ǻ���GetFirst()��GetNext()

//ע�⣺Remove()��RemoveAll()������ɾ�������¼����ɾ��Ԫ��
//AddTo*�������ǽ�Ԫ���������������¼
//����ɾ��Ԫ�أ����ڴ���ɾ����,�����Empty*����

//���棺Empty*������������ʹ��new�����������Ԫ�أ��罫��ջ�еı�����������������ø�ϵ�к��������򽫷�������
//ʹ��Empty*������ȷ��һ��Ԫ����һ��new���������ܡ�ͨ��newһ�����飬��Ϊ����ÿһ��Ԫ�ص���AddTo*��
//��C++�����������������������粻ʹ��Ĭ�϶ѣ���������new��delete������
//Empty*����ע������϶࣬�粻�ܱ�֤��������һ�㣬�������Empty*����

//Remove*��Empty*���������� TYPEָ��ֵ ����Ԫ��(��ͬһ��������������ͬ�Ĳ�ͬ�������������)���������Զ�λ��Ψһ��Ԫ�ء�
//GetData()��operator[]������Ҫͨ����������������Ԫ�أ��ٶȽ�����������������뻻��GetFirst()��GetNext()

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

	if(m_Head == NULL) {//��һ��Ԫ��
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

	if(m_Tail == NULL) {//��һ��Ԫ��
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
			DebugBreak();//������鷵��ֵ���п��ܵ���ʧ��
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
			if(p==m_Head && p==m_Tail) {//ֻ��һ��Ԫ�ص�����
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
	if(bFromHead) {//��ͷ����
		m_PrintFromHead = TRUE;
		m_pCurrent=m_Head;
		return m_Head->m_pObject;
	}
	//��ĩβ����
	m_PrintFromHead = FALSE;
	m_pCurrent=m_Tail;
	return m_Tail->m_pObject;

}

template<class TYPE>
TYPE* CList<TYPE>::GetNext()
{
	if(m_Head == NULL && m_Tail == NULL) {//������
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