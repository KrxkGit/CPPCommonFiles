#pragma once

BOOL SetPrivilege(
	HANDLE hToken, // access token handle
	LPCTSTR lpszPrivilege, // name of privilege to enable/disable
	BOOL bEnablePrivilege // to enable or disable privilege
	)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(
		NULL, // lookup privilege on local system
		lpszPrivilege, // privilege to lookup
		&luid)) // receives LUID of privilege
	{
		return FALSE;
	}
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges.

	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

	{
		return FALSE;
	}
	return TRUE;
}


LRESULT MySetSecurity(LPTSTR str_name, SE_OBJECT_TYPE e_type, LPTSTR geneic_str)
{
	long bRetval = -1;

	HANDLE hToken = NULL;
	PSID pSIDAdmin = NULL;
	PSID pSIDEveryone = NULL;
	PACL pNewDACL = NULL, pOldDACL = NULL;
	SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
	SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;
	const int NUM_ACES = 2;
	EXPLICIT_ACCESS ea[NUM_ACES];
	///获取操作权限
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);
	SetPrivilege(hToken, SE_TAKE_OWNERSHIP_NAME, TRUE);

	///获取所有权，再设置
	AllocateAndInitializeSid(&SIDAuthWorld, 1,
		SECURITY_WORLD_RID,
		0,
		0, 0, 0, 0, 0, 0,
		&pSIDEveryone);

	AllocateAndInitializeSid(&SIDAuthNT, 2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&pSIDAdmin);

	SetNamedSecurityInfo(
		str_name, // name of the object
		e_type, // type of object
		OWNER_SECURITY_INFORMATION, // change only the object's owner
		pSIDAdmin, // SID of Administrator group
		NULL,
		NULL,
		NULL);

	///////新权限设置/////////////////////////////

	ZeroMemory(&ea, NUM_ACES * sizeof(EXPLICIT_ACCESS));

	DWORD dwPermission = 0;
	ACCESS_MODE e_am = SET_ACCESS;

	if (0 == lstrcmpi(geneic_str, _T("ACCESS_READONLY")))
	{
		dwPermission = GENERIC_READ | GENERIC_EXECUTE;
		e_am = SET_ACCESS;
	}
	else
	if (0 == lstrcmpi(geneic_str, _T("ACCESS_DENYALL")))
	{
		dwPermission = GENERIC_ALL;
		e_am = DENY_ACCESS;
	}
	else
	if (0 == lstrcmpi(geneic_str, _T("ACCESS_GENERICALL")))
	{
		dwPermission = GENERIC_ALL;
		e_am = SET_ACCESS;
	}

	ea[0].grfAccessPermissions = dwPermission;
	ea[0].grfAccessMode = e_am;
	ea[0].grfInheritance = NO_INHERITANCE;
	ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
	ea[0].Trustee.ptstrName = (LPTSTR)pSIDEveryone;
	ea[1].grfAccessPermissions = dwPermission;
	ea[1].grfAccessMode = e_am;
	ea[1].grfInheritance = NO_INHERITANCE;
	ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ea[1].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
	ea[1].Trustee.ptstrName = (LPTSTR)pSIDAdmin;


	SetEntriesInAcl(NUM_ACES, ea, NULL, &pNewDACL);

	////////设置////////////////////////
	bRetval = SetNamedSecurityInfo(
		str_name, // name of the object
		e_type, // type of object
		DACL_SECURITY_INFORMATION | PROTECTED_DACL_SECURITY_INFORMATION, // change only the object's DACL
		NULL,
		NULL, // do not change owner or group
		pNewDACL, // DACL specified
		NULL); // do not change SACL

	if (hToken)
		CloseHandle(hToken);

	if (pSIDEveryone)
		FreeSid(pSIDEveryone);

	if (pSIDAdmin)
		FreeSid(pSIDAdmin);

	if (pNewDACL)
		LocalFree(pNewDACL);

	return bRetval;
}