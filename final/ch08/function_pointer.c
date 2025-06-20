#include <stdio.h>

int f1(int a) { return a + 1; }

int main(){
    int (*f)(int a);
    f = f1;

    printf("%d\n", f(10));

    printf("%p - %p", f, f1);
}