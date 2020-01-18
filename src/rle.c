#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

int	rle_compress(const char *src, const char *dst)
{
	FILE	*in, *out;
	uint8_t	t[129];
	int		i, keep;

	// file verification
	if (!(in = fopen(src, "rb")))
		return (-1);
	if (!(out = fopen(dst, "wb")))
	{
		fclose(in);
		return (-1);
	}

	// algorithm
	t[0] = fgetc(in);
	while (!feof(in))
	{
		t[1] = fgetc(in);
		if (t[0] != t[1]) // uncompressible sequence
		{
			i = 1;
			if (!feof(in))
				do
					t[++i] = fgetc(in);
				while (!feof(in) && i < 128 && t[i] != t[i - 1]);
			if ((keep = t[i] == t[i - 1]))
				--i;
			if (fputc(i - 1, out) == EOF ||
				fwrite(t, sizeof(uint8_t), i, out) < (unsigned)i)
				break ; // write error
			t[0] = t[i];
			if (!keep)
				continue ; // size too large or EOF
		}
		// compressible sequence
		i = 2;
		do
			t[1] = fgetc(in);
		while (++i < 130 && t[0] == t[1]);
		if (fputc(i + 125, out) == EOF ||
			fputc(t[0], out) == EOF)
				break ; // write error
		t[0] = t[1];
	}

	// error handling
	i = ferror(in) || ferror(out);

	fclose(in);
	fclose(out);
	return (-i);
}

int	rle_extract(const char *src, const char *dst)
{
	FILE	*in, *out;
	int		i, j, max;

	// file verification
	if (!(in = fopen(src, "rb")))
		return (-1);
	if (!(out = fopen(dst, "wb")))
	{
		fclose(in);
		return (-1);
	}

	// algorithm
	j = 0;
	while ((i = fgetc(in)) != EOF && (j = fgetc(in)) != EOF)
	{
		max = i + (i < 128 ? 1 : -126);
		while (max--)
			if (fputc(j, out) == EOF ||
				(i < 128 && max && (j = fgetc(in)) == EOF))
				break ; // write error
	}

	// error handling
	if (j == EOF)
		errno = EBADMSG; // corrupted file
	i = errno || i != EOF || ferror(in) || ferror(out);

	fclose(in);
	fclose(out);
	return (-i);
}
