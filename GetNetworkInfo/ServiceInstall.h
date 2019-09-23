#pragma once
#include "stdafx.h"
#include <atlstr.h>
class CServiceInstall
{
public:
	BOOL InstallAndStartRemoteService(CString strRemoteMachineIP, CString SERVICENAME, CString REMOTE_ADMIN_SERVICE_EXE);
};
