#include "blur.h"
#include "imageprocessing.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100
#define N 3

void cut_last_word(char delimiter, char* s, char* out)
{
  int i, j, len;
  int del_idx = 0;

  i = 0;
  while (s[i] != '\0')
  {
    if (s[i] == delimiter)
      del_idx = i;
    i++;
  }
  len = i;

  j = 0;
  for (i = del_idx+1; i < len+1; i++)
    out[j++] = s[i];
}

int main(int argc, char *argv[]) {
  imagem img, *out;
  int i;
  char str[MAX];
  char last_word[MAX];

  for (i = 2; i < argc; i++)
  {
    img = abrir_imagem(argv[i]);
    out = create_image(img.width, img.height);

    if (argv[1][0] == 't')
      blur_thread(&img, out, N);
    else if (argv[1][0] == 'p')
      blur_process(&img, out, N); /* blur_process(&img, out, N); */
    else
      blur_image(&img, out);

    strcpy(str, "out/");
    cut_last_word('/', argv[i], last_word);
    strcat(str, argv[1]);
    strcat(str, "_");
    strcat(str, last_word);
    salvar_imagem(str, out);
  }

  return 0;
}
