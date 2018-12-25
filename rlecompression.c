/* 
 * Copyright (c) 2018 Adrien Soursou
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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

int rleCompress(const char *source, const char *destination)
{
	FILE *input = 0, *output = 0;
	uint8_t	n;
	char	neof, i, j;
	
	input = fopen(source, "r");
	if (! input)
		return ERR_LOAD;
	output = fopen(destination, "wb");
	if (! output)
	{
		fclose(input);
		return ERR_WRITE;
	}
	i = fgetc(input);
	j = fgetc(input);
	while (i != EOF)
	{
		n = 0;
		if (i == j)
		{
			while (i == j && n < 127)
			{
				j = fgetc(input);
				n++;
			}
			n += 128;
			fwrite(&n, 1, sizeof(n), output);
			fwrite(&i, 1, sizeof(i), output);
			if (j == EOF)
				break;
		}
		else
		{
			while (j != EOF && i != j && n < 127)
			{
				i = j;
				j = fgetc(input);
				n++;
			}
			neof = (j == EOF) ? 0 : 2;
			if (! neof)
				n++;
			fseek(input, -n - neof, SEEK_CUR);
			n--;
			fwrite(&n, 1, sizeof(n), output);
			n++;
			for (i = 0; i < n; i++)
			{
				j = fgetc(input);
				fwrite(&j, 1, sizeof(j), output);
			}
			if (neof)
				j = fgetc(input);
			else
				break;
		}
		i = j;
		j = fgetc(input);
	}
	fclose(input);
	fclose(output);
	return 0;
}

int rleExtract(const char *source, const char *destination)
{
	FILE *input = 0, *output = 0;
	uint8_t	n;
	char	i, j, repeat;
	
	input = fopen(source, "rb");
	if (! input)
		return ERR_LOAD;
	output = fopen(destination, "w");
	if (! output)
	{
		fclose(input);
		return ERR_WRITE;
	}
	n = 0;
	i = 0;
	while (i != EOF)
	{
		if (fread(&n, sizeof(n), 1, input) != 1)
			break;
		repeat = (n < 127) ? 0 : 1;
		if (repeat)
			n -= 128;
		for (j = 0; j <= n; j++)
		{
			if (! (repeat && j))
			{
				i = fgetc(input);
				if (i == EOF)
					break;
			}
			fwrite(&i, 1, sizeof(i), output);
		}
	}
	fclose(input);
	fclose(output);
	return 0;
}

int _missing(char *bin)
{
	printf("%s: Missing file operand\n", bin);
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
	int i, j, source;

	for (i = 1; i < argc; i++)
		if (! strcmp(argv[i], "--help"))
			return _help(argv[0]);
	arg = ARG_EXTRACT;
	j = 0;
	for (i = 1; i < argc; i++)
	{
		err = 0;
		source = 0;
		if (argv[i][0] ==  '-')
			_argument(argv[i]);
		else
		{
			source = 1;
			if (i + 1 == argc)
				err = ERR_DST;
			else if (arg == ARG_COMPRESS)
				err = rleCompress(argv[i], argv[i + 1]);
			else // arg == ARG_EXTRACT
				err = rleExtract(argv[i], argv[i + 1]);
		}
		if (err)
			_error(argv[0], argv[i]);
		if (source)
			i++;
	}
	if (! source)
		_missing(argv[0]);
	return 0;
}
