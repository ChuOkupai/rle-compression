#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#define ERR_ARG		-1
#define ERR_DST		-2
#define ERR_LOAD	-4
#define ERR_WRITE	-8
int err;

#define ARG_COMPRESS	1
#define ARG_EXTRACT		2
int	arg;

int rlecompress(const char *inputfile, const char *outputfile)
{
	FILE *inputf = 0, *outputf = 0;
	uint8_t n;
	char neof, i, j;
	
	inputf = fopen(inputfile, "r");
	if (! inputf)
		return ERR_LOAD;
	outputf = fopen(outputfile, "wb");
	if (! outputf)
	{
		fclose(inputf);
		return ERR_WRITE;
	}
	i = fgetc(inputf);
	j = fgetc(inputf);
	while (i != EOF)
	{
		n = 0;
		if (i == j)
		{
			while (i == j && n < 127)
			{
				j = fgetc(inputf);
				n++;
			}
			n += 128;
			fwrite(&n, 1, sizeof(n), outputf);
			fwrite(&i, 1, sizeof(i), outputf);
			if (j == EOF)
				break;
		}
		else
		{
			while (j != EOF && i != j && n < 127)
			{
				i = j;
				j = fgetc(inputf);
				n++;
			}
			neof = (j == EOF) ? 0 : 2;
			if (! neof)
				n++;
			fseek(inputf, -n - neof, SEEK_CUR);
			n--;
			fwrite(&n, 1, sizeof(n), outputf);
			n++;
			for (i = 0; i < n; i++)
			{
				j = fgetc(inputf);
				fwrite(&j, 1, sizeof(j), outputf);
			}
			if (neof)
				j = fgetc(inputf);
			else
				break;
		}
		i = j;
		j = fgetc(inputf);
	}
	fclose(inputf);
	fclose(outputf);
	return 0;
}

int rleextract(const char *inputfile, const char *outputfile)
{
	FILE *inputf = 0, *outputf = 0;
	uint8_t n;
	char i, j;
	
	inputf = fopen(inputfile, "rb");
	if (! inputf)
		return ERR_LOAD;
	outputf = fopen(outputfile, "w");
	if (! outputf)
	{
		fclose(inputf);
		return ERR_WRITE;
	}
	n = 0;
	i = 0;
	while (i != EOF)
	{
		if (fread(&n, sizeof(n), 1, inputf) != 1)
			break;
		i = fgetc(inputf);
		if (i == EOF)
			break;
		if (n < 127)
		{
			fwrite(&i, 1, sizeof(i), outputf);
			for (j = 0; j < n; j++)
			{
				i = fgetc(inputf);
				if (i == EOF)
					break;
				fwrite(&i, 1, sizeof(i), outputf);
			}
		}
		else
		{
			n -= 127;
			for (j = 0; j < n; j++)
				fwrite(&i, 1, sizeof(i), outputf);
		}
	}
	fclose(inputf);
	fclose(outputf);
	return 0;
}

int _missing(char *bin)
{
	printf("%s: missing file operand\n", bin);
	printf("Try '%s --help' for more information\n", bin);
	return 0;
}

int _help(char *bin)
{
	printf("usage: %s [-c | -e] source target...\n", bin);
	printf("Compress or extract source to target\n");
	return 0;
}

void	_error(char *bin, char *argument)
{
	if (! err)
		return;
	printf("%s: %s: ", bin, argument);
	if (err == ERR_ARG)
		puts("Illegal option");
	else if (err == ERR_DST)
		puts("Missing target file");
	else if (err == ERR_LOAD)
		puts("No such file or directory");
	else // err == ERR_WRITE
		puts("Could not write file");
}

void	_argument(char *s)
{
	while (! err && *++s)
		if (*s == 'c')
			arg = ARG_COMPRESS;
		else if (*s == 'e')
			arg = ARG_EXTRACT;
		else
			err = ERR_ARG;
}

int main(int argc, char **argv)
{
	if (argc < 2)
		return _missing(argv[0]);
	int i, j;

	for (i = 1; i < argc; i++)
		if (! strcmp(argv[i], "--help"))
			return _help(argv[0]);
	arg = ARG_EXTRACT;
	j = 0;
	for (i = 1; i < argc; i++)
	{
		err = 0;
		if (argv[i][0] ==  '-')
			_argument(argv[i]);
		else
		{
			if (i + 1 == argc)
				err = ERR_DST;
			else
			{
				if (arg == ARG_COMPRESS)
					err = rlecompress(argv[i], argv[i + 1]);
				else // arg == ARG_EXTRACT
					err = rleextract(argv[i], argv[i + 1]);
				if (err)
				{
					_error(argv[0], argv[i]);
					err = 0;
				}
				i++;
			}
		}
		if (err)
			_error(argv[0], argv[i]);
	}
	return 0;
}
