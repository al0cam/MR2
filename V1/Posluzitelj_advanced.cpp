#include <stdlib.h> /* atoi(), exit()*/
#include <arpa/inet.h>
#include <stdio.h> /* printf(); alternativno cout i iostream*/
#include <string.h> /* memset() */

#define MAX_MSG 10000

int main(int argc, char *argv[]) 
{
    int udpSock, rc, n;
    socklen_t cliLen;
    struct sockaddr_in cliAddr, servAddr;
    char meduspremnik[MAX_MSG];
    short portNum;

    if(argc!=2)
    {
        printf("Krivi broj argumenata!\nUpotreba programa: %s brojPorta\n",argv[0]);
        exit(1);
    }

    portNum=atoi(argv[1]);
    
    /* stvaranje socketa */
    udpSock=socket(AF_INET, SOCK_DGRAM, 0);

    if(udpSock<0) 
    {
        printf("%s: ne mogu otvoriti soket \n",argv[0]);
        exit(1);
    }

    /* povezivanje posluzitelja s portom */
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(portNum);
    rc = bind (udpSock, (struct sockaddr *) &servAddr,sizeof(servAddr));

    if(rc<0) 
    {
        printf("%s: ne mogu se povezati s portom broj %d \n", argv[0], portNum);
        exit(1);
    }
    printf("%s: cekam podatke na udp portu %u\n",argv[0],portNum);
    while(1)
    {
        /* popuni meduspremnik nulama */
        memset(meduspremnik,0,MAX_MSG);

        /* primi poruku */
        cliLen = sizeof(cliAddr);
        n = recvfrom(udpSock, meduspremnik, MAX_MSG, 0/*flags=0*/,(struct sockaddr *) &cliAddr, &cliLen);

        if(n<0)
        {
            printf("%s: ne mogu primiti podatke \n",argv[0]);
            continue;
        }

        /* ispis primljene poruke */
        printf("%s:od %s:UDP%u : %s \n", argv[0],inet_ntoa(cliAddr.sin_addr),ntohs(cliAddr.sin_port),meduspremnik);

       /*posalji poruku*/
        sendto(udpSock,meduspremnik,n,0,(struct sockaddr *)&cliAddr,cliLen);
    }
    return 0;
}