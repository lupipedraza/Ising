
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

//---------------------AVISO LAS FUNCIONES QUE VAMOS A USAR
int poblar(int *red,float p,int dim);
int imprimirMat(int *red, int dim, FILE *fp);
int imprimirVector(int *vect, int dim);
int NuevoSpin(int *red, int dim,float J, float H, float *TablaExponencial, float *MyReturn);
int magnitizacion(int *red,int dim,float J,  float *MyReturn);
//Todas estas no se usan pero las deje
int clasificar(int *red, int dim, int *historial,int etiqueta);
int actualizar(int *local,int *historial,int s,int etiqueta);
int etiqueta_falsa(int *local, int *historial, int s1, int s2, int etiqueta);
int arreglar_etiquetas(	int *red,int *historial, int dim);
int percola(int *red, int dim);

//---------------------Main--------------------------------
int main()

{

	srand(time(NULL));
	int dim = 32;
	float p = 1;
	int *red;
	float H=1; //coeficientes

	 //la red ahora tiene un +2 para contemplar el borde
	red=(int *)malloc((dim+2)*(dim+2)*sizeof(int));
	int iteraciones=100000;
	int i;
	float * TablaExponencial;
	TablaExponencial=(float *)malloc(5*sizeof(float));

	float *MyReturn;
	MyReturn=(float *)malloc(2*sizeof(float));

	FILE * fp;
	char filename[64];
		//Para guardar
	sprintf(filename,"magnetizacion_J_iter_anti=%d.txt",iteraciones);

	fp = fopen (filename, "w");
	for (float J=0.1;J<3;J+=0.03)
	{
		i=0;
		//TablaExponencial=[-8J,-4J,0,4J,8J]
		*(TablaExponencial+0)=exp(-8*J);
		*(TablaExponencial+1)=exp(-4*J);
		*(TablaExponencial+2)=1.0;
	
		poblar(red, p,dim);
	 	//imprimirMat(red, dim);
		magnitizacion(red, dim,J, MyReturn);
		
		while (i<iteraciones)
		{
			for (int j=0;j<2000;j++){
				NuevoSpin(red, dim, J, H,TablaExponencial, MyReturn);
				i=i+1;
			}
			fprintf(fp,"%f ", *(MyReturn+0));
			fprintf(fp,"%f\n", *(MyReturn+1));
			

		}
	//imprimirMat(red, dim,fp);
	
	}

	fclose(fp);

	free(MyReturn);
	free(red);
	return 0;
}

//---------------------FUNCIONES--------------------------------
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int poblar(int *red,float p,int dim)
{
  int i,j;
  float rndm;
  for (i=0; i<(dim+2)*(dim+2); i++) //Poblamos todo, y despues sobre escribimos los bordes
  {
    rndm=(float)rand()/(float)RAND_MAX;
    *(red+i)=-1;
    if (rndm < p)
    {
        *(red+i)=1;
    }
  }
 //Bordes
  for (j=1; j<dim+1; j++)
  {
	*(red+j)=*(red+((dim+2)*dim)+j);//Arriba
	*(red+(dim+2)*(dim+1)+j)=*(red+(dim+2)+j);//Abajo
	*(red+j*(dim+2))=*(red+j*(dim+2)+dim);//Izquierda
	*(red+j*(dim+2)+dim+1)=*(red+j*(dim+2)+1);//Derecha

  }
  return 0;
}
//*************************************************************
int imprimirMat(int *red, int dim, FILE *fp)
    {
        int k, l;
        for (k=1; k<dim+1; k++)
	{
            for(l=1; l<dim+1; l++)
            {
		fprintf(fp,"%d ", *(red+((dim+2)*k+l)));
		}
	        fprintf(fp,"\n");
        }
	fprintf(fp,"\n");
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
//**************Da vuelta (o no) un spin************************
int NuevoSpin(int *red, int dim,float J, float H, float *TablaExponencial, float *MyReturn)
	{
	srand(time(NULL));
	float L=dim*dim;
	int casilleroi;
	int casilleroj;
	int casillero;
	float p = 0.5;
	int Delta;
	//El casillero elegido. Hay que hacer que no sea del borde, así que elegimos ambas coordenadas por separado
	casilleroi=rand() % (dim);
	casilleroj=rand() % (dim);
	casillero=(dim+2)*(casilleroj+1)+casilleroi+1;
	Delta=2*(*(red+casillero)*(*(red+casillero+1)+*(red+casillero-1)+*(red+casillero+dim+2)+*(red+casillero-dim-2)));
	p=exp(J*Delta-2*H*(*(red+casillero)));	

	
	


	float rndm;
	rndm=(float)rand()/(float)RAND_MAX;
	if(rndm<p) //Tira la "moneda" y ve si es menor
	{*(red+casillero)=-*(red+casillero);
	 *(MyReturn+0)=*(MyReturn+0)+(*(red+casillero)*2)/L;

	 //calculo la energia
	 float e;
	 e= -J*Delta;
	 *(MyReturn+1)=*(MyReturn+1)+(e/L);
	 //Arreglar los bordes
	 for (int j=1; j<dim+1; j++)
	 {
	*(red+j)=*(red+((dim+2)*dim)+j);//Arriba
	*(red+(dim+2)*(dim+1)+j)=*(red+(dim+2)+j);//Abajo
	*(red+j*(dim+2))=*(red+j*(dim+2)+dim);//Izquierda
	*(red+j*(dim+2)+dim+1)=*(red+j*(dim+2)+1);//Derecha

	 }
	}

	return 0;
}

//*************************************************************

int magnitizacion(int *red,int dim,float J, float *MyReturn)
{
	float L=dim*dim;
	int k,l;
	float m=0;
	float e=0;
	int casillero;
	for (k=1; k<dim+1; k++)
	{
  	for(l=1; l<dim+1; l++)
    {
		casillero=(dim+2)*k+l;
		m=m+*(red+casillero);
		e=e-J*(*(red+casillero)*(*(red+casillero+1)+*(red+casillero-1)+*(red+casillero+dim+2)+*(red+casillero-dim-2)));
		}
  }
	*(MyReturn+0)=m/L;
	*(MyReturn+1)=e/(2*L);

	return 0;


}

