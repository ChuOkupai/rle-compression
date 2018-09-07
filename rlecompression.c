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
	if (j == EOF)
	{
		printf("0%c", i);
	}
	while (1)
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
			if (j == EOF)
			{
				n++;
				fseek(inputf, -n, SEEK_CUR);
				printf("%d", n - 1);
				for (i = 0; i < n; i++)
				{
					j = fgetc(inputf);
					printf("%c", j);
				}
				break;
			}
			else
			{
				fseek(inputf, -n -2, SEEK_CUR);
				printf("%d", n - 1);
				for (i = 0; i < n; i++)
				{
					j = fgetc(inputf);
					printc(j);
				}
				j = fgetc(inputf);
			}
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
