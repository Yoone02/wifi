#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

char buffer[512];
int ma_socket;

void main ( void )
{
int client_socket;
struct sockaddr_in mon_address, client_address;
int mon_address_longueur, lg;

bzero(&mon_address,sizeof(mon_address));
mon_address.sin_port = htons(30000);
mon_address.sin_family = AF_INET;
mon_address.sin_addr.s_addr = htonl(INADDR_ANY);

/* creation de socket /
if ((ma_socket = socket(AF_INET,SOCK_STREAM,0))== -1)
{
printf(“la creation rate\n”);
exit(0);
}
signal(SIGINT,fin);
/ bind serveur - socket */
bind(ma_socket,(struct sockaddr *)&mon_address,sizeof(mon_address));

/* ecoute sur la socket */
listen(ma_socket,5);

/* accept la connexion */
mon_address_longueur = sizeof(client_address);
while(1)
{
client_socket = accept(ma_socket,
(struct sockaddr *)&client_address,
&mon_address_longueur);

if (fork() == 0)
{
close(ma_socket);

lg = read(client_socket,buffer, 512);
printf("le serveur a recu: %s\n",buffer);
sprintf(buffer,"%s du serveur",buffer);
write(client_socket,buffer, 512);
shutdown(client_socket,2);
close(client_socket);
exit(0);

}
}
shutdown(ma_socket,2);
close(ma_socket);
}[/code]

client.c :

[code]// compilation : gcc -Wall -o client.o -c client.c
// gcc -o client client.o

#include <stdio.h>
#include <errno.h>
#include <signal.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SERVEURNAME “192.168.1.106” // adresse IP de mon serveur

int to_server_socket = -1;

void main ( void )
{

char *server_name = SERVEURNAME;
struct sockaddr_in serverSockAddr;
struct hostent *serverHostEnt;
long hostAddr;
long status;
char buffer[512];

bzero(&serverSockAddr,sizeof(serverSockAddr));
hostAddr = inet_addr(SERVEURNAME);
if ( (long)hostAddr != (long)-1)
bcopy(&hostAddr,&serverSockAddr.sin_addr,sizeof(hostAddr));
else
{
serverHostEnt = gethostbyname(SERVEURNAME);
if (serverHostEnt == NULL)
{
printf(“gethost rate\n”);
exit(0);
}
bcopy(serverHostEnt->h_addr,&serverSockAddr.sin_addr,serverHostEnt->h_length);
}
serverSockAddr.sin_port = htons(30000);
serverSockAddr.sin_family = AF_INET;

/* creation de la socket /
if ( (to_server_socket = socket(AF_INET,SOCK_STREAM,0)) < 0)
{
printf(“creation socket client ratee\n”);
exit(0);
}
/ requete de connexion */
if(connect( to_server_socket,
(struct sockaddr )&serverSockAddr,
sizeof(serverSockAddr)) < 0 )
{
printf(“demande de connection ratee\n”);
exit(0);
}
/ envoie de donne et reception /
write(to_server_socket,“Toto”,4);
read(to_server_socket,buffer,512);
printf(buffer);
/ fermeture de la connection */
shutdown(to_server_socket,2);
close(to_server_socket);
}