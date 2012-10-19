#include <stdio.h>
#include <string.h>
#include <assert.h>
#define MAXWORD 1000

struct dbuffer
{
	char* buffer[2048];          //缓冲区空间
	char* beginning,forward;     //前向和后向指针
	int count;                   //前向指针记数
} bbuf;

void parse(char c)
{
	if(c=' ')
	{
		memcpy(word[i],beginning,(size_t)(forward-beginning));
		i++;
	}
	else forward++;
}

int main(int argc,char* argv)
{
	File* fp;
	char* word[MAXWORD];
	int i=0;
	char  buffer[2048];
	fp=open("test.c","r");
	assert(fp);
        char c;
	
	getc(c); 		
	while( (c=getc(fp)) != EOF )
	{
		
	}
}
