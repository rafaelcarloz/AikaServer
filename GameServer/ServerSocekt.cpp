#include "ServerSocket.h"
#include <winsock2.h>
#include <WS2tcpip.h>

using namespace std;

ServerSocket::ServerSocket(string Address, WORD port) {
    this->_address = Address;
    this->_port = port;
}

bool ServerSocket::StartSocket() {
    int iResult;
    char messageStr[2048] = "\n";

    iResult = WSAStartup(MAKEWORD(2, 2), &_wsaData);
    if (iResult != 0) {
        Logger::Write("[ServerSocket::StartSocket] WSAStartup error", LOG_TYPE::Error);
        return false;
    }

    struct addrinfo* result = NULL, * ptr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(this->_address.c_str(), std::to_string(this->_port).c_str(), &hints, &result);
    if (iResult != 0) {
        Logger::Write("[ServerSocket::StartSocket] getaddrinfo error", LOG_TYPE::Error);
        WSACleanup();
        return false;
    }

    this->_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (bind(this->_socket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR) {
        int errorCode = WSAGetLastError();

        sprintf_s(messageStr, "[ServerSocket::StartSocket] socket bind error on [%s:%d] #%d", this->_address.c_str(), this->_port, errorCode);

        Logger::Write(messageStr, LOG_TYPE::Error);
        freeaddrinfo(result);
        closesocket(this->_socket);
        WSACleanup();
        return false;
    }

    if (listen(this->_socket, SOMAXCONN) == SOCKET_ERROR) {
        int errorCode = WSAGetLastError();

        Logger::Write("[ServerSocket::StartSocket] socket listen error", LOG_TYPE::Error);
        closesocket(this->_socket);
        WSACleanup();
        return false;
    }

    sprintf_s(messageStr, "[ServerSocket::StartSocket] socket succesfully started on [%s:%d]", this->_address.c_str(), this->_port);

    Logger::Write(messageStr, LOG_TYPE::Status);

    return true;
}

void ServerSocket::CloseSocket() {
    if (this->_socket == INVALID_SOCKET) {
        return;
    }

    closesocket(this->_socket);
}

bool ServerSocket::AcceptConnection(SOCKET *clientSocket, sockaddr *clientAddr) {
    *clientSocket = accept(this->_socket, clientAddr, NULL);

    if (*clientSocket == INVALID_SOCKET) {
        return false;
    }

    return true;
}