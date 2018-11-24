#include<mpi.h>
#include<iostream>
#include<bits/stdc++.h>
#include<string>
#include<string.h>
#include<fstream>
#include<sstream>


#define MAX 100000

using namespace std;

struct vertex
{
	int vertex_no;
	int data;

	int adj[MAX];
	int adj_size;

};

int dummy(){

	return 1;
}



int main(int args, char **argv)
{
	int rank, size;

	MPI_Init(&args,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int partition[MAX+1];

	ifstream partition_file(argv[1]);
	ifstream graph_file(argv[2]);
	

	//define MPI type for struct

	const int nitems      = 4;
    int blocklengths[4]   = {1,1,MAX+1,1};
    MPI_Datatype types[4] = {MPI_INT, MPI_INT , MPI_INT , MPI_INT};
    MPI_Datatype mpi_vertex_type;
    MPI_Aint     offsets[4];

    offsets[0] = offsetof(vertex, vertex_no);
    offsets[1] = offsetof(vertex, data);
    offsets[2] = offsetof(vertex, adj);
    offsets[3] = offsetof(vertex, adj_size);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_vertex_type);
    MPI_Type_commit(&mpi_vertex_type);

	if(rank == 0)
	{
		int v,e;
		string gph,part;

		getline(graph_file,gph);
		cout << gph << endl;

		sscanf(&gph[0],"%d%d",&v,&e);
		cout << v << " " << e << endl;

		
		for(int i = 1 ; i <= v ; i++)
		{
			int temp1;
			int temp2;
			
			getline(graph_file,gph);
			getline(partition_file,part);

			stringstream partss(part);
			partss >> temp1;

			partition[i] = temp1;

			stringstream ss(gph);

			vertex v_node;

			v_node.vertex_no = i;
			v_node.data = dummy();
			v_node.adj_size = 0;


			while(ss >> temp2)
			{
				v_node.adj[v_node.adj_size] = temp2;
				v_node.adj_size++;
			}
			MPI_Send(&v_node,1,mpi_vertex_type,partition[i]+1,0,MPI_COMM_WORLD);


		}
			
			
	}
	if(rank == 1)
	{

		MPI_Status status;
		vertex v_node;
		
		int ierr = MPI_Recv(&v_node,1,mpi_vertex_type,0,0,MPI_COMM_WORLD,&status);

		if(ierr == MPI_SUCCESS)
		{
			cout << "Am processor " << rank << endl;
			cout << "Vertex number : " << v_node.vertex_no << " Data : " << v_node.data << endl;
			cout << "Adj : ";

			for(int i = 0 ; i < v_node.adj_size ; i++)
				cout << v_node.adj[i] << " ";

			cout << endl;
		}
		 ierr = MPI_Recv(&v_node,1,mpi_vertex_type,0,0,MPI_COMM_WORLD,&status);

		if(ierr == MPI_SUCCESS)
		{
			cout << "Am processor " << rank << endl;
			cout << "vertex number : " << v_node.vertex_no << " Data : " << v_node.data << endl;
			cout << "Adj : ";

			for(int i = 0 ; i < v_node.adj_size ; i++)
				cout << v_node.adj[i] << " ";

			cout << endl;
		}
		

	}
	if(rank == 2)
	{

		MPI_Status status;
		vertex v_node;

		

		int ierr = MPI_Recv(&v_node,1,mpi_vertex_type,0,0,MPI_COMM_WORLD,&status);

		if(ierr == MPI_SUCCESS)
		{
			cout << "Am processor " << rank << endl;
			cout << "Vertex number : " << v_node.vertex_no << " data : " << v_node.data << endl;
			cout << "Adj : ";

			for(int i = 0 ; i < v_node.adj_size ; i++)
				cout << v_node.adj[i] << " ";

			cout << endl;
		}
		 ierr = MPI_Recv(&v_node,1,mpi_vertex_type,0,0,MPI_COMM_WORLD,&status);

		if(ierr == MPI_SUCCESS)
		{
			cout << "Am processor " << rank << endl;
			cout << "vertex number : " << v_node.vertex_no << " data: " << v_node.data << endl;
			cout << "Adj : ";

			for(int i = 0 ; i < v_node.adj_size ; i++)
				cout << v_node.adj[i] << " ";

			cout << endl;
		}

		
	}



	if(rank == 3)
	{

		MPI_Status status;
		vertex v_node;

		int ierr = MPI_Recv(&v_node,1,mpi_vertex_type,0,0,MPI_COMM_WORLD,&status);

		if(ierr == MPI_SUCCESS)
		{
			cout << "Am processor " << rank << endl;
			cout << "Vertex number : " << v_node.vertex_no << " data : " << v_node.data << endl;
			cout << "Adj : ";

			for(int i = 0 ; i < v_node.adj_size ; i++)
				cout << v_node.adj[i] << " ";

			cout << endl;
		}
		 ierr = MPI_Recv(&v_node,1,mpi_vertex_type,0,0,MPI_COMM_WORLD,&status);

		if(ierr == MPI_SUCCESS)
		{
			cout << "Am processor " << rank << endl;
			cout << "vertex number : " << v_node.vertex_no << " data: " << v_node.data << endl;
			cout << "Adj : ";

			for(int i = 0 ; i < v_node.adj_size ; i++)
				cout << v_node.adj[i] << " ";

			cout << endl;
		}


	}
	

	MPI_Finalize();
	return 0;
}