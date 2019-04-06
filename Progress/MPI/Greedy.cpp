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

using namespace std;



//Search function to check whether the synchornization matrix have all 0 entries.

int sear(int synch[CORES][CORES])
{
	for(int i = 0; i < CORES; i++)
	{
		for(int j = 0; j < CORES; j++)
		{
			if(synch[i][j] != 0)
				return 1;
		}
	}
	return 0;
}






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






	int count_all = 0;
	int total_count[CORES*CORES]; // It is used for taking count of communication from all cores.

	for(int i = 0; i < CORES * CORES ; i++)
		total_count[i] = 0;
	

	int send_buffer[CORES]; 	// This buffer used to send the row which contain information about amount of ghost it has to received from other cores.



	for(int i = 0; i < CORES; i++)
	{
		send_buffer[i] = 0;
	}



	int recv_buffer[CORES * CORES]; //This buffer is used to collect send_buffer information from all cores.

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

	
		int v,e;
		string gph,part;

		getline(graph_file,gph); //Line by line reading graph.
		
		sscanf(&gph[0],"%d%d",&v,&e); //Number of vertices and number of edges.

		vector<int> vertex_data(v+1); //To store data in each vertex.


		vector<vertex> sub_graph; //Used to store the subgraph.

		map<int,vector<int>> sub; //Map used for the subgraph.

		map<int,vector<int>> to_peer; //Contains ghost that has to send to other cores key as core number and a list containing the ghosts.

		set<int> ghost; //Used to find the ghost.

		map<int,int> data_map; //Used to store the data of each vertex.


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
				v_node.data = i + 5; //Vertex data is a constant

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
					if(sub.find(it1) == sub.end()) //If an element in the adjacency list does not contained in the list of vetices for that core add to ghost
						ghost.insert(it1);
				}
		}










		// Creating local indexing for the vertices which make easy to access the adjacent vertex in constant time.


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


        // Filling the local indexing
        for(int i = 0; i < sub_graph.size(); i++)
        {
        	for(int j = 0; j < sub_graph[i].adj_size; j++)
        	{
        		int d = sub_graph[i].adj[j];
        		sub_graph[i].adj_loc[j] = local_Index.find(d)->second;
        	}

        }




		//to_peer is a map to pack the ghost curresponding to a core together.

        

		for(auto it : ghost)
		{
			to_peer[partition[it]].push_back(it); //Storing the partition as key and ghost to that partiton as data.


		}
		for(auto it : to_peer)
		{
			send_buffer[it.first] = it.second.size(); //Filling the send_buffer.
		}
		


		//Creating synchornization message to pass to all cores about size of ghost.

		int synch[CORES][CORES];	//Matrix used to store the received synchornization information from all cores.
		int synch_dup[CORES][CORES];	//Duplicating the synch matrix.
		int tmp = 0 , k = 0;
		MPI_Allgather(&send_buffer,CORES,MPI_INT,&recv_buffer,CORES,MPI_INT,MPI_COMM_WORLD);

		for(int i = 0 ; i < CORES * CORES; i++)
		{
			if(i % CORES == 0 && i != 0)
			{
					tmp++;
					k = 0;
			}
			synch[tmp][k] = recv_buffer[i];	//Filling out the synch matrix.
			synch_dup[tmp][k] = recv_buffer[i];
			k++;
		
		}



	// This section used for finding the largest size in the matrix using a greedy technique.
	// After one complete iteration i,j entries are pushed into vector of pairs 'v'.
	// Then the complete vector is stored in vector 'session'. It contain complete sessions.

	// This is a one time computation so does not need much optimization.

	vector<vector<pair<int,int>>> session;
	int pos_i,pos_j;

	while(sear(synch))
	{
		unordered_set<int> j_set; // Map used to store current largest element's i value.
		unordered_set<int> i_set; // Map used to store current largest element's j value.
		vector<pair<int,int>> v;
		for(int m = 0; m < CORES; m++)
		{
			int curr = 0;
			
			for(int i = 0; i < CORES; i++)
			{
				for(int j = 0; j < CORES; j++)
				{
					if(synch[i][j] > curr && i_set.find(i) == i_set.end() && j_set.find(j) == j_set.end() )
					{
						curr = synch[i][j];
						pos_i = i;
						pos_j = j;

					}
				}
			}
			if(synch[pos_i][pos_j] != 0)
				{
					synch[pos_i][pos_j] = 0;
					i_set.insert(pos_i);
					j_set.insert(pos_j);
					v.push_back({pos_i,pos_j});
				}
		}

		session.push_back(v);
	}


	//Communication section. This is a greedy approach which utilizes the session information stored in 'session' vector
	//Inner for loop iterates for each 'i'  'j' in session and outer for loop iterates on sessions.

	for(auto it : session)
	{
		for(auto it1 : it)
		{
			if(rank == it1.first)
			{
				auto it3 = to_peer[it1.second];
				int sz = it3.size();
				int send_arr[sz];
				int k = 0;
				for(auto it2 : it3)
				{
					send_arr[k++] = it2;
				}
				
				MPI_Send(&send_arr,sz,MPI_INT,it1.second,0,MPI_COMM_WORLD);
			}
			if(rank == it1.second)
			{
				int recv_arr[synch_dup[it1.first][it1.second]];
				MPI_Status status;
				int ierr = MPI_Recv(&recv_arr,synch_dup[it1.first][it1.second],MPI_INT,it1.first,0,MPI_COMM_WORLD,&status);
				if(ierr == MPI_SUCCESS)
				{
					
					for(int i = 0; i < synch_dup[it1.first][it1.second]; i++)
						{
							recv_arr[i] = data_map[recv_arr[i]];
						}
					MPI_Send(&recv_arr,synch_dup[it1.first][it1.second],MPI_INT,it1.first,0,MPI_COMM_WORLD);
					
				}		
			}
			if(rank == it1.first)
			{
				int recv_data[synch_dup[it1.first][it1.second]];
				MPI_Status status;
				int ierr = MPI_Recv(&recv_data,synch_dup[it1.first][it1.second],MPI_INT,it1.second,0,MPI_COMM_WORLD,&status);
				if(ierr == MPI_SUCCESS)
				{
					cout << "I am " << rank << " have ";
									for(auto it : recv_data)
									{
										cout << it << " ";
									}
									cout <<  endl;	
				}	

			}
		}
	}






	MPI_Finalize();
	return 0;
}