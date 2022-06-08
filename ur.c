#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>

#define BUFLEN 700000
#define LISTENPORT 1234

int main(int argc,char *argv[])
{
  
  if(argc!=2)
  {
	printf("Invalid Arguments\n");
	exit(0);
  }
 
  int sockfd, len;
  char buffer[BUFLEN];
  struct sockaddr_in receiverAddr, senderAddr;
  char filename[30];
  size_t filesize;
  for(int i=0;i<30;i++)
  {
      filename[i]='\0';
  }      
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket system call failed");
    exit(EXIT_FAILURE);
  }

  bzero(&receiverAddr,sizeof(receiverAddr));
  memset(&senderAddr, 0, sizeof(senderAddr));
  short PORTNO=(short)atoi(argv[1]);
  receiverAddr.sin_family = AF_INET;
  receiverAddr.sin_addr.s_addr = INADDR_ANY;
  receiverAddr.sin_port = htons(PORTNO);
  //memset(&(receiverAddr.sin_zero),'\0',8);
  if(bind(sockfd, (const struct sockaddr *)&receiverAddr, sizeof(receiverAddr)) < 0)
  {
    perror("bind syscall failed");
    exit(EXIT_FAILURE);
  }
  len = sizeof(senderAddr);
  recvfrom(sockfd,filename,1024,0,(struct sockaddr *)&senderAddr, &len);
  for(int i=0;filename[i]!='\0';i++)
  {
      if(filename[i]=='.')
      {
         filename[i-1]='2';
         break;
      }	 
  }    	  
  printf("%s ",filename);
  FILE *fp;
  fp=fopen(filename,"w");	  
  while(recvfrom(sockfd, buffer, 64000, 0, (struct sockaddr *)&senderAddr, &len))
  {	       
       	fwrite(buffer,64000,1,fp);
  }	 
  memset(filename, '\0', sizeof(filename));
  fclose(fp);
  return 0;
}

