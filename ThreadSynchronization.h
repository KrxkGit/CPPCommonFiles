/******************************************************************************
Module:  ThreadSynchronization.h
Notices: Copyright (C) 2016 
Author: Krxk
******************************************************************************/

#pragma once

//为了得到最佳性能，最好不要共享数据，然后依次使用volatile读取、volatile写入、Interlocked API、SRWLock以及关键段[用户模式同步]
//如以上方法均不适用，请使用内核模式下线程同步
//本文件仅为Win32编写，如果使用MFC，请使用MFC类库提供的对应类

//内核模式同步时，若需删除自动清理功能，请删除CKernelModeSynchronization类析构函数；否则，请注意CloseHanle调用次数[对象析构时会自动调用一次]

//用户模式线程同步
class CCriticalSection //关键段
{
private:
	CRITICAL_SECTION m_Cs;
public:
	CCriticalSection() {InitializeCriticalSection(&m_Cs);}
	CCriticalSection(DWORD dwSpinCount/*参考值:4000*/) {InitializeCriticalSectionAndSpinCount(&m_Cs,dwSpinCount);}//同时使用旋转锁
	~CCriticalSection() {DeleteCriticalSection(&m_Cs);}
	VOID Enter() {return EnterCriticalSection(&m_Cs);}
	VOID Leave() {return LeaveCriticalSection(&m_Cs);}
	BOOL TryEnter() {return TryEnterCriticalSection(&m_Cs);}//若返回值为TRUE，请记得调用Leave()函数
	DWORD SetCriticalSectionSpinCount(DWORD dwSpinCount) {
		return ::SetCriticalSectionSpinCount(&m_Cs,dwSpinCount);}//设置旋转锁旋转次数，如果主机只有一个处理器，本参数将被忽略
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

//内核模式线程同步
class CKernelModeSynchronization //内核模式线程同步通用数据，不应实例化该类对象
{
protected:
	HANDLE m_hObject;
public:
	DWORD WaitForSingleObject(DWORD dwMilliSeconds,BOOL bAclertable=FALSE) {
		return ::WaitForSingleObjectEx(m_hObject,dwMilliSeconds,bAclertable);}
	operator HANDLE() {return m_hObject;}
	operator CONST HANDLE() {return m_hObject;}
	BOOL CloseHandle() {return ::CloseHandle(m_hObject);}
	
	~CKernelModeSynchronization() {CloseHandle();}//自动清理子类对象，若不需要，请删除此行。
};

class CEvent:public CKernelModeSynchronization//事件
{
public:
	CEvent(LPSECURITY_ATTRIBUTES psa,BOOL bManualReset,BOOL bInitialState,PCTSTR lpName=NULL) {
		m_hObject = CreateEvent(psa,bManualReset,bInitialState,lpName);}

	BOOL SetEvent() {return ::SetEvent(m_hObject);}
	BOOL ReSetEvent() {return ::ResetEvent(m_hObject);}
	BOOL PulseEvent() {return ::PulseEvent(m_hObject);}
};

class CWaitableTimer:public CKernelModeSynchronization //可等待的计时器
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

class CSemaphore:public CKernelModeSynchronization //信号量
{
public:
	CSemaphore(PSECURITY_ATTRIBUTES psa,LONG lInitialCount,LONG lMaximumCount,PCTSTR pszName=NULL) {
		m_hObject=CreateSemaphore(psa,lInitialCount,lMaximumCount,pszName);
	}
	BOOL ReleaseSemaphore(LONG lRealeaseCount,PLONG plPreviousCount=NULL) {
		return ::ReleaseSemaphore(m_hObject,lRealeaseCount,plPreviousCount);
	}
};

class CMutex:public CKernelModeSynchronization //互斥量
{
public:
	CMutex(PSECURITY_ATTRIBUTES psa,BOOL bInitialOwner=FALSE/*初始状态为触发*/,PCTSTR pszName=NULL) {
		m_hObject=CreateMutex(psa,bInitialOwner,pszName);
	}
	BOOL ReleaseMutex() {return ::ReleaseMutex(m_hObject);}
};
