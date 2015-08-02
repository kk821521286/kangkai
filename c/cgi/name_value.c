#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <strings.h> 
int htoi(char *); 
main() 
{ 
	int i,n; 
	char c; 
	printf ("Contenttype: text/plainnn"); 
	n=0; 
	if (getenv("CONTENT-LENGTH")) 
		n=atoi(getenv("CONTENT-LENGTH")); 
	for (i=0; i < n;i++){ 
		int is_eq=0; 
		c=getchar(); 
		switch (c){ 
			case '&': 
				c='n'; 
				break; 
			case '+': 
				c=' '; 
				break; 
			case '%':{ 
						 char s[3]; 
						 s[0]=getchar(); 
						 s[1]=getchar(); 
						 s[2]=0; 
						 c=htoi(s); 
						 i+=2; 
					 } 
					 break; 
			case '=': 
					 c=':'; 
					 is_eq=1; 
					 break; 
		}; 
		putchar(c); 
		if (is_eq) putchar(' '); 
	} 
	putchar ('n'); 
	printf("\n");
	fflush(stdout); 
} 
/* convert hex string to int */ 
int htoi(char *s) 
{ 
	char *digits="0123456789ABCDEF"; 
	if (islower (s[0])) s[0]=toupper(s[0]); 
	if (islower (s[1])) s[1]=toupper(s[1]); 
	return 16 * (strchr(digits, s[0]) -strchr (digits,'0') ) 
		+(strchr(digits,s[1])-strchr(digits,'0')); 
} 
