#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]){

		int menum,nproc;
		int middle,first,last=0;
		int n,nloc;
		int search,*a;
		int *aloc;
	
		int index=0;	
		int key=-1;
		int i=0;
		
		int result=0;
		
		double t0,t1,time;
		
		MPI_Status status;
		MPI_Init(&argc,&argv);

		MPI_Comm_rank(MPI_COMM_WORLD,&menum); // Ritorna il rank (id) del processore che la sta chiamando es. al primo passo lo chiama il processore 0 al secondo il processore 1 ecc.
		MPI_Comm_size(MPI_COMM_WORLD,&nproc); // Ritorna il numero di processori utilizzati

		if(menum==0){	
		if(argc==2){
			n=atoi(argv[1]);
		}
			else
			n=64;
		a=calloc(n,sizeof(int));
	}

	 if(menum==0){
                for(i=0;i<n;i++)
                        a[i]=i+1;
		}

	/* Il processore root invia a tutti il size dell'array in modo che ogni processore si calcola il size locale */
	MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
	

	/*Calcolo il size parziale per ogni processroe */
	nloc=n/nproc;

	aloc=calloc(nloc,sizeof(int));
	
	/* Distribuisco l'array a scaglioni tra i processori */
	MPI_Scatter(a,nloc,MPI_INT,aloc,nloc,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);	


	if(menum==0){
		printf("Inserire valore da ricercare : ");
		scanf("%d",&key);
	}	
	
	/* Inizio il calcolo del tempo */
	if(menum==0){
		t0=MPI_Wtime();
	}
	/* Invio a tutti il valore da ricercare */
	MPI_Bcast(&key,1,MPI_INT,0,MPI_COMM_WORLD);

	first=0;
	last=nloc-1;
	middle=(first+last)/2;
	
	while(first<=last)
	{	
	if(aloc[middle]<key)
		first=middle+1;

		else if(aloc[middle]==key){
			index=middle+1;
			result=1;
			break; //Se ho trovato il valore fermo il ciclo		
	}
	else 
		last=middle-1;
	
	middle=(first+last)/2;
	}
	if(menum==0){
		t1=MPI_Wtime();
		time=(t1-t0);
	}

	if(first>n)
		printf("Impposibile trovare tale valore poichÃ¨ supera la dimensione dell'array\n");

	if(result==1)
	printf("Il processore %d ha trovato il valore %d nella sua memoria locale \n",menum,key);

	if(menum==0)
	printf("Tempo impiegato = %f\n",time);

	
	MPI_Finalize();
	return 0;

}
