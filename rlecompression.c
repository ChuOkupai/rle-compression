#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

int rlecompress(const char *inputfile, const char *outputfile)
{
	FILE *inputf = 0, *outputf = 0;
	signed char neof, i, j, n;
	
	inputf = fopen(inputfile, "r");
	if (! inputf)
		return 1;
	outputf = fopen(outputfile, "w");
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
			fwrite(&n, 1, sizeof(signed char), outputf);
			fwrite(&i, 1, sizeof(signed char), outputf);
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
			fwrite(&n, 1, sizeof(signed char), outputf);
			n++;
			for (i = 0; i < n; i++)
			{
				j = fgetc(inputf);
				fwrite(&j, 1, sizeof(signed char), outputf);
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
	signed char i, j;
	
	inputf = fopen(inputfile, "r");
	if (! inputf)
		return 1;
	outputf = fopen(outputfile, "w");
	if (! outputf)
	{
		fclose(inputf);
		return 1;
	}
	while (1)
	{
		fread(&i, sizeof(uint8_t), 1, inputf);
		printf("i=%d\n", i);break;
		/*if (i < 127)
		{
			n = i + 1;
			for (i = 0; i < n; i++)
			{
				j = fgetc(inputf);
				if (j == EOF)
					break;
				else
					fwrite(&j, 1, sizeof(char), outputf);
			}
		}
		else
		{
			n = i - 127;
			j = fgetc(inputf);
			if (j == EOF)
					break;
			for (i = 0; i < n; i++)
				fwrite(&j, 1, sizeof(char), outputf);
		}*/
	}
	if (fclose(inputf) || fclose(outputf))
		return 1;
	return 0;
}

int main(int argc, char **argv)
{
	if (argc > 3)
	{
		if (! (strcmp(argv[1], "-compress") && strcmp(argv[1], "-c"))){printf("compress\n");
			return (rlecompress(argv[2], argv[3])) ? 1 : 0;}
		else if (! (strcmp(argv[1], "-extract") && strcmp(argv[1], "-e")))
			return (rleextract(argv[2], argv[3])) ? 1 : 0;
		else
			return 2;
	}
	else
		return 2;
}
