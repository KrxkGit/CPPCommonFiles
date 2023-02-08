#include<CommCtrl.h>
#include"SkinPPWTL.h"
#pragma comment(lib,"SkinPPWTL.lib")

VOID FindLoadSkin()
{
	WIN32_FIND_DATAA fd;
	HANDLE hFind = FindFirstFileA("*.ssk", &fd);
	skinppLoadSkin(fd.cFileName);
	FindClose(hFind);
}