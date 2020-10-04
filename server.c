#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
#include <unistd.h>

int main(int argc, char *argv[]){

	int sockfd;

	char colon[] = ":";
	char *host = strtok(argv[1], colon);
	char *port_no = strtok(NULL, colon);
	int port = atoi(port_no);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1){
		printf("Error creating socket");
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(host);

	bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));

	listen(sockfd, 5);

	while (1) {

		socklen_t server_addr_size;
		server_addr_size = sizeof(struct sockaddr_in);
		int client_sock;
		client_sock = accept(sockfd, (struct sockaddr *) &server_addr, &server_addr_size);
		if (client_sock >= 0) {

			printf("\nTEXT TCP 1.0\n");
		}

		printf("\n Enter the operation : <OPERATION> <OPERAND 1> <OPERAND 2>\n \n");
		char buffer[256];
		fgets(buffer, 256, stdin);
		send(client_sock, buffer, sizeof(buffer), 0);
		char init = buffer[0];
		char *array[3];
		int i = 0;
		array[i] = strtok(buffer, " ");
		while (array[i] != NULL) {
			array[++i] = strtok(NULL, " ");
		}

		int o1 = atoi(array[1]);
		int o2 = atoi(array[2]);
		float f1 = atof(array[1]);
		float f2 = atof(array[2]);
		int result;
		float fresult;

		if (init != 'f') {

			if (strcmp("add", array[0]) == 0) {
				result = o1 + o2;
			}

				else if (strcmp("sub", array[0]) == 0) {
				result = o1 - o2;
			}

			else if (strcmp("mul", array[0]) == 0) {
				result = o1 * o2;
			}

			else if (strcmp("div", array[0]) == 0) {
				result = o1 / o2;
			}

			int buffer1;
			recv(client_sock, &buffer1, sizeof(buffer1), 0);
			printf("\n result : %d \n", result);
			printf("\n recieved result : %d \n", buffer1);
			if (abs(result - buffer1) < 0.0001) {
				char buffer2[5] = "OK";
				send(client_sock, &buffer2, sizeof(buffer2), 0);
				printf("\n %s \n", buffer2);
			}

			else {
				char buffer2[5] = "ERROR";
				send(client_sock, &buffer2, sizeof(buffer2), 0);
				printf("\n %s \n", buffer2);
			}
		}

		else {

			if (strcmp("fadd", array[0]) == 0) {
				fresult = f1 + f2;
			}

			else if (strcmp("fsub", array[0]) == 0) {
				fresult = f1 - f2;
			}

			else if (strcmp("fmul", array[0]) == 0) {
				fresult = f1 * f2;
			}

			else if (strcmp("fdiv", array[0]) == 0) {
				fresult = f1 / f2;
			}

			printf("\n fresult : %8.8g \n", fresult);
			float fbuffer1;
			recv(client_sock, &fbuffer1, sizeof(fbuffer1), 0);
			printf("\n recieved fresult : %8.8g \n", fbuffer1);
			if (abs(fresult - fbuffer1) < 0.0001) {
				char buffer2[5] = "OK";
				send(client_sock, &buffer2, sizeof(buffer2), 0);
				printf("\n %s \n", buffer2);
			}

			else {
				char buffer2[5] = "ERROR";
				send(client_sock, &buffer2, sizeof(buffer2), 0);
				printf("\n %s \n", buffer2);
			}
		}


		close(client_sock);
		sleep(1);

	}

	close(sockfd);
}
