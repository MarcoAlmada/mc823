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

  /* printf("listener: waiting to recvfrom...\n"); */
    
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
  while (1) {
    if ((numbytes = recvfrom(sockfd, buf, MAXDATASIZE-1 , 0,
                             (struct sockaddr *)&their_addr, &addr_len)) == -1) {
      perror("erro no recvfrom");
      exit(1);
    }


    /* printf("listener: got packet from %s\n", */
    /*        inet_ntop(their_addr.ss_family, */
    /*                  get_in_addr((struct sockaddr *)&their_addr), */
    /*                  s, sizeof s)); */
    /* printf("listener: packet is %d bytes long\n", numbytes); */

    /* int bytes_rcv; */
    /* char buf[MAXDATASIZE]; */
    int opt, cont, qte;
    char ISBN[20];
    char bufs[MAXDATASIZE]; // exit buffer		    	
    int mudou = 0; // mudanca no estoque
		    	
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

    /* char msg[4] = "ok\n"; */
    if ((numbytes = sendto(sockfd, bufs, strlen(bufs)+1, 0,
                           (struct sockaddr *)&their_addr, addr_len)) == -1) {
      perror("listener: sendto");
      exit(1);
    }                                
				
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
    /**/
  }					
  close(sockfd);
  //      exit(0);
  //}
  //    close(new_fd);  // parent doesn't need this
  //  }
  return 0;
}
