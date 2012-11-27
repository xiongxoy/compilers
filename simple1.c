#include <stdlib.h>
int main() {
    int a = 10 ;
    int b = 20 ;
    int c[10] = {10,11,12};
    c[1] = 3;
    a = 4;
    b = 5;
    for ( a = 0; a < 5; a++) {
       a = a; 
    }
    a = a * 13;
    a = a * 1;
    a = a - 1;
    scanf("%d", &a);
    printf("%d\n", a);
}
