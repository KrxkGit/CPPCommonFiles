#pragma once

//开机启动

#define VALUENAME "AppName"//程序名


BOOL RunByWindow(BOOL bRun)
{
	TCHAR szKey[] = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	HKEY hKey;
	DWORD dwType;

	if (bRun) {
		BOOL bSuccess = FALSE;
		RegOpenKeyEx(HKEY_CURRENT_USER, szKey, 0, KEY_READ, &hKey);
		bSuccess = RegQueryValueEx(hKey, _T(VALUENAME), 0, &dwType, NULL, NULL);
		RegCloseKey(hKey);
		if (bSuccess == ERROR_SUCCESS || bSuccess == ERROR_MORE_DATA) { //键值已存在
			return TRUE;
		}
		else {
			RegOpenKeyEx(HKEY_CURRENT_USER, szKey, 0, KEY_SET_VALUE, &hKey);
			TCHAR szPath[MAX_PATH];
			GetModuleFileName(NULL, szPath, _countof(szPath));
			RegSetValueEx(hKey, _T(VALUENAME), 0, REG_SZ, (BYTE*)szPath, (DWORD)(_tcslen(szPath) + 1)*sizeof(TCHAR));
			RegCloseKey(hKey);
		}
	}
	else {
		RegOpenKeyEx(HKEY_CURRENT_USER, szKey, 0, KEY_SET_VALUE, &hKey);
		RegDeleteValue(hKey, _T(VALUENAME));
		RegCloseKey(hKey);
	}
	return TRUE;
}

#undef VALUENAME