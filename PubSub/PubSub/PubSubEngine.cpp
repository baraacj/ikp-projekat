
#include <conio.h>
#include "../Common/PubSubEngine.h"


int  main(void)
{
	// Socket used for listening for new clients 
	SOCKET listenSocketPublisher = INVALID_SOCKET;
	int iResult;



	if (InitializeWindowsSockets() == false)
	{
		// we won't log anything since it will be logged
		// by InitializeWindowsSockets() function
		return 1;
	}

	listenSocketPublisher = InitializeListenSocket(PUBLISHER_PORT);
	if (listenSocketPublisher == SOCKET_ERROR || listenSocketPublisher == INVALID_SOCKET) return 1;
	
	// Set listenSocket in listening mode
	iResult = listen(listenSocketPublisher, SOMAXCONN);

	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocketPublisher);
		WSACleanup();
		return 1;
	}

	printf("Server initialized, waiting for clients.\n");

	InitializeQUEUE(&queue);
	InitAllNecessaryCriticalSection();

	CreateAllSemaphores();

	CreateAllThreads(&listenSocketPublisher);

	if (!t1 || !t3) {

		ReleaseSemaphore(FinishSignal, 6, NULL);
	}

	while (1) {

		if (_kbhit()) {
			char c = _getch();
			if (c == 'q') {
				ReleaseSemaphore(FinishSignal, 6, NULL);
				break;
			}
		}
	}

	if (t1) {
		WaitForSingleObject(t1, INFINITE);
	}
	
	if (t3) {
		WaitForSingleObject(t3, INFINITE);
	}
	


	DeleteAllThreadsAndSemaphores();
	DeleteAllNecessaryCriticalSection();


	// clear queue
	//oslobodi memoriju koju je zauzeo queueu ukoliko ima nesto na njemu
	ClearQueue(&queue);

	ZatvoriSveSocketeZaListu(publisherSockets);

	//printf("TABELA BRE BRISANJA\n");
	//printTable(tabela);
	//printf("TABELA POSLE BRISANJA\n");
	//printTable(tabela);
	//printf("END BRISANJE TABELE\n");

	deleteList(&publisherSockets); // oslobodi memoriju koju je zauzela lista



	// cleanup
	closesocket(listenSocketPublisher);
	listenSocketPublisher = INVALID_SOCKET;
	WSACleanup();

	return 0;
}