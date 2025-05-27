/**
 * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
 *
 * 
 * @param numbers int整型一维数组 
 * @param numbersLen int numbers数组长度
 * @return int整型
 */
int find(int* numbers, int numbersLen) {
    int *count = (int*)calloc(201, sizeof(int)); // 初始化为0
    if (!count) return -1; // 分配失败

    // 统计每个数字的出现次数
    for (int i = 0; i < numbersLen; i++) {
        count[numbers[i]]++;
    }

    int total = 0;
    int i = 0;
    while (total < 451 && i <= 200) {
        total += count[i++];
    }

    free(count); // 释放内存
    return i - 1; // 返回数字本身
}