#include "NetworkServer.h"

NetworkServer::NetworkServer(const std::string& Port)
{
    addrinfo hint, * res;
    hint = { 0 };
    hint.ai_family = AF_INET;
    hint.ai_protocol = IPPROTO_TCP;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags = AI_PASSIVE;
    auto RetriveStatus = getaddrinfo(nullptr, Port.c_str(), &hint, &res);
    if (RetriveStatus != 0)
    {
        ThrowException(RetriveStatus);
    }
    LISTEN_SOCKET = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (LISTEN_SOCKET != INVALID_SOCKET)
    {
        auto BindResult = bind(LISTEN_SOCKET, res->ai_addr,(int) res->ai_addrlen);
        freeaddrinfo(res);
        if (BindResult == SOCKET_ERROR)
        {
            ThrowException(WSAGetLastError());
        }
    }
    else
    {
        freeaddrinfo(res);
        ThrowException(WSAGetLastError());
    }
}

NetworkServer::~NetworkServer()
{
    DisConnect();
    DestroyServer();
}

void NetworkServer::Listen() const
{
    if(listen(LISTEN_SOCKET, 1) == SOCKET_ERROR)
    {
        ThrowException(WSAGetLastError());
    }
}

void NetworkServer::AcceptConnection()
{
    if (CONNECTION_SOCKET == INVALID_SOCKET)
    {
        CONNECTION_SOCKET = accept(LISTEN_SOCKET, nullptr, nullptr);
        if (CONNECTION_SOCKET == INVALID_SOCKET)
        {
            ThrowException(WSAGetLastError());
        }
        else
        {
            HasConnection = true;
        }
    }
}
// it doesn't destroy existing connection 
// it stops listening
void NetworkServer::DestroyServer()
{
    if (LISTEN_SOCKET != INVALID_SOCKET)
    {
        closesocket(LISTEN_SOCKET);
        LISTEN_SOCKET = INVALID_SOCKET;
    }
}
