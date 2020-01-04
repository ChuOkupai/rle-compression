#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

int rle_compress(const char *src, const char *dst)
{
	FILE 	*in, *out;
	uint8_t	t[128], n, max;
	int		i, j, keep;
	
	if (!(in = fopen(src, "rb")))
		return (-1);
	if (!(out = fopen(dst, "wb")))
	{
		fclose(in);
		return (-1);
	}

	keep = 0;
	j = fgetc(in);
	while ((i = j) != EOF)
	{
		t[0] = (uint8_t)i;
		if (keep)
			keep = 0;
		else
			j = fgetc(in);
		n = i == j ? 127 : 0;
		while (i == j && ++n < 255)
			j = fgetc(in);
		if (!n)
			while (i != j && j != EOF && n < 127)
			{
				t[++n] = (uint8_t)j;
				i = j;
				j = fgetc(in);
				if (i == j)
					keep = n--;
			}
		max = n < 128 ? n + 1 : 1;
		if (fwrite(&n, sizeof(n), 1, out) < 1 ||
			fwrite(t, sizeof(t[0]), max, out) < max)
			break ;
	}
	i = i != EOF || ferror(in) || ferror(out);

	fclose(in);
	fclose(out);
	return (-i);
}

int rle_extract(const char *src, const char *dst)
{
	FILE 	*in, *out;
	uint8_t	n;
	int		i, j, max;
	
	if (!(in = fopen(src, "rb")))
		return (-1);
	if (!(out = fopen(dst, "wb")))
	{
		fclose(in);
		return (-1);
	}

	max = -1;
	while (max < 0 && (i = fgetc(in)) != EOF && (j = fgetc(in)) != EOF)
	{
		max = i + (i < 128 ? 1 : -126);
		while (max--)
		{
			n = (uint8_t)j;
			if (fwrite(&n, sizeof(n), 1, out) < 1 ||
				(i < 128 && max && (j = fgetc(in)) == EOF))
				break ; // write error
		}
	}
	if (max >= 0 || j == EOF)
		errno = EBADMSG;
	i = errno || i != EOF || ferror(in) || ferror(out);

	fclose(in);
	fclose(out);
	return (-i);
}
