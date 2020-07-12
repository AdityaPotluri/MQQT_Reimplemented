#include<stdio.h>


typedef struct server
{
    int listen_fd;
} server_t;


int server_listen(server_t* server);
int server_accept(server_t * server);

int main(){
    int err;
    server_t server = {0};
    err=server_listen(&server);

    if(err){
        printf("Failed to listen to on address 0.0.0.0%d\n",PORT);
        return err;
    }

    while(1){
        err=server_accept(&server);
        if(err){
            printf("Failed accepting connection\n");
            return err;
        }
    }
    
    return 0;
}