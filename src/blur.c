#include "imageprocessing.h"
#include <stdlib.h>

imagem *create_image(int width, int height)
{
  imagem *I;
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


imagem *blur_image(imagem *I)
{
  imagem *output;
  output = create_image(I->width, I->height);

  for(int j=0;j<I->height-2;j++)
  {
    for(int i=0;i<I->width-2;i++)
    {
      output->r[(j+1)*I->width+i+1] = blur(I->r, i, j, I->width, I->height);
      output->b[(j+1)*I->width+i+1] = blur(I->b, i, j, I->width, I->height);
      output->g[(j+1)*I->width+i+1] = blur(I->g, i, j, I->width, I->height);
    }
  }
  liberar_imagem(I);
  return output;
}
