/*WM_KILLFOCUSʹ��*/

#pragma once
#define Be_DEBUGExitCode -1
#define MYAPPNAME "�ɷ�ϵͳ"

VOID DoKillDebugger()
{
	MessageBox(NULL, _T("�Ƿ����ԣ�\n���򼴽��˳�"), _T("�������"), MB_OK);
	ExitProcess(Be_DEBUGExitCode);
}

BOOL __stdcall EnumDebuggerWnd(HWND hWnd, LPARAM)
{
	TCHAR sz[MAX_PATH];
	GetWindowText(hWnd, sz, MAX_PATH);

	if (_tcsstr(sz, _T("�ƽ�")) || _tcsstr(sz, _T("pojie"))) {
		DoKillDebugger();
		return FALSE;
	}
	return TRUE;
}

VOID KillDebugger()//��ֹ����
{
	if (IsDebuggerPresent()) {
		goto end;
	}
	//�Ҵ���
	if (FindWindow(_T("Jiack  "), NULL) || FindWindow(NULL, _T("ollydbg"))) {
		goto end;
	}
	/*//���㴰��
	TCHAR sz[MAX_PATH];
	HWND hWnd = GetForegroundWindow();
	GetWindowText(hWnd, sz, _countof(sz));
	if (lstrcmp(sz, _T(MYAPPNAME)) != 0) {
		SendMessage(hWnd, WM_CLOSE, 0, 0);
		goto end;
	}*/
	//ö�ٷ�
	EnumWindows(EnumDebuggerWnd, NULL);
	/*#ifdef _WIN32
	/*WIN32����
	HWND hDesktop = GetFirstChild(GetFirstChild(FindWindow(_T("Program"), _T("Program Manager"))));
	DWORD ProcessId = 0;
	GetWindowThreadProcessId(hDesktop, &ProcessId);
	if (GetParent)
	#endif*/
	return;

end: DoKillDebugger();
}