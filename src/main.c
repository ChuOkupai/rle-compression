#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include "rle.h"

int		print_usage(char *bin)
{
	printf("usage: %s [-c | -e] source target...\n", basename(bin));
	return (0);
}

int		main(int ac, char **av)
{
	int err;

	for (int i = 1; i < ac; i++)
		if (!strcmp(av[i], "--help"))
			return (print_usage(av[0]));
	if (ac != 4)
		return (print_usage(av[0]));
	if (!strcmp(av[1], "-c"))
		err = rle_compress(av[2], av[3]);
	else if (!strcmp(av[1], "-e"))
		err = rle_extract(av[2], av[3]);
	else
		return (print_usage(av[0]));
	if (err)
		fprintf(stderr, "error: %s\n", strerror(errno));
	return (errno);
}
