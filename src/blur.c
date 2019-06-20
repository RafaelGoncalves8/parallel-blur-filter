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
  float total = image[(x+1)*width+y+1];
  int filter[9] = {
            1,   4,  1,
            4, -20,  4,
            1,   4,  1
            };

  for(int i=-1; i < 2; i++)
    for(int j =-1; j < 2; j++)
      total += image[(x+j)*width+y+i] * filter[(1+i)*3+j+1];

  if (total > 255)
    total = 255;
  else if (total < 0)
    total = 0;

  return total;
}

void blur_image(imagem *I, imagem *output)
{
  /* imagem *output; */
  /* output = create_image(I->width, I->height); */

  for (int j = 0; j < I->width; j++)
  {
    output->r[j] = 0;
    output->r[(I->height-1)*I->width + j] = 0;
    output->g[j] = 0;
    output->g[(I->height-1)*I->width + j] = 0;
    output->b[j] = 0;
    output->b[(I->height-1)*I->width + j] = 0;
  }

  for (int i = 0; i < I->height; i++)
  {
    output->r[i*I->width] = 0;
    output->r[I->width*i + (I->width - 1)] = 0;
    output->g[i*I->width] = 0;
    output->g[I->width*i + (I->width - 1)] = 0;
    output->b[i*I->width] = 0;
    output->b[I->width*i + (I->width - 1)] = 0;
  }

  for(int i=1;i<I->height-2;i++)
  {
    for(int j=1;j<I->width-2;j++)
    {
      output->r[i*I->width+j] = blur(I->r, i, j, I->width, I->height);
      output->b[i*I->width+j] = blur(I->b, i, j, I->width, I->height);
      output->g[i*I->width+j] = blur(I->g, i, j, I->width, I->height);
    }
  }
  /* liberar_imagem(I); */
  /* I = output; */
}
