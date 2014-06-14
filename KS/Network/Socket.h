#ifndef SOCKET_H
#define SOCKET_H

#include <map>
#include <vector>
#ifdef _UNIX
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


//#ifdef TARGET_POSIX
typedef int SOCKET;
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR (-1)
//#endif


namespace KS {namespace Net{

enum SocketType
{
    ST_TCP,
    ST_UDP,
    ST_UNIX
};

/**********************************************************************/
/* IP address abstraction class                                       */
/**********************************************************************/
class IPAddress
{
public:
    sockaddr_in addr;
    socklen_t size;

    IPAddress()
    {
        memset(&addr,0,sizeof(addr));
        addr.sin_family=AF_INET;
        addr.sin_addr.s_addr=htonl(INADDR_ANY);
        size=sizeof(addr);
    }
    void setAddress(const char* address)
    {
        memset(&addr,0,sizeof(addr));
        addr.sin_family=AF_INET;
        addr.sin_addr.s_addr=inet_addr(address);
        size=sizeof(addr);
    }

    char *toString()
    {
        return inet_ntoa(addr.sin_addr);
    }

    unsigned long ULong()
    {
        return (unsigned long)addr.sin_addr.s_addr;
    }
};


/**
 * Base Class for all sockets
 *
 */
class BaseSocket
{
public:
    BaseSocket()
    {
        m_type=ST_TCP;
        m_ready=false;
        m_port=0;
    }
    virtual ~BaseSocket(){Close();}

    //socket functions
    virtual bool Bind(IPAddress &addr,int port, int range=0)=0;
    virtual bool Connect()=0;
    virtual void Close(){};

    //state functions
    bool ready(){return m_ready;}
    SocketType type(){return m_type;}
    int port(){return m_port;}
    virtual SOCKET Socket()=0;

protected:
    virtual void setReady(bool set=true){m_ready=set;}

protected:
    SocketType m_type;
    bool m_ready;
    //    bool bound;
    int m_port;
};

/**********************************************************************/
/* Base class for UDP socket implementations                          */
/**********************************************************************/
class UDPSocket : public BaseSocket
{
public:
    UDPSocket()
    {
        m_type=ST_UDP;
    }

    //I/O functions
    virtual int Sendto(const IPAddress &addr,const int bufferLen,const void * buffer)=0;
    // read datagrams, return no. of bytes read or -1 or error
    virtual int Read(IPAddress &addr,const int bufferSize,void* buffer)=0;
    virtual bool Broadcast(IPAddress& addr,const int dataSize,const void* data)=0;
};

/**********************************************************************/
/* POSIX based UDP socket implementation                              */
/**********************************************************************/
class PosixUDPSocket: public UDPSocket
{
public:
    PosixUDPSocket()
    {
        m_sock=INVALID_SOCKET;
    }

    bool Bind(IPAddress& addr, int port, int range=0);
    bool Connect() { return false; }
    bool Listen(int timeout);
    int  SendTo(const IPAddress& addr, const int datasize, const void* data);
    int  Read(IPAddress& addr, const int buffersize, void *buffer);
    bool Broadcast(const IPAddress& addr, const int datasize, const void* data)
    {
        // TODO
        return false;
    }
    SOCKET Socket() { return m_sock; }
    void Close();

protected:
    SOCKET m_sock;
    IPAddress m_address;
};




}} // namespace KS::Net
#else  //window



#endif



#endif // SOCKET_H
