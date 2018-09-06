#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

// len_max = 128
// Si compressible : len + 127
// Sinon incompressible : len
// fprintf(f, );

int rlecompress(const char *inputfile, const char *outputfile)
{
	FILE *inputf = 0, *outputf = 0;
	uint8_t c, len;
	int compressible, i, j;
	
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
	while (i != EOF)
	{
		len = 0;
		j = fgetc(inputf);
		compressible = (i == j) ? 1 : 0;
		if (compressible)
		{
			while (j && len < 128)
			{
				if (i != j)
				{
					len += 127;
					printf("c=%hd ", len);
					fwrite(&len, 1, sizeof(uint8_t), outputf);
					c = i;
					printf("v=%hd\n", c);
					fwrite(&c, 1, sizeof(uint8_t), outputf);
					i = j;
					j = 0;
				}
				else
					j = fgetc(inputf);
				len++;
			}
		}
		else
		{
			printf("j=%hd ", j);
			while (j != EOF && len < 128)
			{
				if (i == j)
				{
					printf("SEEK_CUR=%ld\n", ftell(inputf));
					/*fseek(inputf, len, SEEK_SET);
					fwrite(&len, 1, sizeof(uint8_t), outputf);
					c = i;
					printf("v=%hd\n", c);
					fwrite(&c, 1, sizeof(uint8_t), outputf);
					i = j;
					j = EOF;*/
				}
				else
				{
					i = j;
					j = fgetc(inputf);
					printf("j=%hd ", j);
				}
				len++;
			}
		}
		i = fgetc(inputf);
	}
	printf("\n");
	if (fclose(inputf) || fclose(outputf))
		return 1;
	return 0;
}

int uint8convert(const char *inputfile, const char *outputfile)
{
	FILE *inputf = 0, *outputf = 0;
	uint8_t c;
	int i;
	
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
	while (i != EOF)
	{
		c = i;
		printf("%hd ", c);
		fwrite(&c, 1, sizeof(uint8_t), outputf);
		i = fgetc(inputf);
	}
	printf("\n");
	if (fclose(inputf) || fclose(outputf))
		return 1;
	return 0;
}

int main(void)
{
	return (rlecompress("a", "a.rle")) ? 1 : 0;
}
