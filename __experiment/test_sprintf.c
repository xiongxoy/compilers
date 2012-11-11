#include <stdio.h>
#include <assert.h>

int main(int argc, const char *argv[])
{
    char * s = malloc ( 100 ); 
    sprintf(s, "%%esp\n" );
    printf("%s", s);
    return 0;
}
