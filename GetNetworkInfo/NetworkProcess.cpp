#include "stdafx.h"
#include "NetworkProcess.h"
#include <atlstr.h>


int CNetworkProcess::CreateFolderOnClient(CString FolderName, CString ClientName)
{
	if (ClientName.GetLength() == 0)
		return 117;
	if (FolderName.GetLength() == 0)
		return 118;
	if (FolderName.GetAt(0) != '\\')
		FolderName = _T("\\") + FolderName;

	ClientName.Format(_T("\\\\%s\\ADMIN$%s"), ClientName, FolderName);

	//ClientName=_T("\\\\")+ClientName+_T("\\ADMIN$")+FolderName;
	CreatePath(ClientName);

	if (_taccess(ClientName, 0) == -1)
	{
		return 117;
	}
	else
	{
		return 1;
	}

}

void CNetworkProcess::CreatePath(CString Path)
{

	CString str;


	int firstpos, nextpos, slen, curpos = 0;
	int dir_len, find_loc = 0;

	int drive_flag = 0, temp_flag = 0;
	CString dir_n, filename, temp_buff, source_p, t_dir;


	dir_n = Path;
	slen = dir_n.GetLength();
	curpos = 0;
	firstpos = str.Find('\\', 0);

	find_loc = dir_n.ReverseFind('\\');
	t_dir = str;
	//filename = dir_n.Right(dir_n.GetLength() - (find_loc + 1));
	//dir_n = dir_n.Left(dir_n.GetLength() - (filename.GetLength() + 1));

	while (curpos<slen)
	{
		nextpos = dir_n.Find('\\', firstpos + 1);

		if (nextpos < firstpos)
			nextpos = slen;

		dir_len = nextpos - firstpos;

		dir_len--;

		temp_buff = dir_n.Left(nextpos);

		find_loc = temp_buff.ReverseFind('\\');

		temp_buff = temp_buff.Right(temp_buff.GetLength() - (find_loc + 1));
		if (t_dir.Right(1) != '\\' && t_dir.GetLength() > 0)
			t_dir += '\\' + temp_buff;
		else
			t_dir += temp_buff;

		BOOL a = CreateDirectory(_T("\\\\") + t_dir, NULL);

		temp_buff.Empty();

		curpos = nextpos;

		firstpos = nextpos;
	}
}
int CNetworkProcess::GetWindowsDirectoryOfClient(CString ClientName, CString &WinDir)
{
	if (ClientName.GetLength() == 0)
		return 102;


	// Open remote Service Manager
	SC_HANDLE		scManager = NULL;
	SC_HANDLE		scReg = NULL;
	CString			strStatus;
	bool			fRetValue = false;
	DWORD			dwRetValue = 0;

	if ((scManager = OpenSCManager(ClientName, NULL, SC_MANAGER_ALL_ACCESS)) != NULL)
	{
		if ((scReg = OpenService(scManager, _T("RemoteRegistry"), SERVICE_START | SERVICE_QUERY_STATUS)) != NULL)
		{
			SERVICE_STATUS	ssStatus;

			if (StartService(scReg, 0, NULL) == 0
				&& GetLastError() != ERROR_SERVICE_ALREADY_RUNNING)
			{
				//strStatus.Format( "Failed to stop spooler (%d)\r\n", (dwRetValue = GetLastError()) );
			}
			else
			{
				strStatus = "Spooler stopping";
				while (QueryServiceStatus(scReg, &ssStatus)
					&& ssStatus.dwCurrentState != SERVICE_RUNNING)
				{
					//addStatusMessage( "." );
					Sleep(2000);
				}
				if (ssStatus.dwCurrentState == SERVICE_RUNNING)
				{
					// force window on top of defunct DOS windows
					strStatus = "\r\nSpooler stopped\r\n";
					//addStatusMessage( strStatus );
					fRetValue = true;
				}
			}
			CloseServiceHandle(scReg);

		}
		else
		{
			//strStatus.Format( "Failed to open Spooler service (%d)\r\n", (dwRetValue = GetLastError()) );
		}
		CloseServiceHandle(scManager);
	}
	else
	{
		//strStatus.Format( "Service Manager could not be opened (%d)\r\n", (dwRetValue = GetLastError()) );
		return 103;
	}


	HKEY result;
	if (RegConnectRegistry(_T("\\\\") + ClientName, HKEY_LOCAL_MACHINE, &result) == ERROR_SUCCESS)
	{
		HKEY childkey;
		if (RegOpenKeyEx(result, (LPCTSTR)_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), 0, KEY_QUERY_VALUE, &childkey) == ERROR_SUCCESS)
		{
			DWORD keyType;
			TCHAR dt[MAX_PATH];
			DWORD size = sizeof(dt);
			if (RegQueryValueEx(childkey, (LPCTSTR)_T("ActiveXCache"), 0, &keyType, (LPBYTE)dt, &size) == ERROR_SUCCESS)
			{
				///check for agent 
				DWORD type = REG_DWORD;
				TCHAR dt1[MAX_PATH];
				DWORD size = sizeof(dt1);
				if (RegQueryValueEx(childkey, (LPCTSTR)_T("ActiveXCache"), 0, &type, (LPBYTE)dt1, &size) == ERROR_SUCCESS)
				{
					TCHAR data[MAX_PATH];
					_tcscpy(data, dt1);

					/*CFile fp;
					fp.Open(_T("C:\\test.txt"), CFile::modeCreate | CFile::modeWrite);
					//char szRetCheck[MAX_PATH];
					//_itoa(retCheck, szRetCheck, 10);
					fp.Write(data, _tcslen(data));
					fp.Close();
					*/
					std::wfstream fs;
					fs.open("D:\\test.txt", std::fstream::in | std::fstream::out | std::fstream::app);

					fs << dt1;

					fs.close();


				}
				//////////////////////////////

				CString windowpath = dt;
				int location = windowpath.ReverseFind('\\');
				windowpath = windowpath.Left(location);
				WinDir = windowpath;
				return 1;
			}//end of regquary
			else
			{
				return 114;
			}
		}//end of regopen
		else
		{
			return 115;
		}
	}//end of regconnect
	else
	{
		return 116;
	}
}

int CNetworkProcess::DropFileOnClient(CString sIp, CString source, CString target)
{
	if ((sIp.GetLength() == 0) || (source.GetLength() == 0) || (target.GetLength() == 0))
	{
		return 113;
	}

	target.Format(_T("\\\\%s\\ADMIN$\\%s"), sIp, target);
	CopyFile(source, target, FALSE);
	if (_taccess(target, 0) == -1)
	{
		return 103;
	}
	return 1;

}
int CNetworkProcess::RunExeRemotly(CString sIp, CString source, CString target)
{
	if ((sIp.GetLength() == 0) || (source.GetLength() == 0) || (target.GetLength() == 0))
	{
		return 113;
	}

	

}


int CNetworkProcess::AddNetworkConnection(CString sIp, CString Username, CString Password)
{
	NETRESOURCE NetResource;
	NetResource.dwScope = RESOURCE_GLOBALNET;
	NetResource.dwType = RESOURCETYPE_ANY;
	NetResource.dwDisplayType = RESOURCEDISPLAYTYPE_SHARE;
	NetResource.dwUsage = RESOURCEUSAGE_CONNECTABLE;
	NetResource.lpLocalName = NULL;


	NetResource.lpRemoteName = sIp.GetBuffer(MAX_PATH);
	NetResource.lpComment = NULL;
	NetResource.lpProvider = NULL;

	DWORD dwResult= WNetCancelConnection2(sIp.GetBuffer(), CONNECT_UPDATE_PROFILE, FALSE);
	dwResult = WNetAddConnection2(&NetResource, Password, Username, CONNECT_UPDATE_PROFILE);
	if(dwResult == NO_ERROR)
	{
		CNetworkProcess cn;
		CString temp;
		CString val;
		//cn.AddNetworkConnection(L"192.168.10.28", L"domain1\\priyanka", L"Testing123");
		cn.CreateFolderOnClient(L"HelloPawan", L"192.168.10.28");
		cn.GetWindowsDirectoryOfClient(L"192.168.10.28", temp);
		val = L"HelloPawan\\test.exe";
		cn.DropFileOnClient(L"192.168.10.28",L"D:\\test.exe", val);

		return 0;
	}
	else
	{
		return 1;
	}

}