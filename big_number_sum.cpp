#include <iostream>

void string_reverse(char* str, int len)
{
    for (int i = 0; i <= (len / 2); i++)
    {
        char temp    = str[i];
        str[i]       = str[len-i];
        str[len-i]   = temp;
    }
}

/* Description : Calculate the sum of two integers which given as two strings
 * Input       : a & b -- input integer as string
 * Output      : ppsum -- the result of a plus b
 * Note        : since ppsum is allocated inside, so you need to free it
 */
int big_number_sum(char* a, char* b, char** ppsum)
{
    if (NULL == a || NULL == b)
    {
        printf("Invalid param: a=%p, b=%p.\n", a, b);
        return -1;
    }

    int a_len = strlen(a);
    int b_len = strlen(b);
    // alloc 2 more bytes for carry and '\0'
    int sum_len = (a_len > b_len) ? a_len : b_len + 2;
    char* sum   = (char*)malloc(sum_len);
    if (NULL == sum)
    {
        printf("alloc memory failed.\n");
        return -1;
    }
	memset(sum, 0, sum_len);
	
    int carry = 0;
    for (int i = 0; i < sum_len-1; i++)
    {
        int left  = (i < a_len) ? a[a_len-1-i] - '0' : 0;
        int right = (i < b_len) ? b[b_len-1-i] - '0' : 0;

        sum[i] = left + right + carry;
        carry  = sum[i] / 10;
        sum[i] = sum[i] % 10 + '0';
    }
	
	if ('0' == sum[sum_len-2])
	{
		// if carry is '0', replace it '0' with '\0'
		sum[sum_len-2] = '\0';
		string_reverse(sum, sum_len-3);
	}
	else
	{
		string_reverse(sum, sum_len-2);
	}

    *ppsum = sum;
    return 0;
}

int main(void)
{
    char* a = "22345678";
    char* b = "88654321";
    char* sum = NULL;

    big_number_sum(a, b, &sum);
    printf("%s + %s = %s\n", a, b, sum);
    free(sum);
    return 0;
}