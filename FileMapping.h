/******************************************************************************
Module:  FileMapping.h
Notices: Copyright (C) 2016 
Author: Krxk
******************************************************************************/

#pragma once

//CFileMapping::CFileMapping()把SEC_RESERVE与其他标志按位或起来（除SEC_COMMINT），并把结果传入flProtect
//可结合CVMException类实行稀疏调拨内存映射文件

class CFileMapping
{
public:
	CFileMapping(DWORD,DWORD,LPCTSTR,DWORD,HANDLE,LPSECURITY_ATTRIBUTES);
	~CFileMapping();
	LPVOID MapViewOfFile(DWORD,DWORD,DWORD,SIZE_T,LPVOID);
	BOOL UnmapViewOfFile(LPCVOID);
protected:
	HANDLE m_hFileMapping;
};


inline CFileMapping::CFileMapping(DWORD dwMaximumSizeLow,DWORD dwMaximumSizeHigh=0,LPCTSTR lpName=NULL,DWORD flProtect=PAGE_READWRITE,
								  HANDLE hFile=INVALID_HANDLE_VALUE,LPSECURITY_ATTRIBUTES lpFileMappingAttributes=NULL)
{
	m_hFileMapping=::CreateFileMapping(hFile,lpFileMappingAttributes,flProtect,dwMaximumSizeHigh,dwMaximumSizeLow,lpName);
}

inline CFileMapping::~CFileMapping()
{
	CloseHandle(m_hFileMapping);
}

inline LPVOID CFileMapping::MapViewOfFile(DWORD dwDesiredAccess,DWORD dwFileOffsetHigh,
								   DWORD dwFileOffsetLow,SIZE_T dwNumberOfBytesToMap=0,LPVOID lpBaseAddress=NULL)
{
	return ::MapViewOfFileEx(m_hFileMapping,dwDesiredAccess,dwFileOffsetHigh,dwFileOffsetLow,dwNumberOfBytesToMap,lpBaseAddress);
	
}

inline BOOL CFileMapping::UnmapViewOfFile(LPCVOID lpBaseAddress)
{
	return ::UnmapViewOfFile(lpBaseAddress);
}