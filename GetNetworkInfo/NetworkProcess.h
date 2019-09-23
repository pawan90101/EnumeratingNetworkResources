#pragma once
#include "stdafx.h"
#include <atlstr.h>
class CNetworkProcess
{
public:
	int CreateFolderOnClient(CString FolderName, CString ClientName);
	void CreatePath(CString Path);
	int GetWindowsDirectoryOfClient(CString ClientName, CString &WinDir);
	int DropFileOnClient(CString sIp, CString source, CString target);
	int RunExeRemotly(CString sIp, CString source, CString target);
	int AddNetworkConnection(CString sIp,CString Username,CString Password);
};