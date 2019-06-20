#include "imageprocessing.h"
#include "blur.h"
#include <stdlib.h>

imagem *create_image(int width, int height)
{
  imagem *I;

  I = (imagem *) malloc(sizeof(imagem));
  if (I == NULL)
    exit(-1);

  I->width = width;
  I->height = height;
  I->r = (float *) malloc(sizeof(float) * I->width * I->height);
  I->g = (float *) malloc(sizeof(float) * I->width * I->height);
  I->b = (float *) malloc(sizeof(float) * I->width * I->height);

  if (I->r == NULL || I->g == NULL || I->b == NULL)
    exit(-1);

  return I;
}

float blur(float *image, int x, int y, int width, int height)
{
  float total = image[(y+1)*width+x+1];
  int filter[9] = {
            1,   4,  1,
            4, -20,  4,
            1,   4,  1
            };

  for(int i=0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      total += image[(y+j)*width+x+i] * filter[j*3+x];

  if (total > 255)
    total = 255;
  else if (total < 0)
    total = 0;

  return total;
}

void blur_image(imagem *I)
{
  imagem *output;
  output = create_image(I->width, I->height);

  for (int i = 0; i < I->width; i++)
  {
    output->r[i*I->width] = 0;
    output->r[i*I->width + I->height -1] = 0;
    output->g[i*I->width] = 0;
    output->g[i*I->width + I->height -1] = 0;
    output->b[i*I->width] = 0;
    output->b[i*I->width + I->height -1] = 0;
  }

  for (int j = 0; j < I->height; j++)
  {
    output->r[j] = 0;
    output->r[I->width -1 + j] = 0;
    output->g[j] = 0;
    output->g[I->width -1 + j] = 0;
    output->b[j] = 0;
    output->b[I->width -1 + j] = 0;
  }

  for(int i=0;i<I->height-2;i++)
  {
    for(int j=0;j<I->width-2;j++)
    {
      output->r[(i+1)*I->width+j+1] = blur(I->r, i, j, I->width, I->height);
      output->b[(i+1)*I->width+j+1] = blur(I->b, i, j, I->width, I->height);
      output->g[(i+1)*I->width+j+1] = blur(I->g, i, j, I->width, I->height);
    }
  }
  liberar_imagem(I);
  I = output;
}
