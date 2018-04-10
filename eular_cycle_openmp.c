#include<stdio.h>  
#include<stdlib.h>  
#include<omp.h>
int main()  
{  
    int tid, nthreads, i, j, k, chunk;
     chunk = 10;
    int a[10][10],n,ver, temp,count=0;   
    printf("enter the no. of vertices\n");  
    scanf("%d",&n);  
    #pragma omp parallel shared(nthreads,chunk) private(tid,i,j,k)
  {
  tid = omp_get_thread_num();
  if (tid == 0)
    {
    nthreads = omp_get_num_threads();
    printf("Starting eular path with %d threads\n",nthreads);
   
    }


    #pragma omp for schedule(static , chunk)
    for(i=0;i<n;i++)  
    {  
        for(j=0;j<n;j++)  
        {  
            a[i][j]=0;  
        }  
    }  
    #pragma omp for schedule(static, chunk)
    for(i=0;i<n;i++)  
    {  
        printf("enter the no. of adjacent vertices of vertex %d\n",i+1);       
        scanf("%d",&ver);  
        printf("enter vertices nos. which are adjacent to vertex %d\n",i+1);  
        for(j=0;j<ver;j++)  
        {  
               scanf("%d",&temp);  
               a[i][temp-1]=1;                                                   
        }  
        if(ver%2==1)                                   
            count++;  
    }  }
    printf("the adjacency matrix for the graph is\n");  
    for(i=0;i<n;i++)  
    {  
        for(j=0;j<n;j++)  
        {  
            printf("%4d",a[i][j]);                     
        }  
        printf("\n");  
    } 
    
    if(count==0)                                
        printf("graph has a euler cycle\n");  
    else  
        printf("graph doesnt has a euler cycle\n");  
} 
