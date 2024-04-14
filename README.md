C program that checks for possible errors in text, prints out the line and column number of the mispelled word(s) and possible corrections

#### Compile

#### 1. Compile all C source files with GCC

```console
gcc -o spell_checker hashset.c spellcheck.c spell_checker.c -lm
```

#### 2. Or use make

```console
make
```

#### 3. Recompile with make

```console
make clean
make
```

#### Usage

```console
# 1. Use without defining dictionary file & options (replace, swap, erase)

./spell_checker

# 2. Use with dictionary file & options

./spell_checker -d words.txt -r -s -e
```


#### Flags
```console
-d (*) : dictionary file / path (words.txt)

-r (Optional) : suggest possible corrections by replacing a letter(s)
              : mistape -> mistake (replace "p" with "k")

-s (Optional) : suggest possible corrections by swapping a letter(s)
              : pinaepple -> pineapple (swap "a", "e")

-e (Optional) : suggest possible corrections by erasing a letter(s)
              : earthqueake -> earthquake (erase "e")
```