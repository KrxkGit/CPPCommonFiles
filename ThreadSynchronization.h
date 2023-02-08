/******************************************************************************
Module:  ThreadSynchronization.h
Notices: Copyright (C) 2016 
Author: Krxk
******************************************************************************/

#pragma once

//Ϊ�˵õ�������ܣ���ò�Ҫ�������ݣ�Ȼ������ʹ��volatile��ȡ��volatileд�롢Interlocked API��SRWLock�Լ��ؼ���[�û�ģʽͬ��]
//�����Ϸ����������ã���ʹ���ں�ģʽ���߳�ͬ��
//���ļ���ΪWin32��д�����ʹ��MFC����ʹ��MFC����ṩ�Ķ�Ӧ��

//�ں�ģʽͬ��ʱ������ɾ���Զ������ܣ���ɾ��CKernelModeSynchronization������������������ע��CloseHanle���ô���[��������ʱ���Զ�����һ��]

//�û�ģʽ�߳�ͬ��
class CCriticalSection //�ؼ���
{
private:
	CRITICAL_SECTION m_Cs;
public:
	CCriticalSection() {InitializeCriticalSection(&m_Cs);}
	CCriticalSection(DWORD dwSpinCount/*�ο�ֵ:4000*/) {InitializeCriticalSectionAndSpinCount(&m_Cs,dwSpinCount);}//ͬʱʹ����ת��
	~CCriticalSection() {DeleteCriticalSection(&m_Cs);}
	VOID Enter() {return EnterCriticalSection(&m_Cs);}
	VOID Leave() {return LeaveCriticalSection(&m_Cs);}
	BOOL TryEnter() {return TryEnterCriticalSection(&m_Cs);}//������ֵΪTRUE����ǵõ���Leave()����
	DWORD SetCriticalSectionSpinCount(DWORD dwSpinCount) {
		return ::SetCriticalSectionSpinCount(&m_Cs,dwSpinCount);}//������ת����ת�������������ֻ��һ������������������������
};

class CSRWLock //SRWLock
{
private:
	SRWLOCK m_SRWLock;
public:
	CSRWLock() {InitializeSRWLock(&m_SRWLock);}
	VOID AcquireSRWLockExclusive() {return ::AcquireSRWLockExclusive(&m_SRWLock);}
	VOID ReleaseSRWLockExclusive() {return ::ReleaseSRWLockExclusive(&m_SRWLock);}
	VOID AcquireSRWLockShared() {return ::AcquireSRWLockShared(&m_SRWLock);}
	VOID ReleaseSRWLockShared() {return ::ReleaseSRWLockShared(&m_SRWLock);}
};

//�ں�ģʽ�߳�ͬ��
class CKernelModeSynchronization //�ں�ģʽ�߳�ͬ��ͨ�����ݣ���Ӧʵ�����������
{
protected:
	HANDLE m_hObject;
public:
	DWORD WaitForSingleObject(DWORD dwMilliSeconds,BOOL bAclertable=FALSE) {
		return ::WaitForSingleObjectEx(m_hObject,dwMilliSeconds,bAclertable);}
	operator HANDLE() {return m_hObject;}
	operator CONST HANDLE() {return m_hObject;}
	BOOL CloseHandle() {return ::CloseHandle(m_hObject);}
	
	~CKernelModeSynchronization() {CloseHandle();}//�Զ������������������Ҫ����ɾ�����С�
};

class CEvent:public CKernelModeSynchronization//�¼�
{
public:
	CEvent(LPSECURITY_ATTRIBUTES psa,BOOL bManualReset,BOOL bInitialState,PCTSTR lpName=NULL) {
		m_hObject = CreateEvent(psa,bManualReset,bInitialState,lpName);}

	BOOL SetEvent() {return ::SetEvent(m_hObject);}
	BOOL ReSetEvent() {return ::ResetEvent(m_hObject);}
	BOOL PulseEvent() {return ::PulseEvent(m_hObject);}
};

class CWaitableTimer:public CKernelModeSynchronization //�ɵȴ��ļ�ʱ��
{
public:
	CWaitableTimer(PSECURITY_ATTRIBUTES psa,BOOL bManualReset,PCTSTR pszName=NULL) {
		m_hObject=CreateWaitableTimer(psa,bManualReset,pszName);
	}

	BOOL SetWaitableTimer(const LARGE_INTEGER* lpDueTime,LONG lPeriod,PTIMERAPCROUTINE pfnCompletionRoutine,
		PVOID pvArgToCompletionRoutine,BOOL bResume=FALSE) {
		return::SetWaitableTimer(m_hObject,lpDueTime,lPeriod,pfnCompletionRoutine,pvArgToCompletionRoutine,bResume);}
	BOOL CancelWaitableTimer() {return ::CancelWaitableTimer(m_hObject);}
};

class CSemaphore:public CKernelModeSynchronization //�ź���
{
public:
	CSemaphore(PSECURITY_ATTRIBUTES psa,LONG lInitialCount,LONG lMaximumCount,PCTSTR pszName=NULL) {
		m_hObject=CreateSemaphore(psa,lInitialCount,lMaximumCount,pszName);
	}
	BOOL ReleaseSemaphore(LONG lRealeaseCount,PLONG plPreviousCount=NULL) {
		return ::ReleaseSemaphore(m_hObject,lRealeaseCount,plPreviousCount);
	}
};

class CMutex:public CKernelModeSynchronization //������
{
public:
	CMutex(PSECURITY_ATTRIBUTES psa,BOOL bInitialOwner=FALSE/*��ʼ״̬Ϊ����*/,PCTSTR pszName=NULL) {
		m_hObject=CreateMutex(psa,bInitialOwner,pszName);
	}
	BOOL ReleaseMutex() {return ::ReleaseMutex(m_hObject);}
};
