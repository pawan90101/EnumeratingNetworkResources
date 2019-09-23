// GetNetworkInfo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "SelectComputerNetwork.h"
#include "GetIpAddress.h"
#include "GetMacAddress.h"
#include "NetworkProcess.h"
#include <atlstr.h>

int main()
{
	

	vector<basic_string<TCHAR> >vecstrDomains;
	CSelectComputerNetwork::GetDomains(vecstrDomains);
	for (vector<basic_string<TCHAR> >::iterator iot = vecstrDomains.begin(); iot != vecstrDomains.end(); ++iot)
	{
		vector<basic_string<TCHAR> >vecstrServers;
		std::wcout << *iot <<"\n";
		CSelectComputerNetwork::GetServers(*iot, vecstrServers);
		for (vector<basic_string<TCHAR> >::iterator iot1 = vecstrServers.begin(); iot1 != vecstrServers.end(); ++iot1)
		{
			wstring temp = *iot1;
			const std::string sHost(temp.begin(), temp.end());
			string sIp;
			string sMac;
			CGetIpAddress::GetIpAddress(sHost, sIp);
			CGetMacAddress::GetMacAddress(sIp, sMac);
			std::wcout << *iot1 << "  Ip Address: ";
			std::cout << sIp <<"  MACAddress: "<< sMac <<"\n";
		}

		std::wcout << "........................End of domain..............." << "\n";

	}
	CNetworkProcess cn;
	CString val;
	cn.AddNetworkConnection(L"\\\\192.168.10.28", L"domain1\\priyanka", L"Testing123");
	//cn.CreateFolderOnClient(L"HelloPawan",L"192.168.10.28");
	//cn.GetWindowsDirectoryOfClient(L"192.168.10.28", val);

	return 0;
}

