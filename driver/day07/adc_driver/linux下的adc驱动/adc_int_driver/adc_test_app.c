#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main (void)
{
	unsigned int adv = 0;
	int fd = open ("/dev/adc", O_RDWR);
	if (fd < 0) {
		perror ("adc error\n");
		exit (0);
	}

	while (1) {
		sleep (1) ;
		read (fd, &(adv), sizeof (adv));
		printf ("user app adv = %f\n", 3.3 * adv / 4096);
	}

	return 0;
}
