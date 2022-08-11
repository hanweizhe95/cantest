#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <fcntl.h>

int main()
{
        
	//This is for ARM
	/*system("ifconfig can0 down");
	system("echo 250000 > /sys/devices/platform/FlexCAN.0/bitrate");
	system("ifconfig can0 up");*/
	//This is for desktop
	system("sudo ifconfig can0 down");
	system("sudo ip link set can0 type can bitrate 500000");
    	system("sudo ifconfig can0 up");
	printf("can0 is up\n");
	system("sudo ifconfig can1 down");
	system("sudo ip link set can1 type can bitrate 500000");
	system("sudo ifconfig can1 up");
	printf("can1 is up\n");
	int s, nbytes;
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame frame, frame_tx;
	struct can_filter rfilter[1];
	s = socket(PF_CAN, SOCK_RAW, CAN_RAW);//创建套接字
	strcpy(ifr.ifr_name, "can0");
	ioctl(s, SIOCGIFINDEX, &ifr); //指定can0设备
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	bind(s,(struct sockaddr *)&addr, sizeof(addr)); //将套接字与can0绑定
	fcntl(s,F_SETFL,FNDELAY);
	//定义接收规则，只接收表示符等于0x11的报文
	rfilter[0].can_id = 0x11;
	rfilter[0].can_mask = CAN_SFF_MASK;

	frame_tx.can_id = 0x1;
   	frame_tx.can_dlc = 8;
   	frame_tx.data[0] = 0;
   	frame_tx.data[1] = 1;
   	frame_tx.data[2] = 2;
   	frame_tx.data[3] = 3;
   	frame_tx.data[4] = 4;
   	frame_tx.data[5] = 5;
   	frame_tx.data[6] = 6;
   	frame_tx.data[7] = 7;
	nbytes = sendto(s, &frame_tx, sizeof(struct can_frame),0, (struct sockaddr*)&addr, sizeof(addr));
	printf("frame sent\n");
	//设置过滤规则
	while(1)
	{
		//bind(s,(struct sockaddr *)&addr, sizeof(addr));
		//setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
		nbytes = read(s, &frame, sizeof(frame)); //接收报文
		//printf("nbytes = %d; ",nbytes);
		//printf("canid = %X\n", frame.can_id);
		//显示报文
		if(nbytes > 0)
		{
			printf("frame received\n");	
			printf("ID=0x%X DLC=%d data=0x", frame.can_id, frame.can_dlc);
			for(int i=0;i<8;i++){
				printf("%X ",frame.data[i]);
			}
			printf("\n");
		}
	}
	close(s);
	
        return 0;
}

