// 客户端代码
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    // 初始化Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize winsock" << std::endl;
        return 1;
    }

    // 创建Socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        WSACleanup();
        return 1;
    }

    // 设置服务器地址和端口
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345); // 服务器端口
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // 服务器IP地址

    // 连接到服务器
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to server" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // 发送和接收数据
    char buffer[1024];
    std::cout << "Enter a message to send to the server: ";
    std::cin.getline(buffer, sizeof(buffer));

    if (send(clientSocket, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send data to server" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    memset(buffer, 0, sizeof(buffer));
    if (recv(clientSocket, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to receive data from server" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Received from server: " << buffer << std::endl;

    // 关闭Socket和清理Winsock
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
// 服务端代码
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

void runServer() {
    // 初始化Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize winsock" << std::endl;
        return;
    }

    // 创建Socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        WSACleanup();
        return;
    }

    // 设置服务器地址和端口
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345); // 服务器端口
    serverAddress.sin_addr.s_addr = INADDR_ANY; // 任意IP地址

    // 绑定Socket到服务器地址和端口
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    // 监听连接请求
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Failed to listen for connections" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    std::cout << "Server is listening for connections..." << std::endl;

    // 接受连接请求
    sockaddr_in clientAddress;
    int clientAddressSize = sizeof(clientAddress);
    SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to accept connection" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    std::cout << "Client connected" << std::endl;

    // 接收和发送数据
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    if (recv(clientSocket, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to receive data from client" << std::endl;
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    std::cout << "Received from client: " << buffer << std::endl;

    std::cout << "Enter a message to send to the client: ";
    std::cin.getline(buffer, sizeof(buffer));

    if (send(clientSocket, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send data to client" << std::endl;
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    // 关闭Socket和清理Winsock
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}

// 客户端代码
void runClient() {
    // 初始化Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize winsock" << std::endl;
        return;
    }

    // 创建Socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        WSACleanup();
        return;
    }

    // 设置服务器地址和端口
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345); // 服务器端口
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // 服务器IP地址

    // 连接到服务器
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to server" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    // 发送和接收数据
    char buffer[1024];
    std::cout << "Enter a message to send to the server: ";
    std::cin.getline(buffer, sizeof(buffer));

    if (send(clientSocket, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send data to server" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    memset(buffer, 0, sizeof(buffer));
    if (recv(clientSocket, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to receive data from server" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    std::cout << "Received from server: " << buffer << std::endl;

    // 关闭Socket和清理Winsock
    closesocket(clientSocket);
    WSACleanup();
}

int main() {
    // 运行服务器
    runServer();

    // 运行客户端
    runClient();

    return 0;
}

#include <iostream>
#include <cstring>

int main() {
    int intValue = 42;
    double doubleValue = 3.14;
    long longValue = 123456789;

    unsigned char buffer[sizeof(int) + sizeof(double) + sizeof(long)];
    unsigned char* ptr = buffer;

    memcpy(ptr, reinterpret_cast<unsigned char*>(&intValue), sizeof(int));
    ptr += sizeof(int);

    memcpy(ptr, reinterpret_cast<unsigned char*>(&doubleValue), sizeof(double));
    ptr += sizeof(double);

    memcpy(ptr, reinterpret_cast<unsigned char*>(&longValue), sizeof(long));

    // Now the buffer contains the combined data

    // Print the buffer
    for (size_t i = 0; i < sizeof(buffer); i++) {
        std::cout << static_cast<int>(buffer[i]) << " ";
    }
    std::cout << std::endl;

    return 0;
}