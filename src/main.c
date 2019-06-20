#include "blur.h"
#include "imageprocessing.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  imagem *img, *out;
  int i;

  img = (imagem *) malloc(sizeof(imagem) * argc);
  for (i = 0, i < argc; i++)
    img[i] = abrir_imagem(argv[i]);

#if DEBUG
  printf("%d, %d, %f, %f\n", img.width, img.height, img.r[0], img.r[img.width + img.height -1]);
#endif

  out = create_image(img.width, img.height);
  blur_image(&img, out);

#if DEBUG
  printf("%d, %d", out->width, out->height);
  printf(" %f, %f\n", out->r[0], out->r[out->width + out->height -1]);
#endif

  salvar_imagem("00-out.jpg", out);
  liberar_imagem(&img);
  liberar_imagem(out);
  return 0;
}
