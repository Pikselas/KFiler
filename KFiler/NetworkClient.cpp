#include "NetworkClient.h"
void NetworkClient::Connect(const std::string& Address, const std::string& Port)
{
    addrinfo hint = {0}, * res = nullptr;
    hint.ai_family = AF_UNSPEC;
    hint.ai_protocol = IPPROTO_TCP;
    hint.ai_socktype = SOCK_STREAM;
    auto ReriveStatus = getaddrinfo(Address.c_str(), Port.c_str(), &hint, &res);
    if (ReriveStatus != 0)
    {
        ThrowException(ReriveStatus);
    }
    else
    {
        for (auto AddrInfo = res; AddrInfo != nullptr; AddrInfo = AddrInfo->ai_next)
        {
            CONNECTION_SOCKET = socket(AddrInfo->ai_family, AddrInfo->ai_socktype, AddrInfo->ai_protocol);
            if (CONNECTION_SOCKET == INVALID_SOCKET)
            {
                freeaddrinfo(res);
                ThrowException(WSAGetLastError());
            }
            else if (connect(CONNECTION_SOCKET, AddrInfo->ai_addr,(int) AddrInfo->ai_addrlen) == SOCKET_ERROR)
            {
                closesocket(CONNECTION_SOCKET);
                CONNECTION_SOCKET = INVALID_SOCKET;
            }
            else
            {
                HasConnection = true;
                break;
            }
        }
        freeaddrinfo(res);
    }
}