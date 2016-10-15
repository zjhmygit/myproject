#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#define LED_CTROL	1

int main(int argc, char**argv)
{
	int fd;
	int keyflag;

	fd = open("/dev/keyled", O_RDWR);
	if(fd < 0)
	{
		printf("open dev file fail!\n");
		return -1;
	}

	while(1)
	{
		read(fd, &keyflag, 4);
		if(keyflag == 0)
		{
			do{
				read(fd, &keyflag, 4);
				printf("keyflag = %d\n", keyflag);
			}while(keyflag == 0);

			printf("key 2 put up!\n");
			ioctl(fd,  LED_CTROL);
		}
		printf("key 2 not put down\n");
	}

	close(fd);
;	return 0;
}