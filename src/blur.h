#include "imageprocessing.h"

typedef struct {
  int * aux;
  imagem * img;
  imagem * output;
} args;

int * alloc_mmap(int n);

void blur_process(imagem *I, imagem *output, int n);

void create_padding(imagem *I);

void blur_image(imagem *I, imagem*);

float blur(float *image, int x, int y, int width, int height);

imagem *create_image(int width, int height, int is_p);

void *worker(void *args);

void blur_thread(imagem *I, imagem *output, int n);
