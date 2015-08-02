#include "string.h"

void my_bzero (void *s, int len)
{
	char *p = s ;

	while (len > 0) {
		*p ++ = 0 ;
		len -- ;
	}
}

int my_strcmp (const char *sc, const char *ct)
{
	while ((*sc != '\0') && (*ct != '\0') && (*sc == *ct)) {
			sc ++ ;
			ct ++ ;
	}

	return *sc - *ct ;
}

void memcpy (void *dst, void *src, int len)
{
	unsigned char *ps = src ;
	unsigned char *pd = dst ;
	while (len > 0) {
		*pd ++ = *ps ++ ;
		len -- ;
	}
}
