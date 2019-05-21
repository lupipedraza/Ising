
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

//---------------------AVISO LAS FUNCIONES QUE VAMOS A USAR

int imprimirVector(int *vect, int dim);

int main()
{
	srand(time(NULL));

	int iteraciones=10000;
	int x0=0;
	int i;
	float delta;
	float paso;
	float xf,xi;
	xi=x0;
	float pa;
	float rndm;
	float pasodelta=0.01;
	char filename[64];
	//Para guardar
	sprintf(filename,"Tira_it=%d.txt",iteraciones);
	FILE * fp;
	fp = fopen (filename, "w");

	for(float delta=0.1;delta<10;delta=pasodelta+delta)
	{
		int acept=0;
		fprintf(fp,"%f ",delta);
		for (i=0;i<iteraciones;i++)
		{
			paso=-delta+2*delta*(float)rand()/(float)RAND_MAX;	
			xf=xi+paso;
			pa=exp(-(pow(xf,2)-pow(xi,2))/2);
			rndm=(float)rand()/(float)RAND_MAX;
			if (rndm<pa)
			{	
				fprintf(fp,"%f ",xf);
				xi=xf;
				acept++;
				
			}
			else
			{	fprintf(fp,"%f ",xi);
			}
				
		}
		fprintf(fp,"%d \n",acept);
	}
	fclose(fp);
return 0;
}
//*************************************************************
int imprimirVector(int *vect, int dim)
    {
        int k;
        for (k=0; k<dim*dim; k++)
	{printf("%d", *(vect+k));
        }
	 printf("\n");

    return 0;
    }

		


