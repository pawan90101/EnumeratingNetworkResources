#include "ServiceInstall.h"
#include "NetworkProcess.h"
#include <winsvc.h>
#include <Sddl.h>

BOOL CServiceInstall::InstallAndStartRemoteService(CString strRemoteMachineIP , CString SERVICENAME , CString SERVICE_EXE)
{
	// Open remote Service Manager
	SC_HANDLE hSCM = ::OpenSCManager(
		strRemoteMachineIP.GetBuffer(0),
		NULL,
		SC_MANAGER_ALL_ACCESS
	);

	if (hSCM == NULL)
	{
		return FALSE;
	}

	CString ExePath;
	CNetworkProcess cn;
	cn.GetWindowsDirectoryOfClient(strRemoteMachineIP, ExePath);
	ExePath += L"\\";
	ExePath += SERVICE_EXE;

	// Maybe it's already there and installed, let's try to run
	SC_HANDLE hService = ::OpenService(hSCM, SERVICENAME, SERVICE_ALL_ACCESS);


	// Creates service on remote machine, if it's not installed yet
	if (hService == NULL)
	{
		hService = ::CreateService(
			hSCM,
			SERVICENAME,
			SERVICENAME,
			SERVICE_ALL_ACCESS,
			SERVICE_WIN32_OWN_PROCESS,
			SERVICE_DEMAND_START,
			SERVICE_ERROR_NORMAL,
			ExePath,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL
		);
	}


	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		return FALSE;
	}
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	// Open remote Service Manager
	hSCM = ::OpenSCManager(
		strRemoteMachineIP.GetBuffer(0),
		NULL,
		SC_MANAGER_CONNECT
	);

	if (hSCM == NULL)
	{
		return FALSE;
	}

	// Start service
	if (!::StartService(hService, 0, NULL))
	{
		DWORD temp = GetLastError();
		return FALSE;
	}

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	return TRUE;
}
