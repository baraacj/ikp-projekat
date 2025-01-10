#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include "Struct.h" // Ovde se nalaze sve strukture kao što su MESSAGE, SOCKET_NODE, QUEUE, itd.
#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_BUFLEN 524
#define DEFAULT_PORT 27016

// Deklaracije funkcija
bool InitializeWindowsSockets();
int Connect();
int Publish(void* topic, int location, void* message);
char* TopicToLower(char* topic);

// Socket za komunikaciju sa serverom
SOCKET connectSocket = INVALID_SOCKET;

// Implementacija funkcija

bool InitializeWindowsSockets() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        return false;
    }
    return true;
}

int Connect() {
    connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (connectSocket == INVALID_SOCKET) {
        printf("Socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(DEFAULT_PORT);

    if (connect(connectSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        printf("Unable to connect to server.\n");
        closesocket(connectSocket);
        WSACleanup();
        return 1;
    }

    return 0;
}

int Publish(void* topic, int location, void* message) {
    MESSAGE msg;
    strcpy_s(msg.topic, (char*)topic);
    msg.location = location;  // Postavi lokaciju
    strcpy_s(msg.text, (char*)message);

    // Slanje poruke
    int result = send(connectSocket, (char*)&msg, sizeof(MESSAGE), 0);
    if (result == SOCKET_ERROR) {
        printf("Send failed with error: %d\n", WSAGetLastError());
        closesocket(connectSocket);
        WSACleanup();
        return -1;
    }
    return result;
}


char* TopicToLower(char* topic) {
    for (unsigned int i = 0; i < strlen(topic); i++) {
        topic[i] = tolower(topic[i]);
    }
    return topic;
}
