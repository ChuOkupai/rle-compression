#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

int rlecompress(const char *inputfile, const char *outputfile)
{
	FILE *inputf = 0, *outputf = 0;
	uint8_t n;
	int neof, i, j;
	
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
			printf("%d%c", n, i);
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
			printf("%d", n - 1);
			for (i = 0; i < n; i++)
			{
				j = fgetc(inputf);
				printf("%c", j);
			}
			if (neof)
				j = fgetc(inputf);
			else
				break;
		}
		i = j;
		j = fgetc(inputf);
	}
	printf("\n");
	if (fclose(inputf) || fclose(outputf))
		return 1;
	return 0;
}

int main(int argc, char **argv)
{
	if (argc > 1)
		return (rlecompress(argv[1], "rlefile")) ? 1 : 0;
	else
		return 2;
}
