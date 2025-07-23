#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h> // printf() perror()
#include <unistd.h> // close() sleep()
#include <string.h> // strlen()
// #include <errno.h>

/***************************
  * ./write 1.txt str1 str2
  * argc    >= 3
  * argv[0] = "./open"
  * argv[1] = "filename"
****************************/
  
int main(int argc, char**argv)
{
	if (argc < 3)
	{
		printf("Usage: %s <file> <string1> <string2>\n", argv[0]);
		return -1;
	}
	int fd = open(argv[1],   O_RDWR | O_CREAT | O_TRUNC, 
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

	for (int i = 2; i < argc; i++)
	{
		int len = write(fd, argv[i], strlen(argv[i]));
		if (len != strlen(argv[i]))
		{
			perror("write");
			break;
		}
		write(fd, "\r\n", 2);
	}

	close(fd);
	return 0;
}
