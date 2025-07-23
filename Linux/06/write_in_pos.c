#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h> // printf() perror()
#include <unistd.h> // close() sleep()
#include <string.h> // strlen()
// #include <errno.h>

/***************************
  * ./write_in_pos 1.txt
  * argc    >= 3
  * argv[0] = "./open"
  * argv[1] = "filename"
****************************/
  
int main(int argc, char**argv)
{
	if (argc != 2)
	{
		printf("Usage: %s <file>\n", argv[0]);
		return -1;
	}
	int fd = open(argv[1],   O_RDWR | O_CREAT, 
				  S_IRUSR | S_IWUSR | S_IXUSR |   // 用户：rwx
				  S_IRGRP | S_IXGRP |			  // 组：r-x
				  S_IROTH | S_IXOTH);			  // 其他：r-x
					// 等效于 0755（rwxr-xr-x）

	if (fd < 0)
	{
		printf("Can not open the %s\n", argv[1]);
		perror("open");
	}
	else
	{
		printf("fd: %d\n", fd);
		
	}

	printf("lseek to offset 3 from file head\n");
	lseek(fd, 3, SEEK_SET);

	write(fd, "123", 3);

	close(fd);
	return 0;
}
