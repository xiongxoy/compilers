#include <stdio.h>

int main(int argc, const char *argv[])
{
    int a[2] = {1,2};
    int i=0;
    a[1] = 3;
    for(i=0; i<2; i++) {
        a[i] = 999;
    }
    return 0;
}
