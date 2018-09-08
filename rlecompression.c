#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

int rlecompress(const char *inputfile, const char *outputfile)
{
	FILE *inputf = 0, *outputf = 0;
	uint8_t n;
	char neof, i, j;
	
	inputf = fopen(inputfile, "r");
	if (! inputf)
		return 1;
	outputf = fopen(outputfile, "wb");
	if (! outputf)
	{
		fclose(inputf);
		return 1;
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
	if (fclose(inputf) || fclose(outputf))
		return 1;
	return 0;
}

int rleextract(const char *inputfile, const char *outputfile)
{
	FILE *inputf = 0, *outputf = 0;
	uint8_t n;
	char i, j;
	
	inputf = fopen(inputfile, "rb");
	if (! inputf)
		return 1;
	outputf = fopen(outputfile, "w");
	if (! outputf)
	{
		fclose(inputf);
		return 1;
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
	if (fclose(inputf) || fclose(outputf))
		return 1;
	return 0;
}

int rletest(char *input)
{
	if (rlecompress(input, "test.rle"))
		return 1;
	return rleextract("test.rle", "test");
}

int main(int argc, char **argv)
{
	if (argc > 2)
	{
		if (! (strcmp(argv[1], "-test") && strcmp(argv[1], "-t")))
			return rletest(argv[2]);
		else if (argc > 3)
		{
			if (! (strcmp(argv[1], "-compress") && strcmp(argv[1], "-c")))
				return rlecompress(argv[2], argv[3]);
			else if (! (strcmp(argv[1], "-extract") && strcmp(argv[1], "-e")))
				return rleextract(argv[2], argv[3]);
		}
	}
	return 2;
}
