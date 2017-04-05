//Mean Shift Algorithm implementation...
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

string ipFilePath;

struct Coordinates
{
	float x;
	float y;
	float z;
	int bit;
};

void addCoord(vector<Coordinates> &vtNew, float a, float b, float c)
{
	Coordinates points;

	points.x = a; 
	points.y = b; 
	points.z = c; 
	points.bit = 0;	//Ititially keeping 0 as the bit...
	vtNew.push_back(points);
}

void readIpFile(vector<Coordinates> &vtNew)
{
	float a, b, c;
	char delim;
	cout << "The input file path in readIpFile function = " << ipFilePath << endl;
	
	ifstream infile(ipFilePath);
	
	while (infile >> a >> delim >> b >> delim >> c)
	{
		addCoord(vtNew, a, b, c);
	}
}

void displayCoord(vector<Coordinates> vtNew)
{
	vector<Coordinates> ::iterator it;
	for (it = vtNew.begin(); it != vtNew.end(); it++)
	{
		cout << "x = " << it->x;
		cout << "y = " << it->y;
		cout << "z = " << it->z;
		cout << "bit = " << it->bit<<endl;
	}
}

int main(int argc, char **argv)
{
	//TODO: Input validations..

	ipFilePath = argv[1];
	cout << "The input file path = " << argv[1] << endl;
	
	//variable declarations...
	vector<Coordinates> vt;
	
	//Function calls
	readIpFile(vt);
	displayCoord(vt);

	return 0;
}