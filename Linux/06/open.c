#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h> // printf perror
#include <unistd.h> // close sleep
// #include <errno.h>

/***************************
  * ./open filename
  * argc    = 2
  * argv[0] = "./open"
  * argv[1] = "filename"
****************************/
  
int main(int argc, char**argv)
{
	int fd;
	
	if (argc != 2)
	{
		printf("Usage: %s <file>\n", argv[0]);
		return -1;
	}

	fd = open(argv[1], O_RDWR);
	if (fd < 0)
	{
		printf("Can not open the %s\n", argv[1]);
		perror("open");
	}
	else
	{
		printf("fd: %d\n", fd);
	}

	while(1)
	{
		sleep(10);
	}

	close(fd);
	return 0;
}
