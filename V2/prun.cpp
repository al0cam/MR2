#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctime>
#include <iostream>


#define IPV4 "127.0.0.1"
#define PORT "53000"
#define ERR 1;
#define OK 0;
#define VELICINA_REDA_CEKANJA 5
#define VELICINA 40

int main(int argc, char **argv) {
	int povratna;
	int opisnik, opisnik_klijent; 
	struct addrinfo upute; 
	struct addrinfo *rezultat;
	struct sockaddr_storage adresa_klijent; 
	socklen_t adresa_klijent_velicina; 
	char medjuspremnik[VELICINA] = {'\0'}; 
	int procitano;
	time_t now = time(0);

	std::string dt = ctime(&now);
	std::string PORUKA = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 60\r\n\r\n<!DOCTYPE html> <html><body> 28.2.2024 12:07 </body> </html>\n";
	
    memset(&upute, 0, sizeof(struct addrinfo));
	upute.ai_family = AF_INET; 
	upute.ai_socktype = SOCK_STREAM;
	povratna = getaddrinfo(IPV4, PORT, &upute, &rezultat);
	if(povratna != 0) {
		printf("getaddrinfo(): %s (%d)\n", gai_strerror(povratna), povratna);
		return ERR;
	}

	opisnik = socket(rezultat->ai_family, rezultat->ai_socktype, rezultat->ai_protocol);
	povratna = bind(opisnik, rezultat->ai_addr, rezultat->ai_addrlen);
	if(povratna == -1) {
		int brojgreske = errno;
		printf("bind(): %s (%d)\n", strerror(brojgreske), brojgreske);
		freeaddrinfo(rezultat);
		return ERR;
	}
	freeaddrinfo(rezultat);
	povratna = listen(opisnik, VELICINA_REDA_CEKANJA);
	if(povratna == -1) {
		int brojgreske = errno;
		printf("listen(): %s (%d)\n", strerror(brojgreske), brojgreske);
		return ERR;
	}
    
	while(1) {
		adresa_klijent_velicina = sizeof adresa_klijent;
		opisnik_klijent = accept(opisnik, (struct sockaddr *)&adresa_klijent, &adresa_klijent_velicina);
		if(opisnik_klijent == -1) {
			int brojgreske = errno;
			printf("accept(): %s (%d)\n", strerror(brojgreske), brojgreske);
			return ERR;
		}
		printf("Povezao se klijent.\n");
		procitano = recv(opisnik_klijent, medjuspremnik, VELICINA, 0);
		if(procitano < 0 ){
			printf("Error\n");
		}
		if(memcmp(medjuspremnik, "SAMPLE TIME", strlen("SAMPLE TIME"))== 0)
		{
			povratna = send(opisnik_klijent, dt.c_str(), dt.size()+1, 0);
			if(povratna == -1) {
				int brojgreske = errno;
				printf("send(): %s (%d)\n", strerror(brojgreske), brojgreske);
				return ERR;
			}
		}
		if(memcmp(medjuspremnik, "GET / HTTP/1.1", strlen("GET / HTTP/1.1"))== 0)
		{
			povratna = send(opisnik_klijent, PORUKA.c_str(), PORUKA.length(), 0);
			if(povratna == -1) {
				int brojgreske = errno;
				printf("send(): %s (%d)\n", strerror(brojgreske), brojgreske);
				return ERR;
			}
		}
		printf("%s\n", medjuspremnik);		
		close(opisnik_klijent);
		}	
	return OK;
}
