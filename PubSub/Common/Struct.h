#pragma once
#include <WinSock2.h>

#define MAX_TOPIC 21

// Struktura poruke
typedef struct message {
    char topic[MAX_TOPIC];    // Teme: snaga, jačina, napon
    int location;             // Lokacija senzora ili uređaja
    char text[491];           // Poruka
} MESSAGE;

// Lista
typedef struct uticnica {
    SOCKET acceptedSocket;
    struct uticnica* next;
} UTICNICA;

// Red
typedef struct queue {
    struct node* front;
    struct node* back;
} QUEUE;

typedef struct node {
    struct message data;
    struct node* next;
} NODE;

// Hash tabela za pretplatnike
typedef struct subscribers {
    char topic[MAX_TOPIC];            // Teme: snaga, jačina, napon
    int location;                     // Lokacija
    uticnica* acceptedSockets;     // Lista pretplatnika
    struct subscribers* next;         // Sledeći pretplatnik
} SUBSCRIBERS;
