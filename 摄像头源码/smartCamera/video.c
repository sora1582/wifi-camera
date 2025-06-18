#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include "video.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void *send_video_data(void *arg)
{
	int ali_port = *(int *)arg;
	printf("ali server port %d\n", ali_port);

	//create TCP
	int video_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == video_sockfd)
	{
		perror("Tcp Socket");
		exit(1);
	}

	struct sockaddr_in video_addr;
	memset(&video_addr, 0, sizeof(video_addr));
	video_addr.sin_family = AF_INET;
	video_addr.sin_port = htons(VIDEOPORT);
	video_addr.sin_addr.s_addr = inet_addr(VIDEOADDR);

	if (connect(video_sockfd, (struct sockaddr *)&video_addr, sizeof(video_addr)) < 0)
	{
		perror("connect video server");
		exit(1);
	}

	printf("connect to mjpg-streamer success\n");

	//create UDP
	int ali_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == ali_sockfd)
	{
		perror("udp socket");
		exit(1);
	}

	struct sockaddr_in ali_addr;
	memset(&ali_addr, 0, sizeof(ali_addr));
	ali_addr.sin_family = AF_INET;
	ali_addr.sin_port = htons(ali_port);
	ali_addr.sin_addr.s_addr = inet_addr(ALI_ADDR);

	//send post 
	char *buf = (char *)malloc(BUFLEN);
	memset(buf, 0, BUFLEN);
	strcpy(buf, "GET /?action=stream\n");
	if (send(video_sockfd, buf, strlen(buf), 0) < 0)
	{
		perror("send post");
		exit(1);
	}

	//send userinfo
	if (send(video_sockfd, "f\n", 2, 0) < 0)
	{
		perror("send userinfo");
		exit(1);
	}

	memset(buf, 0, BUFLEN);
	if (recv(video_sockfd, buf, BUFLEN, 0) < 0)
	{
		perror("recv head info");
	}

	int recv_size;
	char *begin, *end;
	char cont_len[10] = {0};
	while (1)
	{
		//recv head info
		memset(buf, 0, BUFLEN);
		recv_size = recv(video_sockfd, buf, BUFLEN, 0);
		if (-1 == recv_size)
		{
			perror("recv");
		}

		if (strstr(buf, "Content-Length"))
		{
			begin = strstr(buf, "Content-Length");
			end = strstr(buf, "X-Timestamp");
			memcpy(cont_len, begin + 16,  end - 2 - begin - 16);
			printf("recv head Content-Length = %d\n", atoi(cont_len));
		}
		else
		{
			continue;
		}

		//recv fram
		memset(buf, 0, BUFLEN);
		recv_size = recv(video_sockfd, buf, atoi(cont_len), 0);
		printf("recv picture data %d\n", recv_size);

		//send to server
		if (sendto(ali_sockfd, buf, recv_size, 0, (struct sockaddr *)&ali_addr, 
				sizeof(ali_addr)) < 0)
		{
			perror("sendto ali server");
		}

		memset(buf, 0, BUFLEN);
		recv_size = recv(video_sockfd, buf, 1024, 0);
	}

	/*char *pic_data = (char *)malloc(BUFLEN);
	int pic_length = 0, p = 0;
	while (1)
	{
		memset(buf, 0, BUFLEN);
		recv_size = recv(video_sockfd, buf, 74, 0);
		if (strstr(buf, "Content-Type"))
		{
			begin = strstr(buf, "Content-Length");
			end = strstr(buf, "X-Timestamp");
			memcpy(cont_len, begin + 16,  end - 2 - begin - 16);
			pic_length = atoi(cont_len);
			printf("recv head Content-Length = %d %d\n", atoi(cont_len), recv_size);
			memset(cont_len, 0, 10);
		}
		else 
		{
			continue;
		}

		while (1)
		{
			memset(buf, 0, BUFLEN);
			recv_size = recv(video_sockfd, buf, pic_length, 0);
			if (recv_size == pic_length)
			{
				memcpy(pic_data + p, buf, recv_size);
				p += recv_size;
				sendto(ali_sockfd, pic_data, p, 0, (struct sockaddr *)&ali_addr, 
					sizeof(ali_addr));
				printf("send a pic to server %d\n", p);
				p = 0;
				memset(pic_data, 0, BUFLEN);
				pic_length = 0;
				break;
			}
			else
			{
				memcpy(pic_data + p, buf, recv_size);
				pic_length = pic_length - recv_size;
				p += recv_size;
			}
		}

		recv(video_sockfd, buf, 24, 0);
	}*/
}
