#include <sys/types.h>      // 提供 size_t、ssize_t 等类型定义
#include <sys/stat.h>       // 提供 open() 的文件状态标志（O_RDONLY 等）和模式宏（S_IRUSR 等）
#include <fcntl.h>          // 提供 open()、close() 等文件控制函数
#include <stdio.h>          // 提供 printf()、fprintf()、perror() 等标准 I/O 函数
#include <unistd.h>         // 提供 read()、close()、sleep() 等 POSIX 系统调用
#include <stdlib.h>         // 提供 atoi()、strtol()、EXIT_FAILURE、EXIT_SUCCESS 等工具函数和宏
#include <errno.h>          // 提供 errno 变量和错误码宏（如 ERANGE）
#include <string.h>

/***************************
  * ./process_excel data.csv result.csv
  * argc    = 3
  * argv[0] = "./process_excel"
  * argv[1] = "data.csv"
  * argv[2] = "result.csv"
****************************/
#define MAX_BUFFER_SIZE 100

/* 读取一行 */
int read_row(int fd_data, char* data_buf)
{
	static char internal_buf[MAX_BUFFER_SIZE];  // 静态缓冲区减少 read 调用次数
    static size_t buf_pos = 0;           // 当前缓冲区位置
    static ssize_t buf_len = 0;          // 缓冲区有效数据长度
    size_t i = 0;

    while (1) {
        // 如果内部缓冲区已空，重新填充
        if (buf_pos >= buf_len) {
            buf_len = read(fd_data, internal_buf, sizeof(internal_buf));
            if (buf_len == -1) {
                perror("read failed");
                return -1;  // 读取失败
            } else if (buf_len == 0) {
                return 0;    // 文件结束
            }
            buf_pos = 0;
        }

        // 从内部缓冲区复制字符到用户缓冲区
        char c = internal_buf[buf_pos++];
        if (i >= MAX_BUFFER_SIZE - 1) {  // 预留空间给 '\0'
            fprintf(stderr, "Line too long: buffer overflow\n");
            return -1;
        }

        data_buf[i++] = c;

        // 检测行结束符（支持 \n 或 \r\n）
        if (c == '\n') {
            data_buf[i-1] = '\0';
            return 1;  // 成功读取一行
        }
    }
}

/* 处理一行 */
void process_data(char* data_buf, char* result_buf) {
    // 检查是否为标题行（首字符是逗号）
    if (data_buf[0] == ',') {
        strcpy(result_buf, data_buf);
        return;
    }

    char *token = strtok(data_buf, ",");
    strcpy(result_buf, token);  // 写入姓名

    int sum = 0;
    while ((token = strtok(NULL, ",")) != NULL) {
        int score = atoi(token);
        sum += score;
        if(score != 0) sprintf(result_buf + strlen(result_buf), ",%d", score);
    }

    // 追加总分和评价
    char grade = (sum >= 270) ? 'A' : (sum >= 240) ? 'B' : 'C';

    sprintf(result_buf + strlen(result_buf), ",%d,%c", sum, grade);
}


/* 写入一行 */
void write_data(int fd_result, char* result_buf)
{
	int len = write(fd_result, result_buf, strlen(result_buf));
	if (len != strlen(result_buf))
	{
		perror("write");
	}
	write(fd_result, "\r\n", 2);
}

int main(int argc, char**argv)
{
	int len;
	char* data_buf = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE);
	char* result_buf = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE * 2);

	if (argc != 3)
	{
		printf("Usage: %s <data.csv> <result.csv>\n", argv[0]);
		return -1;
	}
	
	int fd_data = open(argv[1], O_RDONLY);
	int fd_result = open(argv[2], O_RDWR | O_CREAT | O_TRUNC,0644 );
					
	if (fd_data < 0)
	{
		printf("Can not open the %s\n", argv[1]);
		perror("open");
		return -1;
	} 
	else if (fd_result < 0)
	{
		printf("Can not open the %s\n", argv[2]);
		perror("creat");
		return -1;
	}
	else
	{
		printf("fd_data: %d\n", fd_data);
		printf("fd_result: %d\n", fd_result);
	}

	while (1)
	{
		/* 读取一行 */
		len = read_row(fd_data, data_buf);
		if (len <= 0) break;
		
		/* 处理一行 */
		process_data(data_buf, result_buf);
		
		/* 写入一行 */
		write_data(fd_result, result_buf);
	}

	close(fd_data);
	close(fd_result);
	return 0;
}
