#ifndef VIDEO_H
#define VIDEO_H

//video server port addr
#define VIDEOPORT    8080
#define VIDEOADDR    "172.0.0.1"

//ali server port addr
#define ALI_PORT   8000
#define ALI_ADDR   "59.110.90.231"

#define BUFLEN     100 * 1024

void *send_video_data(void *arg);

#endif
