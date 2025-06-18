#include <stdio.h>
#include <json/json.h>
#include "node.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "camera.h"
#include <string.h>
#include <pthread.h>

extern int port;
extern pthread_mutex_t mutex;
extern char *pic_data;
extern int pic_length;

//解析json数据得到deviceid
//操作链表，插入deviceid和fd
//返回摄像头端口信息
void camera_online(struct json_object *json, int fd)
{
	printf("进入摄像头上线处理程序\n");
	struct json_object *val;
	json_object_object_get_ex(json, "deviceid", &val);
	const char *dev_id = json_object_get_string(val);

	InsertLink(dev_id, fd);

	struct json_object *obj = json_object_new_object();
	json_object_object_add(obj, "cmd", json_object_new_string("port_info"));
	json_object_object_add(obj, "port", json_object_new_int(port));
	port++;

	const char *s = json_object_to_json_string(obj);
	if (send(fd, s, strlen(s), 0) < 0)
	{
		perror("send to camera");
		exit(1);
	}

	//启动socket 接收摄像头的视频数据
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sockfd)
	{
		perror("server udp socket");
		exit(1);
	}

	struct sockaddr_in server_udp_addr;
	memset(&server_udp_addr, 0, sizeof(server_udp_addr));
	server_udp_addr.sin_family = AF_INET;
	server_udp_addr.sin_port = htons(port - 1);
	server_udp_addr.sin_addr.s_addr = inet_addr("172.17.7.99");

	//绑定
	if (bind(sockfd, (struct sockaddr *)&server_udp_addr, sizeof(server_udp_addr)) < 0)
	{
		perror("bind error\n");
		exit(1);
	}

	struct sockaddr_in camera_addr;
	int length = sizeof(camera_addr);
	int recv_size;
	char *buf = (char *)malloc(BUFLEN);

	printf("准备接受视频数据\n");
	while (1)
	{
		memset(buf, 0, BUFLEN);
		recv_size = recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr *)&camera_addr, 
								(socklen_t *)&length);
		if (recv_size <= 0)
		{
			perror("recvfrom camera");
			exit(1);
		}

		printf("收到一张图片 %d\n", recv_size);

		pthread_mutex_lock(&mutex);
		memset(pic_data, 0, BUFLEN);
		memcpy(pic_data, buf, recv_size);
		pic_length = recv_size;
		pthread_mutex_unlock(&mutex);
	}
}
