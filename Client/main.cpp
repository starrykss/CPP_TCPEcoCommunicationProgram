#define _WINSOCK_DEPRECATED_NO_WARNINGS // inet_addr() 함수를 그대로 사용하기 위해 선언
#include <iostream>
#include <string>
#include <winsock2.h>
using namespace std;

void ShowErrorMessage(string message) {
    cout << "[오류 발생]: " << message << '\n';
    system("pause");
    exit(1);
}

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    SOCKADDR_IN serverAddress;

    int serverPort = 9876;
    char received[256];
    string sent;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {  // winsock 초기화
        ShowErrorMessage("WSAStartup()");
    }

    clientSocket = socket(PF_INET, SOCK_STREAM, 0);  // TCP 소켓 생성

    if (clientSocket == INVALID_SOCKET) {
        ShowErrorMessage("socket()");
    }

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");	  // 문자열 IP를 네트워크 바이트 형식으로. 127.0.0.1 은 자신의 컴퓨터를 나타내는 약속된 IP 주소
    serverAddress.sin_port = htons(serverPort);	// 2바이트 정수 네트워크 바이트 형식으로 

    if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        ShowErrorMessage("conncet()");
    }
    cout << "[현재 상태] connect()\n";

    while (1) {  // 반복적으로 서버로 메시지를 전송하고 에코 메시지를 전달 받음.
        cout << "[메시지 전송]: ";
        getline(cin, sent);
        if (sent == "") continue;  // 엔터 무시
        send(clientSocket, sent.c_str(), sent.length(), 0);
        int length = recv(clientSocket, received, sizeof(received), 0);
        received[length] = '\0';
        if (strcmp(received, "[exit]") == 0) {
            cout << "[서버 종료]\n";
            break;
        }
        cout << "[서버 메시지]: " << received << '\n';
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}