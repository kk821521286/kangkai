#include <stdio.h> 
#include <stdlib.h> 
main() 
{ 
int i,n; 
printf ("Contenttype:text/plainnn"); 
 n = 0; 
if(getenv("CONTENT-LENGTH")) 
n=atoi(getenv("CONTENT-LENGTH")); 
for (i=0;i < n -1; i++)
putchar(getchar());
putchar ('n'); 
printf("\n");
fflush(stdout); 
} 
