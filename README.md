# rle-compression

Program to compress and extract files using the run-length encoding algorithm.

**Important note :**

This implementation is a modified version of the algorithm to allow a better compression ratio.

Refer to the [Technical specifications](#technical-specifications) section for more information.

# Documentation
### Compile from source
Use the following command:
```shell
make
```
or
```shell
make rle
```

### Usage
```shell
./rle [OPTION] SOURCE DEST
```
**Options list:**
- **-c**
compress SOURCE to DEST
- **-e**
extract SOURCE to DEST
- **--help**
display help and exit
- **--version**
output version information and exit

### Example
To compress the file named *foobar.txt*, the command will be:
```shell
./rle -c foobar.txt foobar.rle
```
Extract a rle file:
```shell
./rle -e foobar.rle foobar.old
```

# Technical specifications
### Description
The format used during compression is as follows:
```shell
[128-255][byte]
# Format of a compressed sequence
# (first byte value - 126) give the compressed sequence length between 2 and 129
# The following byte contain the value

[0-127][byte 0][byte 1]...[byte n]
# Format of a uncompressed sequence
# (first byte value + 1) give the uncompressed sequence length between 1 and 128
# The following n bytes contain the value
```

### Examples
```shell
# Exemple 1: "AAAA" which can be reduced to "4A"
[130][A]
# length = 130 - 126 = 4
# byte value = A

# Exemple 2: "ABCDE" which cannot be reduced
[4][A][B][C][D][E]
# length = 4 + 1 = 5
# bytes value = [A, B, C, D, E]

# Exemple 3: "foobar"
[0][f][128][o][2][b][a][r]
# or [1f 2o 1b 1a 1r]
```

# License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
