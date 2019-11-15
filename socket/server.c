#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close

#define DEFAULT_PORT 8000
#define MAXLINE 4096

void thread_func(void* p_serverfd)
{
    printf("======waiting for client's request======\n");
    int n;
    int client_fd;
    char buff[4096];
    char buff_resp[128];
    int serverfd = *(int*)p_serverfd;
    while(1)
    {
        //阻塞直到有客户端连接，不然多浪费CPU资源。
        if((client_fd = accept(serverfd, (struct sockaddr*)NULL, NULL)) == -1)
        {
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            continue;
        }

        //接受客户端传过来的数据
        n = recv(client_fd, buff, MAXLINE, 0);
        //向客户端发送回应数据

        pthread_t tid = pthread_self();
        int sz = snprintf(buff_resp, sizeof(buff_resp), "Hello, you are connected, thread %lu server for you!\n", tid);

        if(send(client_fd, buff_resp, sz+1 ,0) == -1)
        {
            perror("send error");
            close(client_fd);
            exit(0);
        }

        buff[n] = '\0';
        printf("recv msg from client: %s\n", buff);
        close(client_fd);
    }
}

int main(int argc, char** argv)
{
    int    server_fd;
    struct sockaddr_in servaddr;
    int     n;

    //初始化Socket
    if( (server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }

    //初始化
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。
    servaddr.sin_port = htons(DEFAULT_PORT);//设置的端口为DEFAULT_PORT

    //将本地地址绑定到所创建的套接字上
    if( bind(server_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }

    //开始监听是否有客户端连接
    if(listen(server_fd, 10) == -1)
    {
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }

    int ret = 0;
    pthread_t pthreads[5];
    for (int i = 0; i < 5; i++)
    {
        ret = pthread_create(&pthreads[i], NULL, (void*)thread_func, (int*)&server_fd);
        if (ret != 0)
        {
            printf("create thread function error: %s(errno: %d)\n",strerror(errno),errno);
        }
    }
    for (int i = 0; i < 5; i++)
    {
        pthread_join(pthreads[i], NULL);
    }

    close(server_fd);
}
