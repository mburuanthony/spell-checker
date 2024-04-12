A C program that checks for possible errors in lines of text, prints out the line and column number of the mispelled text and possible corrections

#### Compile

# 1. Compile all C source files with GCC

```console
gcc -o spell_checker hashset.c spellcheck.c spell_checker.c -lm
```

# 2. Or use make

```console
make
```

# 3. Recompile with make

```console
make clean
make
```

#### Run

```console
./spell_checker -d words.txt -r -s -e < input.txt
```

words.txt -> Dictionary file
input.txt -> Text to check for corections
