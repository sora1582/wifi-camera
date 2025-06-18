#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <json/json.h>
#include <string.h>
#include "camera.h"
#include <pthread.h>
#include <unistd.h>
#include "node.h"

extern int port;
extern pthread_mutex_t mutex;
extern char *pic_data;
extern int pic_length;
extern Node *head;

void *app_video_data(void *arg)
{
	//创建 UDP socket
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sockfd)
	{
		perror("app udp socket failure");
		return NULL;
	}

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port - 1);
	server_addr.sin_addr.s_addr = inet_addr("172.31.251.104");

	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind");
		return NULL;
	}

	char *buf = (char *)malloc(BUFLEN);
	struct sockaddr_in app_addr;
	int length = sizeof(app_addr);
	recvfrom(sockfd, buf, 16, 0, (struct sockaddr *)&app_addr, (socklen_t *)&length);

	while (1)
	{
		pthread_mutex_lock(&mutex);				
		memcpy(buf, pic_data, pic_length);
		pthread_mutex_unlock(&mutex);

		if (sendto(sockfd, buf, pic_length, 0, (struct sockaddr *)&app_addr, length) < 0)
		{
			perror("send pic failure");
		}

		usleep(40000);
	}

}

void app_send_video_data(int fd)
{	
	//启动线程 创建UDP Socket
	pthread_t tid;
	pthread_create(&tid, NULL, app_video_data, NULL);

	//返回端口信息
	struct json_object *obj = json_object_new_object();
	json_object_object_add(obj, "cmd", json_object_new_string("reply_port_info"));
	json_object_object_add(obj, "port", json_object_new_int(port));
	const char *s = json_object_to_json_string(obj);

	port++;

	if (send(fd, s, strlen(s), 0) < 0)
	{
		perror("send to app");
		return;
	}

}

void app_send_control_info(struct json_object *obj)
{
	struct json_object *val;
	json_object_object_get_ex(obj, "deviceid", &val);
	const char *deviceid = json_object_get_string(val);
	
	int fd = TraverseLink(deviceid);
	if (-1 == fd)
	{
		printf("摄像头不存在\n");
		return;
	}

	const char *s = json_object_to_json_string(obj);
	if (send(fd, s, strlen(s), 0) < 0)
	{
		printf("服务器转发指令失败\n");
	}
}
