#include "imageprocessing.h"

imagem *blur_image(imagem *I);

float blur(float *image, int x, int y, int width, int height);

imagem *create_image(int width, int height);
