
/* This is an mpi program for transfer a data from process 0 to process 1 */



#include<iostream>
#include<mpi.h>
using namespace std;
int main()
{
	int pid,pcount;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&pid); //intializing 
	MPI_Comm_size(MPI_COMM_WORLD,&pcount);
	
	cout<<"Hello am procees "<<pid<<" out of "<<pcount<<endl;
	if(pcount!=2)
	{
		cout<<"Error in sending"<<endl;
		return MPI_Abort(MPI_COMM_WORLD,1); //if process count is not 2 then print error
		
	}
	if(pid==0)
	{
		double pi=2.34;
		MPI_Send(&pi,1,MPI_DOUBLE,1,0,MPI_COMM_WORLD);
		cout<<"Process "<<pid<<" send value "<<pi<<" to process"<<endl; //process 0 sending data to process 1
	}
	else
	{
		double val;
		int ierr;
		MPI_Status status;
		ierr=MPI_Recv(&val,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
		if(ierr==MPI_SUCCESS)
		{
			cout<<"Proces "<<pid<<" received "<<val<<endl; //process 1 receiving data
		}
		else
		{
			cout<<"Not success"<<endl; //if any failure
		}
	}
	MPI_Finalize();
	return 0;
}