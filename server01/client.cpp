#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsData;
    if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0)
    {
        std::cerr << "Failed to initialize Winsock.\n";
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket.\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    inet_pton(AF_INET, "14.32.200.250", &(serverAddress.sin_addr)); // Replace with your server IP address

    if (connect(clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        std::cerr << "Failed to connect to the server.\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to the server. You can start sending messages.\n";

    char buffer[4096];
    std::string message;

    while (true)
    {
        // Send a message to the server
        std::cout << "Message: ";
        std::getline(std::cin, message);

        if (message == "quit")
        {
            break;
        }

        if (send(clientSocket, message.c_str(), message.size() + 1, 0) == SOCKET_ERROR)
        {
            std::cerr << "Failed to send message to the server.\n";
            break;
        }

        // Receive a message from the server
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0)
        {
            std::cerr << "Failed to receive message from the server.\n";
            break;
        }

        std::cout << "Server: " << buffer << "\n";
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
