#include<mpi.h>
#include<iostream>
#include<bits/stdc++.h>
#include<string>
#include<string.h>
#include<fstream>
#include<sstream>


#define MAX 	  100000
#define CORES 	  3
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
	int adj_loc[ADJ_MAX];
	int adj_size;

};





int main(int args, char **argv)
{
	int rank, size;

	MPI_Init(&args,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	

	int send_buffer[CORES];


	for(int i = 0; i < CORES; i++)
	{
		send_buffer[i] = 0;
	}



	int recv_buffer[CORES * CORES];

	for(int i = 0; i < CORES*CORES; i++)
	{
		recv_buffer[i] = 0;
	}



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
				
				while(ss >> temp2) //String stream for accessing each vertex in that line.
				{
					v_node.adj[v_node.adj_size] = temp2;
					v_node.adj_size++;
					sub[v_node.vertex_no].push_back(temp2);					//Storing the adjacency list.
				}
				sub_graph.push_back(v_node);
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














		// Creating local indexing for the vertices.


		map <int,int> local_Index;

        		int c = 0,r = 0;

                for(int i = 0; i < sub_graph.size(); i++)
                {
                        for(int j = 0; j < sub_graph[i].adj_size; j++,c++)
                        {
                                int a = sub_graph[i].adj[j];
                                local_Index[a] = c;

                        }
                }

        // cout<<"local indexing of each vertex in the sub graph is"<<endl;


        // Filling the local indexing
        for(int i = 0; i < sub_graph.size(); i++)
        {
        	for(int j = 0; j < sub_graph[i].adj_size; j++)
        	{
        		int d = sub_graph[i].adj[j];
        		sub_graph[i].adj_loc[j] = local_Index.find(d)->second;

        		// cout << sub_graph[i].adj[j] << ":" << sub_graph[i].adj_loc[j] << " ";
        	}
        	// cout << endl;

        }














		//to_peer is a map to pack the ghost curresponding to a core together.

        

		for(auto it : ghost)
		{
			to_peer[partition[it]].push_back(it); //Storing the partition as key and ghost to that partiton as data.


		}
		for(auto it : to_peer)
		{
			send_buffer[it.first] = it.second.size();
		}
		


		//Creating synchornization message to pass to all cores about size of ghost.

		int synch[CORES][CORES];
		int tmp = 0 , k = 0;
		MPI_Allgather(&send_buffer,CORES,MPI_INT,&recv_buffer,CORES,MPI_INT,MPI_COMM_WORLD);

		// cout << " Am rank " << rank << "	 ";

		for(int i = 0 ; i < CORES * CORES; i++)
		{
			if(i % CORES == 0 && i != 0)
			{
					// cout << endl;

					tmp++;
					k = 0;
			}
			synch[tmp][k] = recv_buffer[i];
			k++;

			// cout << recv_buffer[i] << " ";
		
		}

		// cout << endl;
		
		for(int i = 0 ; i < CORES; i++)
		{

			if(synch[rank][i] != 0)
			{
				int m = 0;
				MPI_Status status;
				int sample_arr[synch[rank][i]];
				for(auto it1 : to_peer[i])
				{
					sample_arr[m++] = it1; 
				}

				// cout << "am rank " << rank << " to core " << i << " 	";
				// for(auto it : sample_arr)
				// {
				// 	cout << it << " ";
				// }
				// cout << endl;

				MPI_Send(&sample_arr, synch[rank][i], MPI_INT, i, 0, MPI_COMM_WORLD);
			
			}
		}

		// cout << endl;

		for(int i = 0; i < CORES; i++)
		{
			if(synch[i][rank] != 0)
			{
				int data_buff[synch[i][rank]];
				MPI_Status status;
				int ierr = MPI_Recv(&data_buff, synch[i][rank], MPI_INT, i, 0, MPI_COMM_WORLD, &status);
				if(ierr == MPI_SUCCESS)
				{
					for(int j = 0; j < synch[i][rank]; j++)
					{
						data_buff[j] = data_map[data_buff[j]];
					}
					MPI_Send(&data_buff, synch[i][rank], MPI_INT, i, 0, MPI_COMM_WORLD);
				}

			}
		}

		for(int i = 0; i < CORES ; i++)
		{
			if(synch[i][rank] != 0)
			{
				int sample_recv[synch[i][rank]];
				MPI_Status status;
				int ierr = MPI_Recv(&sample_recv, synch[rank][i], MPI_INT, i, 0, MPI_COMM_WORLD, &status);
				if(ierr == MPI_SUCCESS)
				{
					cout << "am rank " << rank << " have ";
					for(auto it : sample_recv)
					{
						cout << it << " ";
					}
					cout << endl;
				}
			}
		}




	MPI_Finalize();
	return 0;
}