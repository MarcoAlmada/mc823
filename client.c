/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to 

#define MAXDATASIZE 1000 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure
	
	int opt, cont;
	char ISBN[20];
	ISBN[0] = '\0';
	char msg[MAXDATASIZE];
	char pass[20];
	int bytes_sent, len, bytes_rcv;
	
	while(1){
		//pseudo user interface
		printf("Escolha uma opcao:\n");
		printf("1- listar todos os ISBN e seus respectivos titulos\n");
		printf("2- dado o ISBN de um livro, retornar descricao\n");
		printf("3- dado o ISBN de um livro, retornar todas as informacoes do livro\n");
		printf("4- listar todas as informacoes de todos os livros\n");
		printf("5- alterar numero de exemplares em estoque\n");
		printf("6- dado o ISBN de um livro, retornar numero de exemplares em estoque\n");
		printf("7- fechar cliente\n");
		scanf("%d", &opt);
		if(opt == 7) break;
		if(opt == 5){
			printf("Digite a senha\n");
			scanf(" %s", pass);
			if(strcmp(pass, "senhalivraria") != 0)
			continue;
		}
		if(opt == 2 || opt == 3 || opt == 5 || opt == 6){
			printf("Digite o ISBN\n");
			scanf(" %s", ISBN);
		}
		
		//escreve requisicao
		sprintf(msg, "%d %s", opt, ISBN);
		
		//pega tempo inicial
		struct timeval tempo_in, tempo_fim;
		gettimeofday(&tempo_in, NULL);
		
		len = strlen(msg);
		if((bytes_sent = send(sockfd, msg, len, 0)) != len){
			printf("erro no send\n");
			continue;	
		}
	
		//le resultado
		while(1){
		    if ((bytes_rcv = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
		        perror("erro no recv");
		        break;
		    }
		    sscanf(buf, "%d %s", &cont, msg);
		    printf("%s", msg);
		    if(cont == 0) break;
		}
		
		//pega tempo final
		gettimeofday(&tempo_fim, NULL);
		double tempo1, tempo2;
		tempo1 = tempo_in.tv_sec + tempo_in.tv_usec;
		tempo2 = tempo_fim.tv_sec + tempo_fim.tv_usec;
		printf("\nTempo total: %lf\n", tempo2-tempo1);
	}

    close(sockfd);

    return 0;
}
