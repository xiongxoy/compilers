#include <stdlib.h>
int main() {
    int a = 10 ;
    int b = 20 ;
    int c[5] = {10,11,12,13,14};
    int d[5] = {20,21,22};
    c[1] = 1;
    c[2] = 2;
    d[1] = 3;
    d[2] = 4;
    printf("c[1] + d[1] is %d\n", c[1] + d[2]);
    for ( a = 0; a < 5; a++) {
       a = a * c[2]; 
       printf("a is %d\n", a);
    }
    a = a * 13;
    a = a * 1;
    a = a - 1;
    printf("a is %d\n", a);
    scanf("%d", &a);
    printf("%d\n", a);
}
