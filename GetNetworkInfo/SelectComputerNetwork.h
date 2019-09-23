// SelectComputerNetwork.h: interface for the CSelectComputerNetwork class.
//
//////////////////////////////////////////////////////////////////////
#include <winnetwk.h>
#include <vector>
#include <string>

using namespace std;

class CSelectComputerNetwork  
{
public:
    CSelectComputerNetwork();
    virtual ~CSelectComputerNetwork();

public: 
    static bool GetDomains(vector<basic_string<TCHAR> >& vecstrDomains);
    static bool GetServers(const basic_string<TCHAR>& strDomain, vector<basic_string<TCHAR> >& vecstrServers);

private:
//NETRESOURCE structure wrapper used to save text data that freed by WNetEnumClose(_);
    class CNETRESOURCE 
    {
    public:
        CNETRESOURCE(const NETRESOURCE& nr):
        m_nr(nr)
        {
			m_strLocalName = nr.lpLocalName ? nr.lpLocalName : _T("");
			m_strRemoteName = nr.lpRemoteName ? nr.lpRemoteName : _T("");
			m_strComment = nr.lpComment ? nr.lpComment : _T("");
			m_strProvider = nr.lpProvider ? nr.lpProvider : _T("");

            m_nr.lpLocalName = new TCHAR [m_strLocalName.size() + 1];
            _tcscpy(m_nr.lpLocalName, m_strLocalName.c_str());

            m_nr.lpRemoteName = new TCHAR [m_strRemoteName.size() + 1];
            _tcscpy(m_nr.lpRemoteName, m_strRemoteName.c_str());

            m_nr.lpComment = new TCHAR [m_strComment.size() + 1];
            _tcscpy(m_nr.lpComment, m_strComment.c_str());

            m_nr.lpProvider = new TCHAR [m_strProvider.size() + 1];
            _tcscpy(m_nr.lpProvider, m_strProvider.c_str());

        }

        CNETRESOURCE(const CNETRESOURCE& rhs):
        m_nr(rhs.m_nr),
        m_strLocalName(rhs.m_strLocalName), 
        m_strRemoteName(rhs.m_strRemoteName), 
        m_strComment(rhs.m_strComment), 
        m_strProvider(rhs.m_strProvider) 
        {
            m_nr.lpLocalName = new TCHAR [m_strLocalName.size() + 1];
            _tcscpy(m_nr.lpLocalName, m_strLocalName.c_str());

            m_nr.lpRemoteName = new TCHAR [m_strRemoteName.size() + 1];
            _tcscpy(m_nr.lpRemoteName, m_strRemoteName.c_str());

            m_nr.lpComment = new TCHAR [m_strComment.size() + 1];
            _tcscpy(m_nr.lpComment, m_strComment.c_str());

            m_nr.lpProvider = new TCHAR [m_strProvider.size() + 1];
            _tcscpy(m_nr.lpProvider, m_strProvider.c_str());
        }
        
        ~CNETRESOURCE()
        {
            delete [] m_nr.lpLocalName;
            delete [] m_nr.lpRemoteName;
            delete [] m_nr.lpComment;
            delete [] m_nr.lpProvider;
        }

    public:
        NETRESOURCE* operator &() 
        {
            return &m_nr;
        }

        operator NETRESOURCE() const
        {
            return m_nr;
        }
    
        void CorrectRemoteName()
        {
            m_strRemoteName = m_strRemoteName.substr(2);//skip 2 leading slashes

            delete [] m_nr.lpRemoteName;
            
            m_nr.lpRemoteName = new TCHAR [m_strRemoteName.size() + 1];
            _tcscpy(m_nr.lpRemoteName, m_strRemoteName.c_str());
        }

        basic_string<TCHAR> GetRemoteName() const
        {
            return m_strRemoteName;
        }
            
        
    
    private:
        NETRESOURCE m_nr;

    private:
        basic_string<TCHAR>     m_strLocalName; 
        basic_string<TCHAR>     m_strRemoteName; 
        basic_string<TCHAR>     m_strComment; 
        basic_string<TCHAR>     m_strProvider; 

    };
//NETRESOURCE structure wrapper

private:
    static bool EnumDomains(NETRESOURCE* pnr, vector<CNETRESOURCE>& vecnrDomains);
    static bool EnumServers(NETRESOURCE* pnr, vector<CNETRESOURCE>& vecnrServers);
};

