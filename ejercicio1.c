
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

	int iteraciones=50;
	int x0=2;
	int i;
	float delta;
	float paso;
	float xf,xi;
	xi=x0;
	float pa;
	float rndm;
	int acept=0;
	float pasodelta=0.1;
	char filename[64];
	//Para guardar
	sprintf(filename,"Tira_it=%d.txt",iteraciones);
	FILE * fp;
	fp = fopen (filename, "a");

	for(delta=0.1;1;delta=pasodelta+delta)
	{
		fprintf(fp,"%f ",delta);
		for (i=0;i<iteraciones;i++)
		{
			paso=-delta+2*delta*(float)rand()/(float)RAND_MAX;	
			xf=xi+paso;
			pa=exp((pow(xf,2)-pow(xi,2))/2);
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
		fprintf(fp,"%d ",acept);
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

		


