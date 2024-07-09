#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <pthread.h>
#define PORT 4444

long long factorial(long long n){

	unsigned long long p= 1;
    if(n>20){
        for (int i = 1 ; i <= 20 ; i++){
		    p *= i;
	    }
        return p;    
    }
	for (int i = 1 ; i <= n ; i++){
		p *= i;
	}
	return p;

}
long long f(long long n){
    unsigned long long p=1,q=1;
    for(int i=1;i<=20;i++){
        q*=i;
    }
    for(int i=1;i<=n;i++){
        p=(((p%q)*(i%q))%q);
    }
    if(n>=20){
        return p+q;
    }
    return p;
}
int check(int e, const char* msg){
	if( e < 0){
		perror(msg);
		exit(1);
	}
}
void *handle_client(void *arg) {
    int client_socket = *((int *)arg);
    char buffer[1024];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        int n = atoi(buffer);
        int factorial = fact(n);
        char response[1024];
        snprintf(response, sizeof(response), "Factorial of %d is %d\n", n, factorial);
        send(client_socket, response, strlen(response), 0);
    }
    close(client_socket);
    free(arg);
    return NULL;
}
int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            perror("accept");
            continue;
        }

        int *arg = (int *)malloc(sizeof(int));
        if (arg == NULL) {
            perror("malloc");
            close(client_socket);
            continue;
        }
        *arg = client_socket;

        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, arg) != 0) {
            perror("pthread_create");
            free(arg);
            close(client_socket);
        }
    }

    close(server_socket);
    return 0;
}
