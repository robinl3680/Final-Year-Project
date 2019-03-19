#include<iostream>
#include<bits/stdc++.h>
using namespace std;

struct elements	//Structure for the elements
{
	vector<long long int> node;
	int type;

};


void makeFace( vector< set<int> > &f , elements &e){ 
//Function that make faces for each element

	switch( e.type ){

		case 10	:	for(int i = 0 ; i < 4 ; i++){

						set<int> n;

						n.insert(e.node[i]);
						n.insert(e.node[(i+1)%4]);
						n.insert(e.node[(i+2)%4]);
						f.push_back(n);

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
		case 12:return 8;
		case 13:return 5;
		case 14:return 5;
		default: return 0;
	}
}
vector<elements> v;
int main(int argv, char **args)
{
	ifstream input(args[1]);
	ofstream output("result.txt");
	ofstream out("faces.txt");
	long long int num_elements,elem_type,diamension;
	string temp1,temp2;
	input>>temp1>>diamension;
	input>>temp2>>num_elements;
	cout<<diamension<<" "<<num_elements<<endl;
	for(long long int i=0;i<num_elements;i++)
	{
	// For storing nodes of each element
		elements store;
		long long int nodes;
		input>>elem_type;
		int num_nodes=findType(elem_type);
		store.type=elem_type;
		while(num_nodes--)
		{
			input>>nodes;
			nodes+=1;
			store.node.push_back(nodes);
		}
		v.push_back(store);
	}


	// Printing element to node mapping


	// for(long long int i=0;i<num_elements;i++)
	// {
	// 	cout<<i<<"-->";
	// 	for(long long int j=0;j<v[i].node.size();j++)
	// 	{
	// 		cout<<v[i].node[j]<<" ";

	// 	}
	// 	cout<<"-->";
	// 	cout<<v[i].type;
	// 	cout<<endl;
	// }


	// long long int npoints;
	// input>>temp1>>npoints;





	// vector<set<long long int> > nodetoelement(npoints); //Vector for node to element mapping
	// for(int i=0;i<v.size();i++)
	// {
	// 	elements store=v[i];
	// 	for(int j=0;j<store.node.size();j++)
	// 	{
	// 		int temp=store.node[j];
	// 		nodetoelement[temp].insert(i);
	// 	}
	// }

	//printing node to element mapping

	// for(int i = 0 ; i < nodetoelement.size() ; i++){

	// 	cout << i << " : ";

	// 	for(auto it : nodetoelement[i])
	// 		cout << it << " ";

	// 	cout << endl;
	// }

	map< set<int> , int> faceID; //Map to store faces and curresponding index in edges array(indexing start from 1).
	vector< pair<int , int> > edges(1 , {-1,-1}); // To store two elements of each face.

	int faceCount = 1;
	int edgeCount = 0;

	for(int i = 0 ; i < v.size() ; i++){

		elements e = v[i];

		//cout << i << endl;

		vector< set<int> > f; //vector that store faces as set of nodes.

		makeFace(f , e); //returns faces that makes the current element.

		for(auto &it : f){

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
		if(it.first != -1)
			out << k++ << " : " << it.first << " " << it.second << endl; //storing elements curresponding to a face int file 'face'.

		if(it.second == -1) //if any one of the element of a face is -1 avoid it from adjacency list.
			continue;
		adjList[it.first].push_back(it.second); //making adjacency list
		adjList[it.second].push_back(it.first);
	}

	for( int i = 0 ; i < v.size() ; i++){ //storing adjacency list to file named 'result'.

		// output << i <<" ";

		for(auto it : adjList[i])
			output << it << " ";

		output << endl;
	}





















































	// vector<nodetoelement> v1(npoints);
	// for(int i=0;i<v.size();i++)
	// {
	// 	elements e=v[i];
	// 	for(int j=0;j<e.node.size();j++)
	// 	{
	// 		int temp=e.node[j];
	// 		nodetoelement ntoe;
	// 		ntoe.elem.push_back(i);
	// 		v1[temp]=ntoe;
	// 	}

	// }

	// for(int i=0;i<v;i++)
	// {
	// 	int node1=nodetoelement
	// }

	// for(int i=0;i<v1.size();i++)
	// {
	// 	nodetoelement e=v1[i];
	// 	cout<<i<<"-->";
	// 	for(int j=0;j<e.elem.size();j++)
	// 	{
	// 		cout<<e.elem[j]<<" ";
	// 	}
	// 	cout<<endl;
	// }









	return 0;

}
