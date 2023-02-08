/******************************************************************************
Module:  Win32Trace.h
Notices: Copyright (C) 2016 
Author: Krxk
******************************************************************************/

#pragma once

//��ͷ�ļ����к������壬ֻ�ܰ�����ͷ�ļ�һ��
#define MAX_CCH_STRING_SIZE 1024 //�Զ����������ַ�������


#ifdef _DEBUG
#define TRACE DebugTrace

#else
#define ReleaseTRACE(LPCTSTR, ...)
#define TRACE ReleaseTRACE

#endif

LPTSTR szDebugString = (LPTSTR)VirtualAlloc(NULL,MAX_CCH_STRING_SIZE*sizeof(TCHAR),
											MEM_RESERVE | MEM_TOP_DOWN | MEM_COMMIT,
											PAGE_READWRITE);

VOID _stdcall DebugTrace(LPCTSTR szTrace, ...)
{
	va_list valist;
	va_start(valist,szTrace);
	_vstprintf(szDebugString,MAX_CCH_STRING_SIZE,szTrace,valist);
	va_end(valist);

	OutputDebugString(szDebugString);
}