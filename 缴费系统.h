//#include"GetInternetTime.h"

TCHAR RegPath[] = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Money_Pay");

BOOL IsHavePayMoney()
{
	SYSTEMTIME st;
	ZeroMemory(&st, sizeof(st));
	//if (!GetInternetTime(&st)) {
		GetLocalTime(&st);
	//}
	HKEY hKey;
	RegOpenKeyEx(HKEY_CURRENT_USER, RegPath, 0, KEY_QUERY_VALUE, &hKey);
	FILETIME ft,temp;
	SystemTimeToFileTime(&st, &temp);
	DWORD dwType = REG_BINARY, dwSize = sizeof(ft);
	BOOL bQuerySuccess = RegQueryValueEx(hKey, _T("Money_Pay"), 0, &dwType, (LPBYTE)&ft, &dwSize);
	if (CompareFileTime(&ft,&temp)>=0 && bQuerySuccess==0) {//未过期
		return TRUE;
	}
	RegCloseKey(hKey);

	return FALSE;
}

#define ExitIfHaveNotPayMoney() \
	if (!IsHavePayMoney()) { \
		MessageBox(NULL, _T("软件尚未激活！"), NULL, MB_ICONERROR); \
		return GetLastError(); \
	}
