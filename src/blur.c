#include "imageprocessing.h"
#include "blur.h"
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define N 9
#define PAD (N-1)/2

pthread_mutex_t key;

void create_padding(imagem *I)
{
  for (int k = 0; k < PAD; k++)
  {
    for (int j = 0; j < I->width; j++)
    {
      I->r[(I->width)*k + j] = 0;
      I->r[(I->height-(k+1))*I->width + j] = 0;
      I->g[(I->width)*k + j] = 0;
      I->g[(I->height-(k+1))*I->width + j] = 0;
      I->b[(I->width)*k + j] = 0;
      I->b[(I->height-(k+1))*I->width + j] = 0;
    }

    for (int i = 0; i < I->height; i++)
    {
      I->r[i*I->width + k] = 0;
      I->r[I->width*i + (I->width - (k + 1))] = 0;
      I->g[i*I->width + k] = 0;
      I->g[I->width*i + (I->width - (k + 1))] = 0;
      I->b[i*I->width + k] = 0;
      I->b[I->width*i + (I->width - (k + 1))] = 0;
    }
  }
}


imagem *create_image(int width, int height, int is_p)
{
  imagem *I;
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  if (is_p == 1)
  {
    I = (imagem *) mmap(NULL, sizeof(imagem), protection, visibility, 0, 0);
    if (I == NULL)
      exit(-1);

    I->width = width;
    I->height = height;

    I->r = (float *) mmap(NULL, sizeof(float) * I->width * I->height, protection, visibility, 0, 0);
    I->g = (float *) mmap(NULL, sizeof(float) * I->width * I->height, protection, visibility, 0, 0);
    I->b = (float *) mmap(NULL, sizeof(float) * I->width * I->height, protection, visibility, 0, 0);
  }
  else
  {
    I = (imagem *) malloc(sizeof(imagem));
    if (I == NULL)
      exit(-1);

    I->width = width;
    I->height = height;

    I->r = (float *) malloc(sizeof(float) * I->width * I->height);
    I->g = (float *) malloc(sizeof(float) * I->width * I->height);
    I->b = (float *) malloc(sizeof(float) * I->width * I->height);
  }

  if (I->r == NULL || I->g == NULL || I->b == NULL)
    exit(-1);

  return I;
}

float blur(float *image, int x, int y, int width, int height)
{
  float total = 0;
  float filter[N*N];

  for (int i = 0; i < N*N; i++)
    filter[i] = 1.0/((float) N*N);

  for(int i=-N/2; i < N/2+1; i++)
    for(int j =-N/2; j < N/2+1; j++)
      total += image[(x+i)*width+y+j] * filter[(N/2+i)*N+j+N/2];

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

  for (int i = 0; i < (img->width)*(img->height); i++)
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

int * alloc_mmap(int n)
{
  int * ans;

  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  ans = (int *) mmap(NULL, sizeof(int), protection, visibility, 0, 0);

  if ((int) ans == -1)
    exit(-1);

  return ans;
}

void blur_process(imagem *I, imagem *output, int n)
{
  pid_t *pid;
  int *aux;
  int is_undone;
  int x, y;

  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  pid = (pid_t *) malloc(sizeof(pid_t) * n);
  if (pid == NULL)
    exit(-1);

  aux = alloc_mmap(I->width * I->height);

  for (int i = 0; i < (I->width * I->height); i++)
    aux[i] = 0;

  create_padding(output);

  /* for (int k = 0; k < PAD; k++) */
  /* { */
  /*   for (int j = 0; j < I->width; j++) */
  /*   { */
  /*     aux[(I->width)*k + j] = 1; */
  /*     aux[(I->height)-(k+1)*I->width + j] = 1; */
  /*   } */
  /*   for (int i = 0; i < I->height; i++) */
  /*   { */
  /*     aux[i*I->width + k] = 1; */
  /*     aux[I->width*i + (I->width - (k + 1))] = 1; */
  /*   } */
  /* } */

  for (int i = 0; i < n; i++)
  {
    pid[i] = fork();

    /* Child process. */
    if (pid[i] == 0)
    {
      for (int j = 0; j < (I->width)*(I->height); j++)
      {
        pthread_mutex_lock(&key);
        is_undone = (aux[j] == 0);
        if (is_undone)
          aux[j] = 1;
        pthread_mutex_unlock(&key);

        if (is_undone)
        {
          x = j/(I->width);
          y = j%(I->width);
          output->r[j] = blur(I->r, x, y, I->width, I->height);
          output->b[j] = blur(I->b, x, y, I->width, I->height);
          output->g[j] = blur(I->g, x, y, I->width, I->height);
        }
      }
      exit(0);
    }
  }

  /* Wait all processes to finish. */
  for (int i = 0; i < n; i++)
    waitpid(pid[i], NULL, 0);
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

  create_padding(output);

  /* for (int k = 0; k < PAD; k++) */
  /* { */
  /*   for (int j = 0; j < I->width; j++) */
  /*   { */
  /*     aux[(I->width)*k + j] = 1; */
  /*     aux[(I->height)-(k+1)*I->width + j] = 1; */
  /*   } */
  /*   for (int i = 0; i < I->height; i++) */
  /*   { */
  /*     aux[i*I->width + k] = 1; */
  /*     aux[I->width*i + (I->width - (k + 1))] = 1; */
  /*   } */
  /* } */
  /*  */
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

  create_padding(output);

  for(int i=PAD;i<I->height-2*PAD;i++)
  {
    for(int j=PAD;j<I->width-2*PAD;j++)
    {
      output->r[i*I->width+j] = blur(I->r, i, j, I->width, I->height);
      output->b[i*I->width+j] = blur(I->b, i, j, I->width, I->height);
      output->g[i*I->width+j] = blur(I->g, i, j, I->width, I->height);
    }
  }
}

