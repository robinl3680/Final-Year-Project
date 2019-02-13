#include<mpi.h>
#include<iostream>
#include<bits/stdc++.h>
#include<string>
#include<string.h>
#include<fstream>
#include<sstream>


#define MAX 	  100000
#define CORES 	  4
#define ADJ_MAX   10
#define GHOST_MAX 100000
#define G_MAX 100

using namespace std;

//Structure used for storing vertex.
struct vertex
{
	int vertex_no;
	int data;

	int adj[ADJ_MAX];
	int adj_size;

};





int main(int args, char **argv)
{
	int rank, size;

	MPI_Init(&args,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int partition[MAX+1];

	ifstream partition_file(argv[1]); //Accessing partitioned graph.
	ifstream graph_file(argv[2]);	//Accessing graph file.


	//Define MPI type for struct.

	const int nitems      = 4;
    int blocklengths[4]   = {1,1,ADJ_MAX+1,1};
    MPI_Datatype types[4] = {MPI_INT, MPI_INT , MPI_INT , MPI_INT};
    MPI_Datatype mpi_vertex_type;
    MPI_Aint     offsets[4];

    offsets[0] = offsetof(vertex, vertex_no);
    offsets[1] = offsetof(vertex, data);
    offsets[2] = offsetof(vertex, adj);
    offsets[3] = offsetof(vertex, adj_size);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_vertex_type);
    MPI_Type_commit(&mpi_vertex_type);

    // Core zero doing the task of reading the partition and adjacency.
	
		int v,e;
		string gph,part;

		getline(graph_file,gph); //Line by line reading graph.
		
		sscanf(&gph[0],"%d%d",&v,&e);
		// cout << v << " " << e << endl; //Vertices and edges.

		vector<int> vertex_data(v+1); //To store data in each vertex.


		vector<vertex> sub_graph;

		map<int,vector<int>> sub;

		map<int,vector<int>> to_peer;

		set<int> ghost;

		map<int,int> data_map;


		for(int i = 1 ; i <= v ; i++)
		{
			int temp1;
			int temp2;
			
			getline(graph_file,gph); //Line by line reading graph.
			getline(partition_file,part); //Line by line reading partition.

			stringstream partss(part);
			partss >> temp1;			//String stream for accessing integer data in the line

			partition[i] = temp1;	//Storing partition information into an array.


			

			stringstream ss(gph);

			

			if(partition[i] == rank)
			{
				
				vertex v_node;
				v_node.vertex_no = i;
				v_node.adj_size = 0;
				v_node.data = i + 5;
				data_map[i] = v_node.data;
				// cout << "am rank " << rank << " have vertex " << i << " have data " << v_node.data << endl;
				while(ss >> temp2) //String stream for accessing each vertex in that line.
				{
					v_node.adj[v_node.adj_size] = temp2;
					v_node.adj_size++;
					sub[v_node.vertex_no].push_back(temp2);					//Storing the adjacency list.
				}
				sub_graph.push_back(v_node);

				//Printing the subgraph peer to peer

			}
		}


		// Map method to find the ghost


		for(auto it:sub)
		{
			for(auto it1 : it.second)
				{
					if(sub.find(it1) == sub.end())
						ghost.insert(it1);
				}
		}


		
		// for(auto it:ghost)
		// {
		// 	cout << " iam " << rank << " ";
		// 	cout << it << " ";
		// }
		// cout << endl;


	













		//to_peer is a map to pack the ghost curresponding to a core together.



		for(auto it : ghost)
		{
			to_peer[partition[it]].push_back(it);
		}

		int sample_arr[G_MAX]; //This array contains packed ghost vertices to a core.

		for(auto it : to_peer)
		{

			

			int send_size = it.second.size();
			sample_arr[0] = send_size;

			int i = 1;

			for(auto it1 : it.second)
			{

				sample_arr[i++] = it1;
			}
			

			MPI_Send(&sample_arr , G_MAX , MPI_INT , it.first , 0 , MPI_COMM_WORLD); //Sending ghost to cores.

		

		}

		
		//All cores receiving the ghosts from others.

			int sample_recv[G_MAX];
			MPI_Status status;
				
			if(rank == 2)
			{
				for(int k = 0; k < 2; k++)
				{			 
					int ierr = MPI_Recv(&sample_recv , G_MAX , MPI_INT , MPI_ANY_SOURCE , 0 , MPI_COMM_WORLD , &status);
							
					
								if(ierr == MPI_SUCCESS)
								{
											for(int j = 1 ; j <= sample_recv[0]; j++)
											{
												// cout << "I am " << rank << " received ";
												// cout << sample_recv[j] << " from core " << status.MPI_SOURCE << " as ghost ";
												sample_recv[j] = data_map[sample_recv[j]];
											}
											// cout <<  endl;
											MPI_Send(&sample_recv , G_MAX , MPI_INT , status.MPI_SOURCE , 0 , MPI_COMM_WORLD);
								}
				}
			}
			else
			{
				int ierr = MPI_Recv(&sample_recv , G_MAX , MPI_INT , MPI_ANY_SOURCE , 0 , MPI_COMM_WORLD , &status);
							
					
								if(ierr == MPI_SUCCESS)
								{
											for(int j = 1 ; j <= sample_recv[0]; j++)
											{
												// cout << "I am " << rank << " received ";
												// cout << sample_recv[j] << " from core " << status.MPI_SOURCE << " as ghost ";
												sample_recv[j] = data_map[sample_recv[j]];
											}
											// cout <<  endl;
											MPI_Send(&sample_recv , G_MAX , MPI_INT , status.MPI_SOURCE , 0 , MPI_COMM_WORLD);
								}
			}
			
		

			int data_recv[G_MAX];
			MPI_Status stat;

			for(int k = 0; k < ghost.size(); k++)
			{	
			
					 	int ie = MPI_Recv(&data_recv , G_MAX , MPI_INT , MPI_ANY_SOURCE , 0 , MPI_COMM_WORLD , &stat);
			
						if(ie == MPI_SUCCESS)
						{
									for(int j = 1 ; j <= data_recv[0]; j++)
									{
										cout << "I am " << rank << " received ";
										cout << data_recv[j] << " from core " << stat.MPI_SOURCE << " as data of ghost";
									}
									cout <<  endl;
						}
			}

			

	MPI_Finalize();
	return 0;
}