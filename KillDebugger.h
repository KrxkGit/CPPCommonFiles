/*WM_KILLFOCUS使用*/

#pragma once
#define Be_DEBUGExitCode -1
#define MYAPPNAME "缴费系统"

VOID DoKillDebugger()
{
	MessageBox(NULL, _T("非法调试！\n程序即将退出"), _T("程序崩溃"), MB_OK);
	ExitProcess(Be_DEBUGExitCode);
}

BOOL __stdcall EnumDebuggerWnd(HWND hWnd, LPARAM)
{
	TCHAR sz[MAX_PATH];
	GetWindowText(hWnd, sz, MAX_PATH);

	if (_tcsstr(sz, _T("破解")) || _tcsstr(sz, _T("pojie"))) {
		DoKillDebugger();
		return FALSE;
	}
	return TRUE;
}

VOID KillDebugger()//防止调试
{
	if (IsDebuggerPresent()) {
		goto end;
	}
	//找窗口
	if (FindWindow(_T("Jiack  "), NULL) || FindWindow(NULL, _T("ollydbg"))) {
		goto end;
	}
	/*//顶层窗口
	TCHAR sz[MAX_PATH];
	HWND hWnd = GetForegroundWindow();
	GetWindowText(hWnd, sz, _countof(sz));
	if (lstrcmp(sz, _T(MYAPPNAME)) != 0) {
		SendMessage(hWnd, WM_CLOSE, 0, 0);
		goto end;
	}*/
	//枚举法
	EnumWindows(EnumDebuggerWnd, NULL);
	/*#ifdef _WIN32
	/*WIN32程序
	HWND hDesktop = GetFirstChild(GetFirstChild(FindWindow(_T("Program"), _T("Program Manager"))));
	DWORD ProcessId = 0;
	GetWindowThreadProcessId(hDesktop, &ProcessId);
	if (GetParent)
	#endif*/
	return;

end: DoKillDebugger();
}