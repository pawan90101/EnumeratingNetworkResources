#include "stdafx.h"
#include "GetIpAddress.h"
#include <WinSock.h>
#include <string>
#pragma comment(lib,"ws2_32.lib")


void CGetIpAddress::GetIpAddress(string sHostName, string &sIp)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(1, 1), &wsaData);
	struct hostent *host; 
	struct in_addr *ptr; // To retrieve the IP Address 
	host = gethostbyname(sHostName.c_str());

	if (host != NULL)
	{
		ptr = (struct in_addr *)
			host->h_addr_list[0];

		// Eg. 211.40.35.76 split up like this.             
		int a = ptr->S_un.S_un_b.s_b1;  // 211           
		int b = ptr->S_un.S_un_b.s_b2;  // 40
		int c = ptr->S_un.S_un_b.s_b3;  // 35
		int d = ptr->S_un.S_un_b.s_b4;  // 76

		sIp = std::to_string(a);
		sIp += ".";
		sIp += std::to_string(b);
		sIp += ".";
		sIp += std::to_string(c);
		sIp += ".";
		sIp += std::to_string(d);
	}
	WSACleanup();
}