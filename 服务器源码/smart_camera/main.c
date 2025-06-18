#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <json/json.h>
#include "node.h"
#include "camera.h"
#include <string.h>
#include <unistd.h>
#include "app.h"

#define SERVERPORT   8000

Node *head = NULL;
int port = 9000;
char *pic_data;
int pic_length;
pthread_mutex_t mutex;

void *ClientHandler(void *arg)
{
	int fd = *(int *)arg;
	int recv_size;
	char buf[128] = {0};

	while (1)
	{
		recv_size = recv(fd, buf, sizeof(buf), 0);
		if (-1 == recv_size)
		{
			perror("recv");
			break;
		}
		else if (recv_size == 0)
		{
			printf("客户端 %d 异常下线\n", fd);
			break;
		}

		printf("TCP收到数据 %s \n", buf);

		struct json_object *obj = json_tokener_parse(buf);
		struct json_object *val;
		json_object_object_get_ex(obj, "cmd", &val);
		const char *cmd = (const char *)json_object_get_string(val);

		if (!strcmp(cmd, "info"))    //摄像头上线
		{
			camera_online(obj, fd);	
		}
		else if (!strcmp(cmd, "get_video_data"))   //APP获取视频数据
		{
			app_send_video_data(fd);		
		}
		else if (!strcmp(cmd, "control"))          //APP控制舵机旋转
		{
			app_send_control_info(obj);
		}
	}
}

int main()
{
	InitLink(&head);
	pic_data = (char *)malloc(BUFLEN);
	pthread_mutex_init(&mutex, NULL);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd)
	{
		perror("socket");
		exit(1);
	}

	int opt = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVERPORT);
	server_addr.sin_addr.s_addr = inet_addr("172.17.7.99");

	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind");
		exit(1);
	}

	if (listen(sockfd, 10) < 0)
	{
		perror("listen");
		exit(1);
	}

	struct sockaddr_in client_addr;
	int length = sizeof(client_addr);
	while (1)
	{
		int fd = accept(sockfd, (struct sockaddr *)&client_addr, (socklen_t *)&length);	
		if (-1 == fd)
		{
			perror("accept");
		}

		pthread_t tid;
		pthread_create(&tid, NULL, ClientHandler, (void *)&fd);
		pthread_detach(tid);

		usleep(10000);
	}

	return 0;
}
