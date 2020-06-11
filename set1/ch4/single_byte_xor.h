#ifndef SINGLE_BYTE_XOR_H
#define SINGLE_BYTE_XOR_H

extern char alphabet[];
extern float freq_table[];

int hti(unsigned char hex);
char decode(char a, char b);
float compute_exp(int c, float n_obs);
float compute_obs(char c, char string[]);
float score_code(char newcode[], float score);
char break_cipher(char code[], char plaintext[]);

#endif
