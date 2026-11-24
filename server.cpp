
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <cstring>





std::string getMessageOnly(const char* arr, const std::string& x) {
    std::string result;
    if (!arr || x.empty()) return result;
    
    size_t arrLen = strlen(arr);
    size_t xLen = x.length();
    
    for (size_t i = 0; i + xLen <= arrLen; ) {
        if (strncmp(arr + i, x.c_str(), xLen) == 0) {
            bool isolatedBefore = (i == 0) || 
                                  (i < xLen || strncmp(arr + i - xLen, x.c_str(), xLen) != 0);
            bool isolatedAfter  = (i + xLen + xLen > arrLen) || 
                                  (strncmp(arr + i + xLen, x.c_str(), xLen) != 0);
            
            if (isolatedBefore && isolatedAfter) {
                if (i + xLen < arrLen) {
                    result += &arr[i + xLen];
                    return result;
                }
            }
            i += xLen;
        } else {
            i++;
        }
    }
    return result;
}






int main(){
    std::string message = "We finally did it";
    WSADATA wsadata;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2,2);
    wsaerr = WSAStartup(wVersionRequested,&wsadata);
    if(wsaerr != 0){
        std::cout<<"The winsock dll not found! " << std::endl;
        return 0;
    }else{
        std::cout<<"The winsock dll found! " << std::endl;
        std::cout<<"The status : " <<wsadata.szSystemStatus<< std::endl;
    }
    SOCKET mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(mySocket == INVALID_SOCKET){
        std::cerr << "error at socket!! " << WSAGetLastError() <<std::endl;
        WSACleanup();
        return 0;
    }else {
        std::cout<<"socket is OK! " << std::endl;
        }
    sockaddr_in service;
    service.sin_family = AF_INET;
    InetPtonA(AF_INET, "127.0.0.1", &service.sin_addr.s_addr);
    service.sin_port = htons(54000);
    if(bind(mySocket,(SOCKADDR*)&service,sizeof(service)) == SOCKET_ERROR){
            std::cout <<"bind failed! " << WSAGetLastError() << std::endl;
            closesocket(mySocket);
            WSACleanup();
            return 0;
        }else {
            std::cout <<"bind succesddfully! " << std::endl;
        }
    if(listen(mySocket,SOMAXCONN) == SOCKET_ERROR){
        std::cerr << "error trying to listen!! " << WSAGetLastError() <<std::endl;
    }   
    char buffer[1024];
    while (true)
    {
        std::cout << "Waiting for incoming connections..." << std::endl;
        SOCKET clientSocket = accept(mySocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            std::cout << "accept() failed: " << WSAGetLastError() << std::endl;
        } else {
            std::cout << "Client connected!" << std::endl;
            int bytesreceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if( bytesreceived== SOCKET_ERROR){
                std::cout << "recv() failed: " << WSAGetLastError() << std::endl;
                break;
            } else {
                std::cout << "DATA NOW on buffer array " << std::endl;
                buffer[bytesreceived] = '\0';
                break;
            }
        }
    }
    std::cout << "L browser galk : " << getMessageOnly(buffer,"\r\n\r\n");
    
    closesocket(mySocket);
    WSACleanup();
return 0;
}