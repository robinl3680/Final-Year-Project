#include<iostream>
#include<bits/stdc++.h>
#include<vector>
#include<unordered_map>
using namespace std;
int c=0;
void compare(vector<long long int> a, vector<long long int> b,vector<long long int > *adj,long long int  v,long long int orgi,long long int neighbour)
{
	cout<<"hai"<<endl;
	int fl=0;
	map<long long int,long long int> m;


	int t = 0;
	vector<long long int>::iterator it;
	for(it = a.begin(); it != a.end(); it++)
		m[(*it)] += 1;
	for(it = b.begin(); it != b.end(); it++)
	{
		if(m[(*it)] > 0)
		{
			t += 1;
			m[(*it)] -= 1;
		}
		if(t >= 2)
		{
							adj[orgi].push_back(neighbour);
							adj[neighbour].push_back(orgi);
							fl=0;
							c++;
							break;
		}
	}










	// for(long long int i=0;i<a.size();i++)
	// {
	// 	for(long long int j=0;j<b.size();j++)
	// 	{
	// 					if(a[i]==b[j])
	// 					{
		
	// 						fl+=1;
	// 					}
						
	// 					if(fl>=2)
	// 					{
	// 						cout<<"hai inner"<<endl;
	// 						adj[orgi].push_back(neighbour);
	// 						adj[neighbour].push_back(orgi);
	// 						fl=0;
	// 						c++;
	// 						break;
							
	// 					}
	// 				}
	// }
			
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
		case 12:return 6;
		default: return 0;
	}
}


int main()
{
	ifstream input("bigger.su2");
	ofstream output("output.txt");
	string st1,st2;
	long long int v,e,d,element_type;
	input>>st1>>d; //reading string and numerical value in first line.
	cout<<"diamension "<<d<<endl;
	input>>st2>>v; //reading second line
	vector<long long int> *mesh=new vector<long long int> [v];
	long long int data;
	vector<long long int> *adj=new vector<long long int> [v];
	output<<v<<" ";

	// cout<<st1;
	input>>element_type; //finding which type of element and its value
	int re=findType(element_type);
	int tp=re;
	int fl=0;
	int n1,n2;

	for(int i=0;i<v;i++)
	{
		adj[i].push_back(0);
	}
	
	for(int i=0;i<v;i++)
	{
		while(re--)
		{
			input>>data; //reading index of vertex of an element
			mesh[i].push_back(data);
		}
		input>>n1; // avoiding last value(element number) and first value from next line(element id)
		re=tp; //reintializing no.of vertices
	}

	// for(int i=0;i<v;i++)    //printing vertices of each element
	// {
	// 	cout<<i<<"->";
	// 	for(int j=0;j<mesh[i].size();j++)
	// 		cout<<mesh[i][j]<<" ";
	// 	cout<<endl;
	// }
	for(long long int i=0;i<v;i++)
	{
		for(long long int j=i+1;j<v;j++)
			{
				
				compare(mesh[i],mesh[j],adj,v,i,j);
			}	
	}

	// int count=0;
	output<<c<<endl;
	for(long long int i=0;i<v;i++)
	{

		//writing to file result
		cout<<i<<"->";
		output<<i<<" ";
		for(long long int j=1;j<adj[i].size();j++)
		{
			cout<<adj[i][j]<<" ";
			long long int k=adj[i][j];
			output<<k<<" ";

		}
		output<<endl;
		cout<<endl;
	}


	return 0;
}