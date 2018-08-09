#include<iostream>
#include<bits/stdc++.h>
using namespace std;
struct elements
{
	vector<long long int> node;
	int type;

};
struct nodetoelement
{
	vector<long long int> elem;
	long long int npoint;
};
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
int main()
{
	ifstream input("bigger.su2");
	ofstream output("result.txt");
	long long int num_elements,elem_type,diamension;
	string temp1,temp2;
	input>>temp1>>diamension;
	input>>temp2>>num_elements;
	cout<<diamension<<" "<<num_elements<<endl;
	for(long long int i=0;i<num_elements;i++)
	{

		elements store;
		long long int nodes;
		input>>elem_type;
		int num_nodes=findType(elem_type);
		store.type=elem_type;
		while(num_nodes--)
		{
			input>>nodes;
			store.node.push_back(nodes);
		}
		v.push_back(store);
	}
	for(long long int i=0;i<num_elements;i++)
	{
		cout<<i<<"-->";
		for(long long int j=0;j<v[i].node.size();j++)
		{
			cout<<v[i].node[j]<<" ";

		}
		cout<<"-->";
		cout<<v[i].type;
		cout<<endl;
	}


	long long int npoints;
	input>>temp1>>npoints;






	// vector<long long int> *nodetoelement=new vector<long long int>[npoints];
	// for(int i=0;i<v.size();i++)
	// {
	// 	elements store=v[i];
	// 	for(int j=0;j<store.node.size();j++)
	// 	{
	// 		int temp=store.node[j];
	// 		nodetoelement[temp].push_back(i);
	// 	}
	// }





	vector<nodetoelement> v1(npoints);
	for(int i=0;i<v.size();i++)
	{
		elements e=v[i];
		for(int j=0;j<e.node.size();j++)
		{
			int temp=e.node[j];
			nodetoelement ntoe;
			ntoe.elem.push_back(i);
			v1[temp]=ntoe;
		}

	}

	for(int i=0;i<v1.size();i++)
	{
		nodetoelement e=v1[i];
		cout<<i<<"-->";
		for(int j=0;j<e.elem.size();j++)
		{
			cout<<e.elem[j]<<" ";
		}
		cout<<endl;
	}







	// for(long long int i=0;i<npoints;i++)
	// {
	// 	cout<<i<<"-->";
	// 	for(long long int j=0;j<nodetoelement[i].size();j++)
	// 	{
	// 		cout<<nodetoelement[i][j]<<" ";

	// 	}
	// 	cout<<endl;
	// }







	return 0;

}