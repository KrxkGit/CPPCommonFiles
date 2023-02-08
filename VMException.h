/******************************************************************************
Module:  VMException.h
Notices: Copyright (C) 2016 
Author: Krxk
******************************************************************************/

#pragma once

//��ͷ�ļ������ඨ�壬ֻ�ܰ�����ͷ�ļ�һ��

//����CVMException�������Ч
//����������ʧЧ
//CVMException g_VmException;
//PINT p=VirtualAlloc(...) ֻԤ������
//p[i]=4 CVMException::MyUnhandledExceptionFilter������,�ܳɹ�����p[i]������ʱ��Ҫ���ĵ���ѡ��

class CVMException
{
protected:
	static LPTOP_LEVEL_EXCEPTION_FILTER m_Unhandledpfn;
	static UINT sm_nCount;
	static LONG CALLBACK MyUnhandledExceptionFilter(PEXCEPTION_POINTERS);
public:
	CVMException();
	~CVMException();
};

inline CVMException::CVMException()
{
	if(sm_nCount == 0) {
		m_Unhandledpfn = SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
	}
	sm_nCount++;
}

inline CVMException::~CVMException()
{
	sm_nCount--;
	if(sm_nCount == 0) {
		SetUnhandledExceptionFilter(m_Unhandledpfn);
	}
}

LONG CALLBACK CVMException::MyUnhandledExceptionFilter(PEXCEPTION_POINTERS pep)
{
	PEXCEPTION_RECORD per = pep->ExceptionRecord;
	DWORD dwExceptionCode = per->ExceptionCode;
	if(dwExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
		SYSTEM_INFO sinf;
		GetSystemInfo(&sinf);
		return (NULL==VirtualAlloc((LPVOID)per->ExceptionInformation[1],sinf.dwPageSize,MEM_COMMIT,PAGE_READWRITE))? EXCEPTION_CONTINUE_SEARCH:
			EXCEPTION_CONTINUE_EXECUTION;
	}
	return m_Unhandledpfn(pep);
}

LPTOP_LEVEL_EXCEPTION_FILTER CVMException::m_Unhandledpfn=NULL;
UINT CVMException::sm_nCount=0;