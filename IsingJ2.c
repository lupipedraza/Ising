
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

//---------------------AVISO LAS FUNCIONES QUE VAMOS A USAR
int poblar(int *red,float p,int dim);
int imprimirMat(int *red, int dim);
int imprimirVector(int *vect, int dim);
int NuevoSpin(int *red, int dim,float J, float H, int m, float *TablaExponencial, float *MyReturn);
int magnitizacion(int *red,int dim);
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
	int dim = 30;
	float p = 0.5;
	int *red;
	float H=0; //coeficientes
	int m;
	 //la red ahora tiene un +2 para contemplar el borde
	red=(int *)malloc((dim+2)*(dim+2)*sizeof(int));
	int iteraciones=10000000;
	float * TablaExponencial;
	TablaExponencial=(float *)malloc(5*sizeof(float));

	float *MyReturn;
	MyReturn=(float *)malloc((2)*(2)*sizeof(float));

	FILE * fp;
	char filename[64];
		//Para guardar
	sprintf(filename,"magnetizacion_J_iter=%d.txt",iteraciones);


	fp = fopen (filename, "w");
	for (float J=0.1;J<0.7;J+=0.01)
	{
		/*fprintf(fp,"J= %f", J);
		fprintf(fp,"\n\n");
		fprintf(fp,"m	e\n");*/

		//TablaExponencial=[-8J,-4J,0,4J,8J]
		*(TablaExponencial+0)=exp(-8*J);
		*(TablaExponencial+1)=exp(-4*J);
		*(TablaExponencial+2)=1.0;
		*(TablaExponencial+3)=1.0;
		*(TablaExponencial+4)=1.0;//puedo poner 1?

		poblar(red, p,dim);
	 	//imprimirMat(red, dim);
		m=magnitizacion(red, dim);
		//printf("%f", J);
		fprintf(fp,"%i ", m);
		fprintf(fp,"%f\n", 0.0);

		int i=0;
		while (i<iteraciones)
		{
			for (int j=0;j<2000;j++){
				NuevoSpin(red, dim, J, H,m,TablaExponencial, MyReturn);
				i=i+1;
			}
			fprintf(fp,"%f ", *(MyReturn+0));
			fprintf(fp,"%f\n", *(MyReturn+1));
		}
		/*fprintf(fp,"\n");*/
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
  for (j=1; j<dim; j++)
  {
	*(red+j)=*(red+(dim+2)*dim+j);//Arriba
	*(red+(dim+2)*(dim+1)+j)=*(red+(dim+2)+j);//Arriba
	*(red+j*(dim+2))=*(red+j*(dim+2)+dim);//Izquierda
	*(red+j*(dim+2)+dim+1)=*(red+j*(dim+2)+1);//Derecha

  }
  return 0;
}
//*************************************************************
int imprimirMat(int *red, int dim)
    {
        int k, l;
        for (k=1; k<dim+1; k++)
	{
            for(l=1; l<dim+1; l++)
            {
		printf("%d", *(red+((dim+2)*k+l)));
		}
	        printf("\n");
        }
	printf("\n");
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
int NuevoSpin(int *red, int dim,float J, float H, int m, float *TablaExponencial, float *MyReturn)
	{

	int casilleroi;
	int casilleroj;
	int casillero;
	float p = 0.5;
	int Delta;
	//El casillero elegido. Hay que hacer que no sea del borde, así que elegimos ambas coordenadas por separado
	casilleroi=rand() % (dim);
	casilleroj=rand() % (dim);
	casillero=(dim+2)*(casilleroj+1)+casilleroi+1;
	Delta=(*(red+casillero)*(*(red+casillero+1)+*(red+casillero-1)+*(red+casillero+dim+2)+*(red+casillero-dim-2)));
	if (Delta==-8)
	{p=*TablaExponencial;}
	if (Delta==-4)
	{p=*TablaExponencial+1;}
	if (Delta==0)
	{p=*TablaExponencial+2;}
	if (Delta==4)
	{p=*TablaExponencial+3;}
	if (Delta==8)
	{p=*TablaExponencial+4;}


	float rndm;
	rndm=(float)rand()/(float)RAND_MAX;
	if(rndm<p) //Tira la "moneda" y ve si es menor
	{*(red+casillero)=-*(red+casillero);
	 m=m+*(red+casillero)*2;
	}

	//calculo la energia
	float e=0.0;
	e= -J*Delta;
	//Arreglar los bordes
	for (int j=1; j<dim; j++)
	{
	*(red+j)=*(red+(dim+2)*dim+j);//Arriba
	*(red+(dim+2)*(dim+1)+j)=*(red+(dim+2)+j);//Arriba
	*(red+j*(dim+2))=*(red+j*(dim+2)+dim);//Izquierda
	*(red+j*(dim+2)+dim+1)=*(red+j*(dim+2)+1);//Derecha

	}

	*(MyReturn+0)=(float)m;
	*(MyReturn+1)=e;
	return 0;
}

//*************************************************************

int magnitizacion(int *red,int dim)
{
	int k,l;
	int sum=0;
	for (k=1; k<dim+1; k++)
	{
            for(l=1; l<dim+1; l++)
            {
		sum=sum+*(red+((dim+2)*k+l));

	    }

        }


	return sum;


}
//*************************************************************

int clasificar(int *red, int dim,int *historial,int etiqueta)
{

	int j,i;
	int s1;
	int s2;

	if (*red)// la primer celda
	{
	*red=etiqueta;
	etiqueta++;
	}
	for(i=1; i<dim;i++)//La primer fila
	{
		if(*(red+i)>0)// Si tiene un 1
			etiqueta=actualizar(red+i,historial,*(red+(i-1)),etiqueta);

	}

	for(j=1;j<dim;j++) //el resto de las filas
	{

		if(*(red+j*dim)>0) // La primer columna de cada fila, si tiene un 1
			etiqueta=actualizar(red+j*dim,historial,*(red+(j-1)*dim),etiqueta);

		for(i=1;i<dim;i++) // el resto de las columnas
		{

				if(*(red+j*dim+i)>0) // si tiene un 1
				{

					s1=*(red+j*dim+i-1); // la de la izquierda
					s2=*(red+(j-1)*dim+i);	// la de arriba

						if(s1*s2>0) // si las dos son positivas
						{
						etiqueta_falsa((red+j*dim+i),historial,s1,s2,etiqueta);
						}
						else
						{
							if(s1>0)
							{etiqueta=actualizar((red+j*dim+i),historial,s1,etiqueta);
							}
							else
							{etiqueta=actualizar((red+j*dim+i),historial,s2,etiqueta);
							}
						}

				}
		}

	}

	return 0;
}

//*************************************************************
int actualizar(int *local,int *historial,int s,int etiqueta)
{
	if (s>0)
	{
	while(*(historial+s)<0)
		{
			s=-*(historial+s);
		}
	*local=s;
	}else{
	*local=etiqueta;
	etiqueta++;
	}
	return etiqueta;
}
//*************************************************************
int etiqueta_falsa(int *local, int *historial, int s1, int s2, int etiqueta)
{
	int mini,maxi;
	while(*(historial+s1)<0)
	{
		s1=-(*(historial+s1));
	}
	while(*(historial+s2)<0)
	{
		s2=-(*(historial+s2));
	}
	if(s1<s2)
	{
	mini=s1;
	maxi=s2;
	}else{
	mini=s2;
	maxi=s1;
	}
	*local=mini;
	*(historial+maxi)=-mini;
	*(historial+mini)=mini;
	return 0;
}
//*************************************************************
int arreglar_etiquetas(	int *red,int *historial, int dim)
{
int i;
int s;
for (i=0; i<dim*dim; i++)
{	s=*(historial+*(red+i));
	while(s<0)
	{
	s= *(historial +(-s));
	}
	*(red+i)=s;
}
return 0;
}

//*************************************************************
int percola(int *red, int dim)
{
int i;
int j=0;
int b=0;
int *vect1;
vect1=(int *)malloc(dim*dim*sizeof(int));
int *vect2;
vect2=(int *)malloc(dim*dim*sizeof(int));
int k;


for (i=0; i<dim; i++)
{

	if (*(red+i)>0)
	{
	*(vect1+*(red+i))=1;
	}if(*(red+(dim*(dim-1))+i))
	{
	*(vect2+*(red+(dim*(dim-1))+i))=1;
	}
}

while (*(vect1+j) * *(vect2+j)==0 && j<dim*dim)
{
j++;
}

if (j<dim*dim)
{

	for (i=0; i<dim; i++)
	{

		for (k=0; k<dim; k++)
		{
			if (*(red+i*dim+k)==j){

			b++;
			}
		}
	}
}


return b;
}
//*************************************************************
