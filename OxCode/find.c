/**
 * �����е����������������������Ѿ�ָ���������޸ģ�ֱ�ӷ��ط����涨��ֵ����
 *
 * 
 * @param numbers int����һά���� 
 * @param numbersLen int numbers���鳤��
 * @return int����
 */
int find(int* numbers, int numbersLen) {
    int *count = (int*)calloc(201, sizeof(int)); // ��ʼ��Ϊ0
    if (!count) return -1; // ����ʧ��

    // ͳ��ÿ�����ֵĳ��ִ���
    for (int i = 0; i < numbersLen; i++) {
        count[numbers[i]]++;
    }

    int total = 0;
    int i = 0;
    while (total < 451 && i <= 200) {
        total += count[i++];
    }

    free(count); // �ͷ��ڴ�
    return i - 1; // �������ֱ���
}