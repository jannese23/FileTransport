#include <cstring>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

/*
    Correct order:
    1. initialize Winsock
    2. create socket
    3. configure address
    4. bind socket
    5. listen on socket
    6. accept connections
    7. send/receive data
    8. close socket
    9. cleanup Winsock
*/

int main() {
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != 0) {
        cout << "WSAStartup failed: " << iResult << endl;
        return 1;
    }

    cout << "Winsock initialized successfully." << endl;
    // Initialize Winsock end

    // Create socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cout << "Socket creation failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }
    // Create socket end

    // Configure address
    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    // Configure address end

    // Bind socket
    int binding = bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
    if (binding == SOCKET_ERROR) {
        cout << "Bind failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    // Bind socket end

    // Listen on socket
    int listening = listen(serverSocket, SOMAXCONN);
    if (listening == SOCKET_ERROR) {
        cout << "Listen failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Accept connections
    SOCKADDR_IN clientAddress;
    int size_clientAddress = sizeof(clientAddress);
    SOCKET clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &size_clientAddress);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Accept failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    cout << "Client connected." << endl;

    // Send/receive data
    char buff[] = "hello world";
    int sentbytes = send(clientSocket, buff, sizeof(buff), 0);
    cout << "Sent bytes: " << sentbytes << endl;

    // Cleanup
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}