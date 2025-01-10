// Publisher.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "../Common/Publisher.h"

int main()
{
	// variable used to store function return value
	int iResult;


	if (InitializeWindowsSockets() == false)
	{
		// we won't log anything since it will be logged
		// by InitializeWindowsSockets() function
		return 1;
	}

	if (Connect())
		return 1;

	char topic[21];
	char message[491];
	int location;



	do {
		printf("Unesite TOPIC za vest: ");
		gets_s(topic, MAX_TOPIC);
		strcpy_s(topic, TopicToLower(topic));  // Pretvori u mala slova

		printf("Unesite lokaciju (broj): ");
		scanf_s("%d", &location);  // Unos lokacije kao celog broja
		getchar();  // Ukloni novi red posle unosa broja

		printf("Unesite sada poruku: ");
		gets_s(message, sizeof(message));

		// Provera za kraj unosa
		if (strcmp("kraj", topic) == 0 || strcmp("kraj", message) == 0 ||
			strcmp("exit", topic) == 0 || strcmp("exit", message) == 0) {
			break; // Izlaz iz petlje
		}

		// Poziv funkcije Publish sa dodatim parametrom za lokaciju
		iResult = Publish((void*)topic, location, (void*)message);

		// Provera greške pri slanju
		if (iResult == -1) {
			printf("Greska prilikom slanja poruke.\n");
			break;
		}

		printf("Bytes Sent: %ld\n", iResult);

	} while (true);


}


