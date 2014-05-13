#include "Socket.h"

using namespace KS::Net;

/**********************************************************************/
/* PosixUDPSocket                                                    */
/**********************************************************************/
bool PosixUDPSocket::Bind(IPAddress &addr, int port, int range)
{
//    close();
    m_sock=::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(m_sock ==INVALID_SOCKET)
    {
        //log info

        return false;
    }

    int yes=1;
    if(setsockopt(m_sock,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1)
    {
       // error
    }

    m_address=addr;
    m_address.addr.sin_port=htons(port);
    m_port=port;

    if(::bind(m_sock,(struct sockaddr*)&m_address.addr,sizeof(m_address.addr)) !=0)
    {
       // error
        Close();
        return false;
    }
    setReady();

    return true;
}

void PosixUDPSocket::Close()
{
    if(m_sock)
    {
        ::close(m_sock);
        m_sock=INVALID_SOCKET;
    }
    m_ready=false;
}

int PosixUDPSocket::Read(IPAddress &addr, const int buffersize, void *buffer)
{
    return (int)recvfrom(m_sock, (char*)buffer, (size_t)buffersize, 0,
                         (struct sockaddr*)&addr.addr, &addr.size);
}

int PosixUDPSocket::SendTo(const IPAddress& addr, const int buffersize,
                          const void *buffer)
{
  return (int)sendto(m_sock, (const char *)buffer, (size_t)buffersize, 0,
                     (const struct sockaddr*)&addr.addr,
                     sizeof(addr.addr));
}
