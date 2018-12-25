# rle-compression

Program to compress and decompress files using the run-length encoding algorithm (cf. [Wikipedia](https://en.wikipedia.org/wiki/Run-length_encoding)).

# How to use:
First, make sure that gcc is installed on your computer.  
Then, dowload the lastest version from the [releases page](https://github.com/ChuOkupai/rle-compression/releases).  
To compile and run the source code, use the following command:
```bash
make run
```
That's it!

# Example:
I want to compress the file named *foo*.  
So, I just have to type:
```bash
./rle.out -c foo foo.rle
```
In order to be extracted, I'll have to type:
```bash
./rle.out -e foo.rle foo.bak
```
