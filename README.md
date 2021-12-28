# LLR
Generate Bi-grams and their Log Likelihood Ratios from a Corpus.

FILENAME: "LLR.c"

COMPILE & LINK:
gcc LLR.c -o LLR -lm

EXECUTE:
./LLR textfile.txt > file.output

  SAMPLE OUTPUT: 

  ngram: repeat of

  2          5
 
  1         90

  ngram: of tokens

  2           2

  6           89

  ngram: tokens i

  1           5

  3          90

. . . 

  repeat of 3.931

  of tokens 7.515

  tokens i 5.283

. . .
