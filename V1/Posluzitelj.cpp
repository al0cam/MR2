#include <arpa/inet.h>
#include <string.h> /* memset() */

#define MAX_MSG 10000

int main(int argc, char *argv[]) 
{
  int udpSock, rc, n;
  socklen_t cliLen;
  struct sockaddr_in cliAddr, servAddr;
  char meduspremnik[MAX_MSG];

  udpSock = socket(AF_INET, SOCK_DGRAM, 0);

  servAddr.sin_family = AF_INET;
  inet_pton(AF_INET,"0.0.0.0",&(servAddr.sin_addr.s_addr));
  servAddr.sin_port = htons(8000);
  rc = bind (udpSock, (struct sockaddr *) &servAddr,sizeof(servAddr));

   while(1) 
   {
    /* popuni meduspremnik nulama */
    memset(meduspremnik,0,MAX_MSG);
    /* primi poruku */
    cliLen = sizeof(cliAddr);
    n = recvfrom(udpSock, meduspremnik, MAX_MSG, 0,(struct sockaddr *) &cliAddr, &cliLen);
    if(n>0)/*posalji poruku nazad*/
      sendto(udpSock,meduspremnik,n,0,(struct sockaddr *)&cliAddr,cliLen);
  }
    return 0;
}