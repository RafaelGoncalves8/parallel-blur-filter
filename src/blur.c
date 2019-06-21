#include "imageprocessing.h"
#include "blur.h"
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t key;

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
  /* Average. */
  float filter[25] = {
            1/25,   1/25,   1/25,  1/25,  1/25,
            1/25,   1/25,   1/25,  1/25,  1/25,
            1/25,   1/25,   1/25,  1/25,  1/25,
            1/25,   1/25,   1/25,  1/25,  1/25,
            1/25,   1/25,   1/25,  1/25,  1/25
            };

  /* Gaussian. */
  /* float filter[25] = { */
  /*           1/256,  4/256,  6/256,  4/256,  1/256, */
  /*           4/256, 16/256, 24/256, 16/256,  4/256, */
  /*           6/256, 24/256, 36/256, 24/256,  6/256, */
  /*           4/256, 16/256, 24/256, 16/256,  5/256, */
  /*           1/256,  4/256,  6/256,  4/256,  1/256 */
  /*           }; */

  for(int i=-2; i < 3; i++)
    for(int j =-2; j < 3; j++)
      total += image[(x+i)*width+y+j] * filter[(2+i)*5+j+2];

  if (total > 255)
    total = 255;

  return total;
}

void *worker(void *arg)
{
  args *argv;
  argv = (args *) arg;

  int is_undone, x, y;
  int *aux = argv->aux;
  imagem *img = argv->img;
  imagem *output = argv->output;

  for (int i; i < (img->width)*(img->height); i++)
  {
    pthread_mutex_lock(&key);
    is_undone = (aux[i] == 0);
    if (is_undone)
      aux[i] = 1;
    pthread_mutex_unlock(&key);

    if (is_undone)
    {
      x = i/img->width;
      y = i%img->width;
      output->r[i] = blur(img->r, x, y, img->width, img->height);
      output->b[i] = blur(img->b, x, y, img->width, img->height);
      output->g[i] = blur(img->g, x, y, img->width, img->height);
    }
  }
}

void blur_thread(imagem *I, imagem *output, int n)
{
  pthread_t *workers;
  int *aux;
  args *argv;

  argv = (args *) malloc(sizeof(args));
  if (argv == NULL)
    exit(-1);

  workers = (pthread_t *) malloc(sizeof(pthread_t) * n);
  if (workers == NULL)
    exit(-1);

  aux = (int *) malloc(sizeof(int) * (I->width * I->height));
  if (aux == NULL)
    exit(-1);

  for (int i = 0; i < (I->width * I->height); i++)
    aux[i] = 0;

  for (int j = 0; j < I->width; j++)
  {
    output->r[j] = 0;
    output->r[(I->height-1)*I->width + j] = 0;
    output->g[j] = 0;
    output->g[(I->height-1)*I->width + j] = 0;
    output->b[j] = 0;
    output->b[(I->height-1)*I->width + j] = 0;
    aux[j] = 1;
    aux[(I->height-1)*I->width + j] = 1;
  }

  for (int i = 0; i < I->height; i++)
  {
    output->r[i*I->width] = 0;
    output->r[I->width*i + (I->width - 1)] = 0;
    output->g[i*I->width] = 0;
    output->g[I->width*i + (I->width - 1)] = 0;
    output->b[i*I->width] = 0;
    output->b[I->width*i + (I->width - 1)] = 0;
    aux[i*I->width] = 1;
    aux[I->width*i + (I->width - 1)] = 1;
  }


  argv->aux = aux;
  argv->img = I;
  argv->output = output;

  for (int i =0; i < n; i++)
    pthread_create(&(workers[i]), NULL, worker, (void *) argv);

  for (int i=0; i < n; i++)
    pthread_join(workers[i], NULL);

  free(aux);
  free(argv);
  free(workers);
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
