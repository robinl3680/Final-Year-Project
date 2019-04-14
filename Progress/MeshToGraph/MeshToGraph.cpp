#include<iostream>
#include<bits/stdc++.h>
using namespace std;

struct elements	//Structure for the elements
{
	vector<long long int> node;
	int type;

};


void makeFace( vector< set<long long int> > &f , elements &e){ 
//Function that make faces for each element
	// cout << "hai" << endl;
	switch( e.type ){


		case 10	:	for(int i = 0 ; i < 4 ; i++){

						set<long long int> n;

						n.insert(e.node[i]);
						n.insert(e.node[(i+1)%4]);
						n.insert(e.node[(i+2)%4]);
						f.push_back(n);

					} 
					break;
		case 11 :	 {
						set<long long int> s1 = {e.node[0],e.node[1],e.node[3],e.node[2]};
						set<long long int> s2 = {e.node[1],e.node[5],e.node[7],e.node[3]};
						set<long long int> s3 = {e.node[2],e.node[3],e.node[7],e.node[6]};
						set<long long int> s4 = {e.node[4],e.node[5],e.node[7],e.node[6]};
						set<long long int> s5 = {e.node[0],e.node[2],e.node[6],e.node[4]};
						set<long long int> s6 = {e.node[0],e.node[1],e.node[5],e.node[4]};
						f.push_back(s1);
						f.push_back(s2);
						f.push_back(s3);
						f.push_back(s4);
						f.push_back(s5);
						f.push_back(s6);

					 }
					 break;
		case 13:	{
						set<long long int> s1 = {e.node[0],e.node[1],e.node[4],e.node[3]};
						set<long long int> s2 = {e.node[1],e.node[2],e.node[0]};
						set<long long int> s3 = {e.node[1],e.node[4],e.node[5],e.node[2]};
						set<long long int> s4 = {e.node[0],e.node[3],e.node[5],e.node[2]};
						set<long long int> s5 = {e.node[3],e.node[4],e.node[5]};
						f.push_back(s1);
						f.push_back(s2);
						f.push_back(s3);
						f.push_back(s4);
						f.push_back(s5);

					}
					break;

		case 14 : {
						set<long long int> s1 = {e.node[0],e.node[1],e.node[2],e.node[3]};
						set<long long int> s2 = {e.node[1],e.node[2],e.node[4]};
						set<long long int> s3 = {e.node[0],e.node[3],e.node[4]};
						set<long long int> s4 = {e.node[0],e.node[1],e.node[4]};
						set<long long int> s5 = {e.node[2],e.node[3],e.node[4]};
						f.push_back(s1);
						f.push_back(s2);
						f.push_back(s3);
						f.push_back(s4);
						f.push_back(s5);
				  }	
				  break;			



	}
}


int findType(int element_type)
{
	//function return no of vertices of an element according to element

	switch(element_type)
	{
		case 3: return 2;
		case 5: return 3;
		case 9: return 4;
		case 10:return 4;
		case 11:return 8;
		case 13:return 6;
		case 14:return 5;
		default: return 0;
	}
}
vector<elements> v;
int main(int argv, char **args)
{
	if(argv != 4)
	{
		cout << " Enter input file as first argument, graph file as second argument and face file as third argument" << endl;
		cout << "Argument missing" << endl;
		exit(0);
	}
	ifstream input(args[1]);
	ofstream output(args[2]);
	ofstream out(args[3]);
	long long int num_elements,elem_type,diamension;
	string temp1,temp2;
	input >> temp1 >> diamension;
	input >> temp2 >> num_elements;
	cout << diamension << " " << num_elements << endl;

	// cout << "hai dear" << endl;

	for(long long int i=0; i < num_elements; i++)
	{
	// For storing nodes of each element
		elements store;
		long long int nodes;
		input >> elem_type;
		int num_nodes = findType(elem_type);
		store.type = elem_type;
		while(num_nodes--)
		{
			input >> nodes;
			// nodes += 1;
			store.node.push_back(nodes);
		}
		v.push_back(store);
	}


	map< set<long long int> , int> faceID; //Map to store faces and curresponding index in edges array(indexing start from 1).
	vector< pair<int , int> > edges(1 , {-1,-1}); // To store two elements of each face.

	int faceCount = 1;
	int edgeCount = 0;

	for(int i = 0 ; i < v.size() ; i++){

		elements e = v[i];

		//cout << i << endl;

		vector< set<long long int> > f; //vector that store faces as set of nodes.

		makeFace(f , e); //returns faces that makes the current element.

		for(auto &it : f){


			// set<long long int> temp = it;
			// for(auto it1 : temp)
			// 	cout << it1 << " ";
			// cout << endl;



			if(faceID[it] == 0){ //Check for the first occurence of a face.

				faceID[it] = faceCount++; //store index in map and increment size of 'edges'
				edges.push_back( {i,-1}); // add edge to array
			}

			else{ //second occurance of face 'it' . It will be present at index faceID[it] in edges array.
				edgeCount++;
				edges[ faceID[it] ].second = i; //fill second element of face 'it'
			}

		}
	}
	output<<v.size()<<" "; //storing number of vertiices to ouput file.
	output<<edgeCount<<endl; //storing number of edges to ouput file.

	vector< vector<int> > adjList(v.size()); //making adjacency list.

	int k = 1;	//face count
	for(auto &it : edges){

		// cout << k++ << " : " << it.first << " " << it.second << endl;
		if(it.second != -1 )
			out << k++ << " : " << it.first << " " << it.second << endl; //storing elements curresponding to a face int file 'face'.

		if(it.second == -1) //if any one of the element of a face is -1 avoid it from adjacency list.
			continue;
		adjList[it.first].push_back(it.second); //making adjacency list
		adjList[it.second].push_back(it.first);
	}

	for( int i = 0 ; i < v.size() ; i++){ //storing adjacency list to file named 'result'.

		for(auto it : adjList[i])
			output << it << " ";

		output << endl;
	}



	return 0;

}
