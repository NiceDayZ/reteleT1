#include <stdio.h>
#define MAX_LEN 128
 
 
int main(void)
{
    char s[300];

    printImage();
    print("Welcome to BearFiles. Please authenticate: ");
    fgets(s, sizeof(s), stdin);
    return 0;
}
 
void printImage()
{
    char *filename = "bear.txt";
    FILE *fptr = NULL;
 
    if((fptr = fopen(filename,"r")) == NULL)
    {
        fprintf(stderr,"error opening %s\n",filename);
        return 1;
    }

    char read_string[MAX_LEN];
 
    while(fgets(read_string,sizeof(read_string),fptr) != NULL)
        printf("%s",read_string);

     fclose(fptr);
}