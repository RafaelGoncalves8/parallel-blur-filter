#include "blur.h"
#include "imageprocessing.h"

int main() {
  imagem img;
  img = abrir_imagem("data/00.jpg");

  img = blur_image(img);

  salvar_imagem("00-out.jpg", &img);
  liberar_imagem(&img);
  return 0;
}
