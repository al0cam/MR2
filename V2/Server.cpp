/*

Programski kod demonstrira izvedbu jednostavnog blokirajuceg TCP servera.

Uputa za kompiliranje:
1) postaviti se u direktorij gdje je spremljena ova datoteka (naredbom 'cd')
2) izvrsiti Makefile koji je prilozen (naredbom 'make')

*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <iostream>

/* IPV4 makro konstanta navodi IPv4 adresu na kojoj ce server bind()-ati */
#define IPV4 "127.0.0.1"
/* PORT makro konstanta navodi broj TCP porta na kojem ce server primati konekcije */
#define PORT "7890"


#define ERR 1;
#define OK 0;
#define VELICINA_REDA_CEKANJA 5

int main(int argc, char **argv) 
{
	int povratna; /* privremena varijabla za pohranu povratnih vrijednosti funkcijskih poziva */
	int opisnik, opisnik_klijent; /* socket descriptor: jedan od servera i jedan koji predstavlja trenutno obradjivanog klijenta */
	struct addrinfo upute; /* struktura za parametriziranje getaddrinfo poziva (u engl. literaturi obicno 'hints') */
	struct addrinfo *rezultat; /* pokazivac na strukturu koja ce sadrzavati popunjene informacije o loopback adresi servera */
	struct sockaddr_storage adresa_klijent; /* struktura koja ce sadrzavati informacije o povezanom klijentu */
	socklen_t adresa_klijent_velicina; /* velicina strukture sockaddr_storage koja se popunjava pozivom accept() */
    time_t vrijeme;

	/* dohvacanje strukture lokalne adrese */
	memset(&upute, 0, sizeof(struct addrinfo));
	upute.ai_family = AF_INET; /* koristi se IPv4 */
	upute.ai_socktype = SOCK_STREAM;
	povratna = getaddrinfo(IPV4, PORT, &upute, &rezultat);
	if(povratna != 0) 
    {
		printf("getaddrinfo(): %s (%d)\n", gai_strerror(povratna), povratna);
		return ERR;
	}

	/* kreiranje opisnika prikljucnice (socket) */
	opisnik = socket(rezultat->ai_family, rezultat->ai_socktype, rezultat->ai_protocol);
	povratna = bind(opisnik, rezultat->ai_addr, rezultat->ai_addrlen);
	if(povratna == -1) 
    {
		int brojgreske = errno;
		printf("bind(): %s (%d)\n", strerror(brojgreske), brojgreske);
		freeaddrinfo(rezultat);
		return ERR;
	}
	freeaddrinfo(rezultat);
	povratna = listen(opisnik, VELICINA_REDA_CEKANJA);
	if(povratna == -1) 
    {
		int brojgreske = errno;
		printf("listen(): %s (%d)\n", strerror(brojgreske), brojgreske);
		return ERR;
	}
	/* u beskonacnoj petlji prihvacaju se klijenti (blokirajuci TCP server) */
	while(1) 
    {
        time(&vrijeme);
	    char *pozdravna = ctime(&vrijeme); /* poruka koju ce server poslati klijentu */
		std::string poruka = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 60\r\n\r\n<!DOCTYPE html> <html><body> 28.2.2024 12:07 </body> </html>\n";

        int *broj;
        adresa_klijent_velicina = sizeof adresa_klijent;
		opisnik_klijent = accept(opisnik, (struct sockaddr *)&adresa_klijent, &adresa_klijent_velicina);
		if(opisnik_klijent == -1) {
			int brojgreske = errno;
			printf("accept(): %s (%d)\n", strerror(brojgreske), brojgreske);
			return ERR;
		}
		printf("Povezao se klijent.\n");

        char dobivena[40] = {'\0'};

        recv(opisnik_klijent, dobivena, 40, 0);
        //std::cout << dobivena << std::endl;
        
        if(dobivena == "SIMPLE TIME")
        {
            pozdravna = ctime(&vrijeme);
        }
        else if(dobivena)
        {
            pozdravna = &poruka[0];
        }
		povratna = send(opisnik_klijent, pozdravna, poruka.length(), 0);
		if(povratna == -1) {
			int brojgreske = errno;
			printf("send(): %s (%d)\n", strerror(brojgreske), brojgreske);
			return ERR;
		} 
		close(opisnik_klijent);
	}
	return OK;
}