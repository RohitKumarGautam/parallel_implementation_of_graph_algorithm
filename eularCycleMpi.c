#include<stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include<time.h>
#include<math.h>
#define n 4     
MPI_Status status;
int count=0,sum=0,ans,send_count;
double start,end;

int main(int argc, char **argv)
 {
  clock_t t;
  int a[n][n],deg[n];
  int numtasks,taskid,numworkers,source,dest,rows,offset,i,j,k,ver,temp;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

  numworkers = numtasks-1;
  t=clock();
  /*--------------------------- master ----------------------------*/
  if (taskid == 0) {
    printf("enter the no. of vertices\n");  
   // scanf("%d",&n);  
    for(i=0;i<n;i++)  
    {  
        for(j=0;j<n;j++)  
        {  
            a[i][j]=0;  
        }  
    }
    for(i=0;i<n;i++)  
    {  
        printf("enter the no. of adjacent vertices of vertex %d\n",i+1);       
        scanf("%d",&ver);
         deg[i]=ver;  
        printf("enter vertices nos. which are adjacent to vertex %d\n",i+1);  
        for(j=0;j<ver;j++)  
        {  
               scanf("%d",&temp);  
               a[i][temp-1]=1;                                                   
        }  
   }    

    /* send matrix data to the worker tasks */
    rows = n/numworkers;
    offset = 0;

    for (dest=1; dest<=numworkers; dest++)
    {
      MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&a[offset][0], rows*n, MPI_INT,dest,1, MPI_COMM_WORLD);
   //   MPI_Send(&b, N*N, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
      offset = offset + rows;
    }

    /* wait for results from all worker tasks */
    for (i=1; i<=numworkers; i++)
    {
      source = i;
      MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&a[offset][0], rows*n, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
   }
  // printf("count = %d \n",count);
  MPI_Recv(&count,1,MPI_INT,source,2,MPI_COMM_WORLD,&status);
   printf("count = %d\n",count);
   printf("Here is the result matrix:\n");
    for (i=0; i<n; i++) {
      for (j=0; j<n; j++)
        printf("%d   ", a[i][j]);
      printf ("\n");
    }
   
    if(ans==0 && ( count == 0)){
        printf("Eular cycle exist\n"); }
    else {
        printf("No eular cycle\n"); }    

   

  }

  /*---------------------------- worker----------------------------*/
  if (taskid > 0) {
    source = 0;
    MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&a, rows*n, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
   // MPI_Recv(&b, N*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);

   // printf("matrix in worker\n");
    for (k=0; k<n; k++){
     sum=0;
      for (i=0; i<rows; i++) {
            sum=sum+a[i][k];
        } 
          //  printf(" for %d sum = %d \n",k,sum);
             if(sum==0) {
                  ans=1; }
                if(sum%2==1) {
                      count++;
               //   printf("count in workers %d\n",count);
                   }
      }
   MPI_Reduce(&count,&send_count,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

   // printf("After - reduce count = %d\n",count);
    MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&a, rows*n, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&send_count,1,MPI_INT,0,2,MPI_COMM_WORLD);
  }

   t = clock()-t;
	
	if ( taskid == 0 ) {
			printf( "Total time spent is %f\n", ((double)t) / CLOCKS_PER_SEC  );
	}
    MPI_Finalize(); 

  return 0;
}

