#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdlib.h>
#include <json/json.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "video.h"
#include "motor.h"

int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);	
	if (-1 == sockfd)
	{
		perror("socket");
		exit(1);
	}

	struct sockaddr_in ali_addr;
	memset(&ali_addr, 0, sizeof(ali_addr));
	ali_addr.sin_family = AF_INET;
	ali_addr.sin_port = htons(ALI_PORT);
	ali_addr.sin_addr.s_addr = inet_addr(ALI_ADDR);

	//connect ali server
	if (connect(sockfd, (struct sockaddr *)&ali_addr, sizeof(ali_addr)) < 0)
	{
		perror("connect ali server failure");
		exit(1);
	}

	//create json object 
	struct json_object *obj = (struct json_object *)json_object_new_object();  
	json_object_object_add(obj, "cmd", json_object_new_string("info"));
	json_object_object_add(obj, "deviceid", json_object_new_string("0001"));
	const char *s = (const char *)json_object_to_json_string(obj);
	if (send(sockfd, s, strlen(s), 0) < 0)
	{
		perror("send to ali failure");
		exit(1);
	}

	//recv server port info
	char buf[256] = {0};
	if (recv(sockfd, buf, sizeof(buf), 0) < 0)
	{
		perror("recv from server failure");
		exit(1);
	}

	struct json_object *json = json_tokener_parse(buf);
	struct json_object *cmd, *port;
	int server_udp_port;
	cmd = json_object_object_get(json, "cmd");
	port = json_object_object_get(json, "port");

	if (!strcmp((const char *)json_object_get_string(cmd), "port_info"))
	{
		server_udp_port = json_object_get_int(port);
	}
	else 
	{
		printf("recv from server error\n");
	}

	//start new thread, send video data to ali server
	pthread_t tid;
	pthread_create(&tid, NULL, send_video_data, &server_udp_port);

	while (1)
	{
		memset(buf, 0, sizeof(buf));
		if (recv(sockfd, buf, sizeof(buf), 0) < 0)
		{
			perror("recv failure");
		}

		json = json_tokener_parse(buf);	
		struct json_object *cmd, *action;
		cmd = json_object_object_get(json, "cmd");
		action = json_object_object_get(json, "action");

		if (!strcmp((const char *)json_object_get_string(cmd), "control"))
		{
			const char *act = json_object_get_string(action);
			if (!strcmp(act, "left"))      //left
			{
				motor_turn_left();		
			}
			else if (!strcmp(act, "right"))
			{
				motor_turn_right();		
			}
			else if (!strcmp(act, "up"))
			{
				motor_turn_up();		
			}
			else if (!strcmp(act, "down"))
			{
				motor_turn_down();		
			}
		}
	}

	return 0;
}
