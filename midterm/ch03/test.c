#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // char * 배열 (문자열 포인터 배열)
    char **arr = (char **)malloc(sizeof(char *) * 10);
    if (arr == NULL)
    {
        perror("Memory allocation failed");
        return 1;
    }

    free(arr);

    return 0;
}