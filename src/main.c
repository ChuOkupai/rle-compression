#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include "rle.h"

static int	print_usage(char *bin)
{
	printf("Usage: %s [OPTION] SOURCE DEST\n", basename(bin));
	return (0);
}

static int	print_help(char *bin)
{
	print_usage(bin);
	printf("Compress and extracts files using the RLE compression algorithm.\n\n");
	printf("  -c             compress SOURCE to DEST\n");
	printf("  -e             extract SOURCE to DEST\n");
	printf("      --help     display this help and exit\n");
	printf("      --version  output version information and exit\n");
	return (0);
}

static int	print_version(char *bin)
{
	printf("%s 1.0\n", basename(bin));
	printf("Copyright (c) 2020 Adrien Soursou\n");
	printf("License: MIT\n");
	printf("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n");
	printf("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n");
	printf("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n");
	printf("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n");
	printf("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n");
	printf("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n");
	printf("SOFTWARE.\n");
	printf("\nWritten by Adrien Soursou.\n");
	return (0);
}

static int	print_error(char *bin, const char *message)
{
	bin = basename(bin);
	printf("%s: %s\n", bin, message);
	printf("Try '%s --help' for more information.\n", bin);
	return (0);
}

int			main(int ac, char **av)
{
	for (int i = 1; i < ac; i++)
		if (!strcmp(av[i], "--help"))
			return (print_help(*av));
		else if (!strcmp(av[i], "--version"))
			return (print_version(*av));
	if (ac != 4)
		return (print_usage(*av));
	if (!strcmp(av[1], "-c"))
		rle_compress(av[2], av[3]);
	else if (!strcmp(av[1], "-e"))
		rle_extract(av[2], av[3]);
	else
		return (print_error(*av, "invalid option"));
	if (errno)
		print_error(*av, strerror(errno));
	return (errno);
}
