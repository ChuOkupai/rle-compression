#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

int	fopen_error(FILE *f1, FILE *f2)
{
	if (f1)
		fclose(f1);
	if (f2)
		fclose(f2);
	return (-1);
}

int rle_compress(const char *source, const char *destination)
{
	FILE 	*input = 0, *output = 0;
	uint8_t	n;
	char	neof, i, j;
	
	input = fopen(source, "r");
	output = fopen(destination, "wb");
	if (! input || ! output)
		return (fopen_error(input, output));
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
	return (0);
}

int rle_extract(const char *source, const char *destination)
{
	FILE 	*input = 0, *output = 0;
	uint8_t	n;
	char	i, j, repeat;
	
	input = fopen(source, "rb");
	output = fopen(destination, "w");
	if (! input || ! output)
		return (fopen_error(input, output));
	n = 0;
	i = 0;
	while (i != EOF && fread(&n, sizeof(n), 1, input) == 1)
	{
		if ((repeat = n >= 127))
			n -= 128;
		for (j = 0; j <= n; j++)
			if (!(repeat && j) && (i = fgetc(input)) == EOF)
				break;
			else
				fwrite(&i, 1, sizeof(i), output);
	}
	fclose(input);
	fclose(output);
	return 0;
}
