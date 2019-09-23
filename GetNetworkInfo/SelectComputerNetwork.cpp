// SelectComputerNetwork.cpp: implementation of the CAMSSAdministratorNetwork class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "SelectComputerNetwork.h"

//we need this library to use WNetXXX functions
#pragma comment(lib,"mpr.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static const int BUFSIZE = 10000;

CSelectComputerNetwork::CSelectComputerNetwork()
{
}

CSelectComputerNetwork::~CSelectComputerNetwork()
{
}

bool CSelectComputerNetwork::GetDomains(vector<basic_string<TCHAR> >& vecstrDomains)
{
    vector<CNETRESOURCE> vecnrDomains;
    if (!EnumDomains(NULL, vecnrDomains))
        return false;
	
	for (vector<CNETRESOURCE>::const_iterator ci = vecnrDomains.begin();
			ci != vecnrDomains.end(); ++ci)
		vecstrDomains.push_back(ci->GetRemoteName());

	return true;
}

bool CSelectComputerNetwork::GetServers(const basic_string<TCHAR>& strDomain, vector<basic_string<TCHAR> >& vecstrServers)
{
    vector<basic_string<TCHAR> > vecstrDomains;

	if (!GetDomains(vecstrDomains))	
		return false;
 
    vector<CNETRESOURCE> vecnrDomains;

    if (!EnumDomains(NULL, vecnrDomains))
        return false;

    for (vector<CNETRESOURCE>::iterator i = vecnrDomains.begin();
                i != vecnrDomains.end(); ++i)
	if ((*i).GetRemoteName() == strDomain)//equal strings
	{
		vector<CNETRESOURCE> vecnrServers;
		if (!EnumServers(&(*i), vecnrServers))
			return false;

		for(vector<CNETRESOURCE>::const_iterator ci = vecnrServers.begin();
				ci != vecnrServers.end(); ++ ci)
			vecstrServers.push_back(ci->GetRemoteName());

		return true;
	}
	
	return true;
}


bool CSelectComputerNetwork::EnumDomains(NETRESOURCE* pnr, vector<CNETRESOURCE>& vecnrDomains)
{
    HANDLE handle = NULL;   
    DWORD dwResult = ::WNetOpenEnum(RESOURCE_GLOBALNET, 
                                    RESOURCETYPE_ANY, 
                                    0, 
                                    pnr,
                                    &handle);

    if (dwResult == NO_ERROR)
    {
        NETRESOURCE nr, *pnrChildrenResource = new NETRESOURCE[BUFSIZE];
        
        DWORD dwRequested = -1;
        DWORD& dwGotten = dwRequested;

        DWORD dwBufSize = sizeof(NETRESOURCE) * BUFSIZE;

        dwResult = ::WNetEnumResource(handle, &dwRequested, pnrChildrenResource, &dwBufSize);
        
        if (dwResult == NO_ERROR)
        {
            for (int i = 0; i < dwGotten; ++i)
            {
                if (pnrChildrenResource[i].dwDisplayType == RESOURCEDISPLAYTYPE_DOMAIN)
                {
                    vecnrDomains.push_back(pnrChildrenResource[i]);
                }
                else
                {
                    memcpy(&nr, &pnrChildrenResource[i], sizeof(NETRESOURCE));
					if(_tcscmp(nr.lpRemoteName, _T("Microsoft Terminal Services")) && _tcscmp(nr.lpRemoteName, _T("Web Client Network")))
					{
						if (!EnumDomains(&nr, vecnrDomains))
						{
							delete[] pnrChildrenResource;
							::WNetCloseEnum(handle);
							return false;
						}
					}
                }
            }
        
            delete[] pnrChildrenResource;
            return (::WNetCloseEnum(handle) == NO_ERROR) ? true : false;
        }
		else if (dwResult == ERROR_NO_MORE_ITEMS)
		{
            delete[] pnrChildrenResource;
            return (::WNetCloseEnum(handle) == NO_ERROR) ? true : false;
		}
        else//WNetEnumResource(_) failed
        {
            _ASSERTE(FALSE);
            delete[] pnrChildrenResource;
            ::WNetCloseEnum(handle);
            return false;
        }

    }
    else//WNetOpenEnum(_) failed
    {
        _ASSERTE(FALSE);
		if(vecnrDomains.size() == 0)
			return false;
		else
			return true;
    }
}

bool CSelectComputerNetwork::EnumServers(NETRESOURCE* pnr, vector<CNETRESOURCE>& vecnrServers)
{
    HANDLE handle = NULL;   
    DWORD dwResult = ::WNetOpenEnum(RESOURCE_GLOBALNET, 
                                    RESOURCETYPE_ANY, 
                                    0, 
                                    pnr,
                                    &handle);

    if (dwResult == NO_ERROR)
    {
        NETRESOURCE *pnrChildrenResource = new NETRESOURCE[BUFSIZE];
        
        DWORD dwRequested = -1;
        DWORD& dwGotten = dwRequested;

        DWORD dwBufSize = sizeof(NETRESOURCE) * BUFSIZE;

        dwResult = ::WNetEnumResource(handle, &dwRequested, pnrChildrenResource, &dwBufSize);
        
        if (dwResult == NO_ERROR)
        {
            for (int i = 0; i < dwGotten; ++i)
            {
				if (pnrChildrenResource[i].dwDisplayType == RESOURCEDISPLAYTYPE_SERVER)
                {   
                    vecnrServers.push_back(pnrChildrenResource[i]);
                    //kill 2 leading "\\" tokens from lpRemoteName member 
                    vecnrServers.back().CorrectRemoteName();
                }
            }
        
            delete[] pnrChildrenResource;
            return (::WNetCloseEnum(handle) == NO_ERROR) ? true : false;
        }
		else if (dwResult == ERROR_NO_MORE_ITEMS)
		{
            delete[] pnrChildrenResource;
            return (::WNetCloseEnum(handle) == NO_ERROR) ? true : false;
		}
		else//WNetEnumResource(_) failed
        {
			_ASSERTE(FALSE);
            delete[] pnrChildrenResource;
            ::WNetCloseEnum(handle);
            return false;
        }

    }
    else//WNetOpenEnum(_) failed
    {
        _ASSERTE(FALSE);
        return false;
    }
}

