#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>


/* IPV4 makro konstanta navodi IPV4 adresu na koju se klijent povezuje */
#define IPV4 "127.0.0.1"
/* PORT makro navodi broj TCP porta na koji se klijent povezuje */
#define PORT "7890"

/* maksimalna velicina poruke koju klijent moze prihvatiti od servera */
#define VELICINA 40

int main(int argc, char **argv) {
	struct addrinfo upute; /* struktura za parametriziranje getaddrinfo poziva (u engl. literaturi obicno 'hints') */
	struct addrinfo *rezultat; /* struktura koja ce sadrzavati popunjene informacije o loopback adresi servera */
	int opisnik; /* socket descriptor klijenta */
	int procitano; /* broj procitanih okteta koje nam je server poslao */
	char medjuspremnik[VELICINA] = {'\0'}; /* medjuspremnik za pohranu poruke sa servera */
	char poruka;

	/* dohvacanje adrese servera */
	memset(&upute, 0, sizeof(struct addrinfo));
	upute.ai_family = AF_INET;
	upute.ai_socktype = SOCK_STREAM;
	getaddrinfo(IPV4, PORT, &upute, &rezultat);
	/* kreiranje prikljucnice (socket-a) */
	opisnik = socket(rezultat->ai_family, rezultat->ai_socktype, rezultat->ai_protocol);
	/* povezivanje na server */
	connect(opisnik, rezultat->ai_addr, rezultat->ai_addrlen);

    char message[40] = "SIMPLE TIME";
    send(opisnik, message, 40, 0);

	/* ucitavanje poruke sa servera u lokalni medjuspremnik */
	procitano = recv(opisnik, medjuspremnik, VELICINA, 0);
	if(procitano > 0 && procitano < VELICINA) {
		medjuspremnik[procitano] = '\0';
	}
	printf("%s\n", medjuspremnik);

	close(opisnik);
	return 0;
}
