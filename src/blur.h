#include "imageprocessing.h"

void blur_image(imagem *I, imagem*);

float blur(float *image, int x, int y, int width, int height);

imagem *create_image(int width, int height);