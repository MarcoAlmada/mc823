/*
** talker.c -- a datagram "client" demo
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
#include <sys/time.h>

#define SERVERPORT "4950"    // the port users will be connecting to
#define MAXDATASIZE 200123

int main(int argc, char *argv[])
{
    int sockfd;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    int rv;
    int numbytes;
    int estoque;

    if (argc != 2) {
        fprintf(stderr,"usage: talker hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "talker: failed to bind socket\n");
        return 2;
    }
    
	int opt, cont;
	char ISBN[20];
	char msg[MAXDATASIZE];
	char pass[20];
	int bytes_sent, len, bytes_rcv;
	
	while(1){
		ISBN[0] = '\0';
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
			else{
			  printf("Digite o ISBN\n");
			  scanf(" %s", ISBN);
			  printf("Digite o novo valor do estoque\n");
			  scanf("%d", &estoque);
			}
		}
		if(opt == 2 || opt == 3 || opt == 6){
			printf("Digite o ISBN\n");
			scanf(" %s", ISBN);
		}
		
		//escreve requisicao
		if (opt == 5) sprintf(msg, "%d %s %d", opt, ISBN, estoque);
		else sprintf(msg, "%d %s", opt, ISBN);  
		
		
		//pega tempo inicial
		struct timeval tempo_in, tempo_fim;
		gettimeofday(&tempo_in, NULL);
		
		len = strlen(msg);
		/*if((bytes_sent = send(sockfd, msg, len, 0)) != len){
			printf("erro no send\n");
			continue;	
		}*/
		
		if ((numbytes = sendto(sockfd, msg, len, 0,
		         p->ai_addr, p->ai_addrlen)) == -1) {
		    perror("talker: sendto");
		    exit(1);
		}
	
		//le resultado
		//while(1){
			addr_len = sizeof their_addr;
			if ((numbytes = recvfrom(sockfd, buf, MAXDATASIZE-1 , 0,
		        (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        		perror("erro no recvfrom");
        		exit(1);
    		}
    		
		   /* if ((bytes_rcv = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
		        perror("erro no recv");
		        break;
		    }*/
		    sscanf(buf, "%d", &cont);    		    
		    printf("%s", buf);
		   // if(cont == 0) break;
		    //		    printf("%d\n", cont);
		    //       		    printf("%s", buf+2);
		    //		    printf("\n%d", cont);
		//}
		
		//pega tempo final
		gettimeofday(&tempo_fim, NULL);
		double tempo1, tempo2;
		tempo1 = tempo_in.tv_sec + 0.000001*tempo_in.tv_usec;
		tempo2 = tempo_fim.tv_sec + 0.000001*tempo_fim.tv_usec;
		printf("\nTempo total: %lf\n", tempo2-tempo1);
	}

    freeaddrinfo(servinfo);

    //printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
    close(sockfd);

    return 0;
}
