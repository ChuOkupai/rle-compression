# rle-compression

Program to compress and decompress files using the run-length encoding algorithm

# How to use:
First, make sure that gcc is installed on your computer.

To compile and run the source code, use the following command:
```shell
make
```

# Example:
I want to compress the file named *foo.txt*.  
So, I just have to type:
```shell
./rle -c foo.txt foo.rle
```
In order to be extracted, I'll have to type:
```shell
./rle -e foo.rle foo.old
```

# Technical specifications
### Description
The format used during compression is as follows:
```
[128-255][byte]
# Format of a compressed sequence
# (first byte value - 126) give the compressed sequence length between 2 and 129
# The following byte contain the value

[0-127][byte 0][byte 1]...[byte n]
# Format of a uncompressed sequence
# (first byte value + 1) give the uncompressed sequence length
# The following n bytes contain the value
```

### Examples
```
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
