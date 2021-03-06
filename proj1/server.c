/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/time.h>

#define PORT "3490"  // the port users will be connecting to

#define BACKLOG 10	 // how many pending connections queue will hold

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

void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

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
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	printf("server: waiting for connections...\n");

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
	
	//memory map
	//char *addr;
	//addr = mmap(NULL, sizeof(biblioteca), PROT_WRITE | PROT_READ, MAP_SHARED,
     //      int fd, off_t offset);
	
	int bytes_rcv;
	char buf[MAXDATASIZE];
	int opt, cont, qte;
	char ISBN[20];
	FILE *db = fopen("dados.txt", "rw");
	while(1) {  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);
		printf("server: got connection from %s\n", s);

		if (!fork()) { // this is the child process
			close(sockfd); // child doesn't need the listener
			
			//LOOP QUE PROCESSA REQUISICOES
			char bufs[MAXDATASIZE];
			
		/**/while(1){		
				if ((bytes_rcv = recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1) {
		       		perror("erro no recv");
		        	break;
		    	}
		    	
		    	int mudou = 0; //mudou estoque
		    	
		    	//pega tempo
				struct timeval tempo_in, tempo_fim;
				gettimeofday(&tempo_in, NULL);
		    	
		    	sscanf(buf, "%d", &opt);
		    	
		    	if(opt == 1){
		    		bufs[0] = '\0';
		    		strcat(bufs, "0 ");
		    		for(i = 0; i < total_livros; ++i){
		    			strcat(bufs, biblioteca[i].ISBN);
		    			strcat(bufs, " ");
		    			strcat(bufs, biblioteca[i].titulo);
		    		}
		    	}
		    	
		    	else if(opt == 2){
		    		bufs[0] = '\0';
		    		strcat(bufs, "0 ");
		    		sscanf(buf, "%d %s", &opt, ISBN);
		    		for(i = 0; i < total_livros; ++i){
		    			if(strcmp(biblioteca[i].ISBN, ISBN) == 0){
		    				strcat(bufs, biblioteca[i].descricao);
		    				break;
		    			}		    				
		    		}
		    	}
		    	
		    	else if(opt == 3){
		    		bufs[0] = '\0';
		    		strcat(bufs, "0 ");
		    		sscanf(buf, "%d %s", &opt, ISBN);
		    		for(i = 0; i < total_livros; ++i){
		    			if(strcmp(biblioteca[i].ISBN, ISBN) == 0){
		    				strcat(bufs, biblioteca[i].ISBN);
		    				strcat(bufs, "\n");
		    				strcat(bufs, biblioteca[i].titulo);
		    				strcat(bufs, biblioteca[i].descricao);
		    				sprintf(aux, "%d\n", biblioteca[i].estoque);
		    				strcat(bufs, aux);
		    				strcat(bufs, biblioteca[i].autor);
		    				strcat(bufs, biblioteca[i].editora);
		    				sprintf(aux, "%d\n", biblioteca[i].ano);
		    				strcat(bufs, aux);
		    				break;
		    			}		    				
		    		}
		    	}
		    
		    	else if(opt == 4){
		    		/*for(i = total_livros-2; i >= 0; i--){
		    			bufs[0] = '\0';
			    		if(i == 0) strcat(bufs, "0 ");
			    		else strcat(bufs, "1 ");
			    		
			    		printf("%d %s\n", i, biblioteca[i].titulo);
			    		
	    				strcat(bufs, biblioteca[i].ISBN);
	    				strcat(bufs, "\n");
	    				strcat(bufs, biblioteca[i].titulo);
	    				strcat(bufs, biblioteca[i].descricao);
	    				sprintf(aux, "%d\n", biblioteca[i].estoque);
	    				strcat(bufs, aux);
	    				strcat(bufs, biblioteca[i].autor);
	    				strcat(bufs, biblioteca[i].editora);
	    				sprintf(aux, "%d\n", biblioteca[i].ano);
	    				strcat(bufs, aux);
	    				strcat(bufs, "\n");   				
						
						printf("%s\n", bufs);
			    		if (send(new_fd, bufs, strlen(bufs)+1, 0) == -1)
						perror("send");
					}*/
					bufs[0] = '\0';
		    		strcat(bufs, "0 ");
		    		//sscanf(buf, "%d %s", &opt, ISBN);
		    		for(i = 0; i < total_livros; ++i){
		    			//if(strcmp(biblioteca[i].ISBN, ISBN) == 0){
		    				strcat(bufs, biblioteca[i].ISBN);
		    				strcat(bufs, "\n");
		    				strcat(bufs, biblioteca[i].titulo);
		    				strcat(bufs, biblioteca[i].descricao);
		    				sprintf(aux, "%d\n", biblioteca[i].estoque);
		    				strcat(bufs, aux);
		    				strcat(bufs, biblioteca[i].autor);
		    				strcat(bufs, biblioteca[i].editora);
		    				sprintf(aux, "%d\n", biblioteca[i].ano);
		    				strcat(bufs, aux);
		    				//break;
		    			//}		    				
		    		}
		    	}
		    	
		    	else if(opt == 6){
		    		bufs[0] = '\0';
		    		strcat(bufs, "0 ");
		    		sscanf(buf, "%d %s", &opt, ISBN);
		    		for(i = 0; i < total_livros; ++i){
		    			if(strcmp(biblioteca[i].ISBN, ISBN) == 0){
		    				sprintf(aux, "%d\n", biblioteca[i].estoque);
	    					strcat(bufs, aux);
		    				break;
		    			}		    				
		    		}
		    	}
		    	
		    	else if(opt == 5){
		    		bufs[0] = '\0';
		    		strcat(bufs, "0 ");
		    		sscanf(buf, "%d %s %d", &opt, ISBN, &qte);
		    		mudou = 1;
		    		for(i = 0; i < total_livros; ++i){
		    			if(strcmp(biblioteca[i].ISBN, ISBN) == 0){
		    				biblioteca[i].estoque = qte;
		    				break;
		    			}		    				
		    		}
		    	}
		    	
		    	//pega tempo final
				gettimeofday(&tempo_fim, NULL);
				double tempo1, tempo2;
				tempo1 = tempo_in.tv_sec + 0.000001*tempo_in.tv_usec;
				tempo2 = tempo_fim.tv_sec + 0.000001*tempo_fim.tv_usec;
				printf("\nTempo total: %lf\n", tempo2-tempo1);
				
				
				if (send(new_fd, bufs, strlen(bufs)+1, 0) == -1)
					perror("send");
					
				if(mudou){
					bufs[0] = '\0';
		    		for(i = 0; i < total_livros-1; ++i){
		   				strcat(bufs, biblioteca[i].ISBN);
		   				strcat(bufs, "\n");
		   				strcat(bufs, biblioteca[i].titulo);
		   				strcat(bufs, biblioteca[i].descricao);
		   				sprintf(aux, "%d\n", biblioteca[i].estoque);
		   				strcat(bufs, aux);
		   				strcat(bufs, biblioteca[i].autor);
		   				strcat(bufs, biblioteca[i].editora);
		   				sprintf(aux, "%d\n", biblioteca[i].ano);
		   				strcat(bufs, aux);			
			    	}
			    	FILE *fout = fopen("dados.txt", "w");
			    	fprintf(fout, "%s", bufs);
			    	fclose(fout);
				}
		/**/}
						
			close(new_fd);
			exit(0);
		}
		close(new_fd);  // parent doesn't need this
	}

	return 0;
}

