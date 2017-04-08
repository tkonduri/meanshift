//Mean Shift Algorithm implementation...
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//Also known as Kernel..
//2 for Dataset 1
//4.5 for Dataset 2
#define SQUARE_DIM 2

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
		cout << ", y = " << it->y;
		cout << ", z = " << it->z;
		cout << ", bit = " << it->bit<<endl;
	}
}

void averageFunc(vector<Coordinates> vtNew)
{
	float mX = 0., mY = 0., mZ = 0.;
	vector<Coordinates> ::iterator it;
	for (it = vtNew.begin(); it != vtNew.end(); it++)
	{		
		mX += it->x;
		mY += it->y;
		mZ += it->z;
	}
	mX /= vtNew.size();
	mY /= vtNew.size();
	mZ /= vtNew.size();

	cout << mX << "," << mY << "," << mZ;
	//TODO: Add this new point in the main vector...
}

void msAlgo(vector<Coordinates> vtNew)
{
	//New variable declarations...
	vector<Coordinates> vtInRange;
	vector<Coordinates> ::iterator it, it2;

	//For each point in the image, check for all the points inside its surrounding square to find out the mean.
	for (it = vtNew.begin(); it != vtNew.end(); it++)	
	{
		float startPntX = ((it->x) - SQUARE_DIM);
		float startPntY = ((it->y) + SQUARE_DIM);
		float endPntX = ((it->x) + SQUARE_DIM);
		float endPntY = ((it->y) - SQUARE_DIM);
		
		for (float i = startPntY; i >= endPntY; i--)
			for (float j = startPntX; j <= endPntX; j++)
			{				
				//TODO: Check if we need to handle border cases...
				if (i>0 && j>0)
				{
					//Traversing each entry of the points vector to see if there is a match (If the point is present)...
					for (it2 = vtNew.begin(); it2 != vtNew.end(); it2++)
					{						
						if (i == (it2->y) && j == (it2->x))
						{
							//Adding the the element in the range to the vtInRange vector, for average calculation...
							addCoord(vtInRange, j, i, it2->z);
						}
					}					
				}
			}
		averageFunc(vtInRange);
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

	cout << "##### Algorithm:" << endl;
	msAlgo(vt);

	return 0;
}