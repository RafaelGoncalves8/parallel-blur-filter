#include "imageprocessing.h"

typedef struct {
  int * aux;
  imagem * img;
  imagem * output;
} args;

void blur_image(imagem *I, imagem*);

float blur(float *image, int x, int y, int width, int height);

imagem *create_image(int width, int height);

void *worker(void *args);

void blur_thread(imagem *I, imagem *output, int n);
