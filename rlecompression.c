#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

// len_max = 128
// Si compressible : len + 127
// Sinon incompressible : len
// fprintf(f, );

void printc(char c)
{
	if (c == '\n')
		printf("\\n");
	else if (c == EOF)
		printf("'EOF'");
	else
		printf("%c", c);
}

int rlecompress(const char *inputfile, const char *outputfile)
{
	FILE *inputf = 0, *outputf = 0;
	uint8_t n;
	int i, j;
	
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
	printc(i);printc(j);
	if (j == EOF)
	{
		printf("[%d%c\\n]", 0, i);
	}
	while (j != EOF)
	{
		n = 0;
		if (i == j)
		{
			while (i == j && n < 127)
			{
				j = fgetc(inputf);printc(j);
				n++;
			}
			n += 128;
			printf("[%d%c]", n, i);
		}
		else
		{
			while (j != EOF && i != j && n < 127)
			{
				j = fgetc(inputf);printc(j);
				n++;
			}
			if (j == EOF)
			{
				fseek(inputf, -2, SEEK_CUR);
				i = fgetc(inputf);printc(i);
				printf("[%d%c\\n]", n, i);
			}
			else
				printf("[%d%c]", n, i);
		}
		if (j != EOF)
		{
			i = j;printc(j);
			j = fgetc(inputf);
			printc(j);
		}
	}
	if (j == EOF)
		printf("'j EOF'");
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
