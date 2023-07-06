#include "sync.hpp"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

SyncHandler::SyncHandler()
{
    senddata();
}

int SyncHandler::senddata()
{
    WSADATA wsaData;
    int iResult;
    char server_ip_addr[14];
    int port_number;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    // SOCKET ConnectSocket = INVALID_SOCKET;
    // ConnectSocket = socket(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP);

    // if (ConnectSocket == INVALID_SOCKET)
    // {
    //     std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
    //     // freeaddrinfo(result);
    //     WSACleanup();
    //     return;
    // }

    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

#define DEFAULT_PORT "1337"

    // Resolve the server address and port
    iResult = getaddrinfo("172.27.203.224", DEFAULT_PORT, &hints, &result);
    if (iResult != 0)
    {
        std::cout << "getaddrinfo failed: " << iResult << std::endl;
        WSACleanup();
        return 1;
    }

    SOCKET ConnectSocket = INVALID_SOCKET;

    // Attempt to connect to the first address returned by
    // the call to getaddrinfo
    ptr = result;

    // Create a SOCKET for connecting to server
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
                           ptr->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET)
    {
        std::cout << "Error at socket(): " << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Connect to server.
    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    // Should really try the next address returned by getaddrinfo
    // if the connect call failed
    // But for this simple example we just free the resources
    // returned by getaddrinfo and print an error message

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

#define DEFAULT_BUFLEN 512

    int recvbuflen = DEFAULT_BUFLEN;

    const char *sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    
    // Send an initial buffer
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR)
    {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);

    // shutdown the connection for sending since no more data will be sent
    // the client can still use the ConnectSocket for receiving data
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Receive data until the server closes the connection
    do
    {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
            printf("Bytes received: %d\n", iResult);
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());
    } while (iResult > 0);

    // std::cout << "接続先IPアドレスを入力してください(xxx.xxx.xxx.xxx)" << std::endl;
    // std::cin >> server_ip_addr;
    // std::cout << "ポート番号を入力してください" << std::endl;
    // std::cin >> port_number;

    // // sockaddr_in構造体の作成とポート番号、IPタイプの入力
    // struct sockaddr dst_addr;
    // memset(&dst_addr, 0, sizeof(dst_addr));
    // // dst_addr.sin_port = htons(port_number); // ポート番号
    // // dst_addr.sa_data = server_ip_addr;
    // strcpy(dst_addr.sa_data, server_ip_addr);
    // dst_addr.sa_family = AF_UNSPEC;          // AF_INETはipv4を示す

    // iResult = connect(ConnectSocket, (struct sockaddr *)&dst_addr, sizeof(dst_addr));
    // if (iResult == SOCKET_ERROR)
    // {
    //     closesocket(ConnectSocket);
    //     ConnectSocket = INVALID_SOCKET;
    //     std::cout << "Unable to connect to server" << std::endl;
    // }



    // // 引数は (1) Type(ipv4 or v6) (2) IPアドレスのテキスト形式 (3) IPアドレスのバイナリ形式【(2)→(3)に変換】
    // inet_pton(dst_addr.sin_family, server_ip_addr, &dst_addr.sin_addr.s_addr);

    // // AF_INETはipv4のIPプロトコル & SOCK_STREAMはTCPプロトコル
    // int dst_socket = socket(AF_INET, SOCK_STREAM, 0);

    // // 接続処理
    // if (connect(dst_socket, (struct sockaddr *)&dst_addr, sizeof(dst_addr)))
    // {
    //     std::cerr << "接続失敗(サーバIPアドレス" << server_ip_addr << "/接続先ポート番号" << port_number << std::endl;
    //     exit(0);
    // }

    // std::cout << "接続完了(サーバIPアドレス" << server_ip_addr << "/接続先ポート番号" << port_number << std::endl
    //           << std::endl;
    // ;

    // char send_buf1[256], send_buf2[256];
    // char recv_buf[256];

    // while (1)
    // {

    //     std::cout << "数字を2個入力してください" << std::endl;
    //     std::cin >> send_buf1 >> send_buf2;

    //     // Packetの送信(SOCKET, Buffer, Datasize, 送信方法)
    //     send(dst_socket, send_buf1, 256, 0);
    //     send(dst_socket, send_buf2, 256, 0);

    //     // Packetの受信
    //     recv(dst_socket, recv_buf, 256, 0);

    //     // 受信結果の表示
    //     std::cout << "合計は" << atoi(recv_buf) << std::endl
    //               << std::endl;
    // }

    // // 解放処理
    // closesocket(dst_socket);

    // // WinSockの終了処理
    // WSACleanup();
    // return;
    return 0;
}