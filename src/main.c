#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include "rle.h"

#define HELP_MSG \
"Compress and extracts files using the RLE compression algorithm.\n\n" \
"  -c             compress SOURCE to DEST\n" \
"  -x             extract SOURCE to DEST\n" \
"      --help     display this help and exit\n" \
"      --version  output version information and exit\n"

#define LICENSE_MSG \
"Copyright (c) 2020 Adrien Soursou\n" \
"License: MIT\nTHE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY " \
"KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES " \
"OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. " \
"IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM" \
", DAMAGES OR OTHER\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT " \
"OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR " \
"THE USE OR OTHER DEALINGS IN THE\nSOFTWARE.\n\nWritten by Adrien Soursou.\n"

static int	print_help(char *bin)
{
	printf("Usage: %s [OPTION] SOURCE DEST\n", bin);
	printf(HELP_MSG);
	return (0);
}

static int	print_version(char *bin)
{
	printf("%s 1.1\n", bin);
	printf(LICENSE_MSG);
	return (0);
}

static int	print_invalid(char *bin)
{
	fprintf(stderr, "%s: invalid option\n", bin);
	fprintf(stderr, "Try '%s --help' for more information.\n", bin);
	return (-1);
}

int			main(int ac, char **av)
{
	*av = basename(*av);
	for (int i = 1; i < ac; i++)
		if (!strcmp(av[i], "--help"))
			return print_help(*av);
		else if (!strcmp(av[i], "--version"))
			return print_version(*av);
	if (ac != 4)
		return print_invalid(*av);
	if (!strcmp(av[1], "-c"))
		rle_compress(av[2], av[3]);
	else if (!strcmp(av[1], "-x"))
		rle_extract(av[2], av[3]);
	else
		return print_invalid(*av);
	if (errno)
		fprintf(stderr, "%s: %s\n", *av, strerror(errno));
	return (-(errno != 0));
}
