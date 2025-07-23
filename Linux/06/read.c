#include <sys/types.h>      // 提供 size_t、ssize_t 等类型定义
#include <sys/stat.h>       // 提供 open() 的文件状态标志（O_RDONLY 等）和模式宏（S_IRUSR 等）
#include <fcntl.h>          // 提供 open()、close() 等文件控制函数
#include <stdio.h>          // 提供 printf()、fprintf()、perror() 等标准 I/O 函数
#include <unistd.h>         // 提供 read()、close()、sleep() 等 POSIX 系统调用
#include <stdlib.h>         // 提供 atoi()、strtol()、EXIT_FAILURE、EXIT_SUCCESS 等工具函数和宏
#include <errno.h>          // 提供 errno 变量和错误码宏（如 ERANGE）
#include <limits.h>         // 提供 INT_MAX 等整数类型极限值宏


int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file> <count>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // 1. 验证 count 参数
    char *endptr;
    long count = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || count <= 0 || count > INT_MAX) {
        fprintf(stderr, "Invalid count: must be a positive integer\n");
        return EXIT_FAILURE;
    }

    // 2. 打开文件
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open failed");
        return EXIT_FAILURE;
    }
    printf("fd: %d\n", fd);

    // 3. 读取文件（安全处理缓冲区）
    char buf[10];
    ssize_t size = read(fd, buf, (count < sizeof(buf)) ? count : sizeof(buf) - 1);
    if (size == -1) {
        perror("read failed");
        close(fd);
        return EXIT_FAILURE;
    } else if (size == 0) {
        printf("Reached end of file\n");
    } else {
        buf[size] = '\0';
        printf("Read %zd bytes: %s\n", size, buf);
    }

    // 4. 关闭文件
    close(fd);
    return EXIT_SUCCESS;
}

