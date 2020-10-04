#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <math.h>

int main(int argc, char *argv[]){

	int sockfd, port;
      	char *host = strtok(argv[1], ":");
	char *portno = strtok(NULL, ":");
	port = atoi(portno);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1){
		printf("Error creating socket");
	}


	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(host);

	int connection = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (connection == 0){

		printf("\n OK \n");
	}
	char server_msg[256];
	recv(sockfd, &server_msg, sizeof(server_msg), 0);
	char *array[3];
	int i = 0;
	array[i] = strtok(server_msg, " ");
	while (array[i] != NULL) {
		array[++i] = strtok(NULL, " ");
	}
	int o1 = atoi(array[1]);
	int o2 = atoi(array[2]);
	float f1 = atof(array[1]);
	float f2 = atof(array[2]);
	int result;
	float fresult;

	if (server_msg[0] != 'f') {

		if (strcmp("add", array[0]) == 0) {
			result = o1 + o2;
		}

		else if (strcmp("sub", array[0]) == 0) {
			result = o1 - o2;
		}

		else if (strcmp("div", array[0]) == 0) {
			result = o1 / o2;
		}

		else if (strcmp("mul", array[0]) == 0) {
			result = o1 * o2;
		}

		printf("\n result : %d \n", result);
		send(sockfd, &result, sizeof(result), 0);

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
		send(sockfd, &fresult, sizeof(fresult), 0);
	}

	char buffer2[5];
	recv(sockfd, &buffer2, sizeof(buffer2), 0);
	printf("\n %s \n", buffer2);
	return 0;

}
