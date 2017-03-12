/*
* Universidade Federal do Mato Grosso do Sul
* Faculdade de Computação
* Redes de Computadores
*
* Mateus Gabi Moreira / Engenharia de Software
* Pedro Souza Júnior / Ciência da Computação
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <netdb.h>

#include <pthread.h> 
#include <sys/time.h>

/* método que gerencia a requisição do cliente */
static void *Request_Manager(void *);

/* porta que o proxy irá escutar */
int Porta;

/* estrutura que serve para saber qual socket e sua thread */
struct Var{
	int Sock;
	pthread_t Ponteiro;
};

/* cache do proxy, isso "diminui" o tempo de resposta */
struct Cache {
	int size_MB;
	/* adicionar hashtable */
	/* adicionar fila */
};

/* método que manipula errors */
void error(char* str){
	printf("%s\n", str);
}

/**
* Veja como configurar o firefox:
*
* http://wiki.uepg.br/index.php/Proxy_para_Mozilla_Firefox
*
* argv[1] -> Porta
* argv[2] -> tamanho da cache em MB
*/
int main (int argc, char const *argv[])
{
	int sock, newsock;
	int clilen;

	struct Var *var;
	struct Cache *cache;

	/* declarando estrutura do socket*/
	struct sockaddr_in client_a;
	struct sockaddr_in server_a;

	if(argc == 3)
	{
		Porta = atoi(argv[1]);
	}
	else
	{
		error("Siga a formatação: ./proxy [porta] [tamanho_da_cache_em_MB]");
		return 0;
	}

	/*seta a estrutura com o valor zero*/
	memset((char*)&client_a, 0, sizeof(struct sockaddr_in));
	memset((char*)&server_a, 0, sizeof(struct sockaddr_in));

	/*setar as opções do socket*/
	server_a.sin_family = AF_INET;
	server_a.sin_port = htons(Porta);
	server_a.sin_addr.s_addr = INADDR_ANY;

	/*inicializar o socket com as opções TCP */
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	/*verifica se ocorreu erro na inicialização*/
	if (sock < 0)
	{
		error("Problema ao iniciar socket");
		return 0;
	}

	/*bindar ao socket*/
	if(bind(sock, (struct sockaddr*)&server_a, sizeof(server_a)) < 0)
	{
		error("Erro ao bindar");
		return 0;
	}


	/* criar estrutura de dados da cache */
	cache = (struct Cache*) malloc (sizeof(struct Cache));
	cache->size_MB = atoi(argv[2]);

	printf("Tamanhho da cache: %d\n", cache->size_MB);

	while(1)
	{
		printf("Escutando conexoes...->%d\n", newsock);
		/*escutar tentativas de conexão*/
		
		listen(sock, 64);
		clilen = sizeof(client_a);

		newsock = accept(sock, (struct sockaddr*)&client_a, (socklen_t*)&clilen);
		printf("Nova conexao encontrada. Aceitando.\n");

		if(newsock < 0) error("Problema ao aceitar conexão");

		/*aloca uma estrutura para passar os parametros*/
		var = (struct Var*) malloc (sizeof(struct Var));
		var->Sock = newsock;

		printf("Gerando thread para cuidar da conexao.->%d\n", newsock);

		pthread_create(&(var->Ponteiro), NULL, Request_Manager, (void*) var);

	}
}

static void *Request_Manager(void *Var2){
	int Socket_srv1, Socket_srv2;
	struct Var* Var;
	struct Cache* cache;
	char buffer[40000];
	size_t buff_size = 39999;
	char Request[40000];
	char Host[40000];
	char Body[40000];

	struct hostent * Server;
	struct sockaddr_in Srv_addr;

	/*size_t Tam1, Tam2, Tam3, Tam4;*/

	int J;

	char* SiteBloqueado = "HTTP/1.0 200 OK\nConnection: close\n\n<html><head><title>An Acesso Negado</title></head><body>Dominio bloqueado.</body></html>";

	Var = (struct Var*) Var2;

	printf("Thread: Recebendo requisicao. ->%d\n", Var->Sock);
	
	/*recebe requisição do cliente*/
	if((J = recv(Var->Sock, buffer, buff_size, 0)) < 0){
		printf("Erro ao receber dados de requisicao.\n");
	}
	else{
		printf("Thread: %d bytes recebidos. Processando requisicao.\n", J);

		printf("Tamanho da cache: %d\n", cache->size_MB);

		/*Inicializa a estrutura de conexao*/
		memset((char*)&Srv_addr, 0, sizeof(Srv_addr));
		Srv_addr.sin_port	= htons(80);
		Srv_addr.sin_family	= AF_INET;
	
		/*Isola o endereço do host*/
		sscanf(buffer, "%[^\n] %[^\n] %s", Request, Host, Body);
	    sscanf(Host, " Host: %s ", Host);
		
		/*Caso o tipo de requisicao seja GET*/
		if(!strncmp(Request, "GET", 3)){
			printf("Thread: Consultando DNS para \"%s\"\n", Host);
			
			/*Resolve o endereço do host*/
			if((Server = gethostbyname(Host)) == 0)
				printf("Thread: Erro ao resolver host\n");
	
			
			memcpy((char*) &Srv_addr.sin_addr.s_addr, Server->h_addr_list[0], Server->h_length);

			/*Verifica se o site é permitido*/
			if(1){
				/*Conecta socket*/
				printf("Thread: Configurando Socket\n");
				if((Socket_srv1 = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
					printf("Thread: Erro\n");
				
				/*conecta ao site*/
				printf("Thread: Conectando ao servidor\n");
				if((Socket_srv2 = connect(Socket_srv1, (struct sockaddr*) &Srv_addr, (socklen_t) sizeof(struct sockaddr))) < 0)
					printf("Thread: Erro %d\n", (int )Socket_srv2);
				else
					printf("Thread: Conectado ao servidor com sucesso.\n");

				/*Verifica se o site possui cache
				if(tem_cache()){
					//Envia o buffer para o servidor
					write(Socket_srv2, buffer_modificado, buff_size);

					//Pedir cabeçalho

					//compara data de modificação
					if(cache_ok){
						//Fecha conexao server

						//Envia cache para cliente

					}
					else{
						//Faz requisição completa

						//Envia o buffer para o servidor
						J = send(Socket_srv1, buffer, strlen(buffer), 0);

						//recebe dados do site, trata os dados e repassa ao cliente
						if(J < 0)
							printf("Thread: Error writing to socket\n");
						else{
							do
							{
								//Limpa buffer
								memset(buffer, 0, 40000);

								//Recebe resposta do servidor
								J = recv(Socket_srv1, buffer, 40000, 0);
								
								//Encaminha o buffer para o cliente
								if(J > 0)
									send(Var->Sock, buffer, J, 0);

							}while(J > 0);
						}
					}
				}
				else{*/

					/*Envia o buffer para o servidor*/
					J = send(Socket_srv1, buffer, strlen(buffer), 0);

					/*recebe dados do site, trata os dados e repassa ao cliente*/
					if(J < 0)
						printf("Thread: Error writing to socket\n");
					else{
						do
						{
							/*Limpa buffer*/
							memset(buffer, 0, 40000);

							/*Recebe resposta do servidor*/
							J = recv(Socket_srv1, buffer, 40000, 0);
							
							/*Encaminha o buffer para o cliente*/
							if(J > 0)
								send(Var->Sock, buffer, J, 0);

						}while(J > 0);
					}
				/*}*/
				close(Socket_srv2);
				close(Socket_srv1);
			}
			/*Envia mensagem de site bloqueado*/
			else{
				/*Encaminha o buffer para o cliente*/
				send(Var->Sock, SiteBloqueado, strlen(SiteBloqueado), 0);
			}
		}
		else{
			printf("Thread: Pacote nao reconhecido.\n\t%s\n", Request);
		}
	}
	close(Var->Sock);
	free(Var);
	pthread_exit(0);
}

/****************************************

		EXEMPLO DE REQUISIÇÃO

*****************************************

GET /index.html HTTP/1.1
Host: www.example.com
Connection: close

****************************************

		EXEMPLO DE RESPOSTA

*****************************************

HTTP/1.1 200 OK
Date: Mon, 23 May 2005 22:38:34 GMT
Server: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)
Last-Modified: Wed, 08 Jan 2003 23:11:55 GMT
ETag: "3f80f-1b6-3e1cb03b"
Content-Type: text/html; charset=UTF-8
Content-Length: 138
Accept-Ranges: bytes
Connection: close

<html>
<head>
  <title>An Example Page</title>
</head>
<body>
  Hello World, this is a very simple HTML document.
</body>
</html>

*/