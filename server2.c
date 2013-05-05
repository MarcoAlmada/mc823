/*
** listener.c -- a datagram sockets "server" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MYPORT "4950"    // the port users will be connecting to

#define MAXBUFLEN 100

#define MAXDATASIZE 200123 // max number of bytes we can get at once

#define MAXBIBSIZE 10

struct livro{
	char ISBN[14];
	char titulo[1000];
	char descricao[100123];
	int estoque;
	char autor[100];
	char editora[100];
	int ano;
};

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;
    char buf[MAXDATASIZE];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);

    printf("listener: waiting to recvfrom...\n");
    
    //preprocessamento
	struct livro biblioteca[MAXBIBSIZE];
	char aux[MAXDATASIZE];
	FILE *dados = fopen("dados.txt", "r");
	int i = 0, j;
	while(!feof(dados) ){
		fgets(biblioteca[i].ISBN, 11, dados);
		fgetc(dados);
		
		fgets(biblioteca[i].titulo, 1000, dados);
		//printf("%s \n", biblioteca[i].titulo);
		fgets(biblioteca[i].descricao, MAXDATASIZE-1, dados);
		
		fgets(aux, MAXDATASIZE, dados);
		biblioteca[i].estoque = atoi(aux);
		
		fgets(biblioteca[i].autor, MAXDATASIZE-1, dados);
		
		fgets(biblioteca[i].editora, MAXDATASIZE-1, dados);
		
		fgets(aux, MAXDATASIZE-1, dados);
		biblioteca[i].ano = atoi(aux);
		i++;
		
	}
	int total_livros = i;
	printf("--> %d\n", total_livros);

    addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfd, buf, MAXDATASIZE-1 , 0,
        (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("erro no recvfrom");
        exit(1);
    }

    printf("listener: got packet from %s\n",
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s));
    printf("listener: packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    printf("listener: packet contains \"%s\"\n", buf);
    
    char msg[4] = "ok\n";
    if ((numbytes = sendto(sockfd, msg, 4, 0,
		         (struct sockaddr *)&their_addr, addr_len)) == -1) {
		    perror("talker: sendto");
		    exit(1);
		}

    close(sockfd);

    return 0;
}
