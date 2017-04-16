//Mean Shift Algorithm implementation...
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <chrono>
#include <iomanip>

//Also known as Kernel..
//2 for Dataset 1
//4.5 for Dataset 2
//#define SQUARE_DIM 2
float SQUARE_DIM;
int ROUNDOFF;
#define OUTPUT_FILE_NAME "centers.out"

using namespace std;

string ipFilePath;

struct Coordinates
{
	float x;
	float y;
	float z;
};

vector<Coordinates> vtCenters;

float roundOff(float inp)
{
	//To keep only 3 values after . 
	return(floor(inp * ROUNDOFF) / ROUNDOFF);
}

void addCoord(vector<Coordinates> &vt, float a, float b, float c)
{
	Coordinates points;

	points.x = a; 
	points.y = b; 
	points.z = c; 
	vt.push_back(points);
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
		cout << "x = " << setprecision(8) << it->x;
		cout << ", y = " << setprecision(8) << it->y;
		cout << ", z = " << setprecision(8) << it->z << endl;
	}
}

void averageFunc(vector<Coordinates> vtInRange, vector<Coordinates> &vtNew, vector<Coordinates> &vtTemp)
{
	int flag = 0;
	float mX = 0., mY = 0., mZ = 0.;

	vector<Coordinates> ::iterator it, it2;
	for (it = vtInRange.begin(); it != vtInRange.end(); it++)
	{		
		mX += it->x;
		mY += it->y;
		mZ += it->z;
	}
	mX /= vtInRange.size();
	mY /= vtInRange.size();
	mZ /= vtInRange.size();

	//Add the mean to the existing points list...
	for (it2 = vtNew.begin(); it2 != vtNew.end(); it2++)
	{
		if ((roundOff(it2->x) == roundOff(mX)) && (roundOff(it2->y) == roundOff(mY)) && (roundOff(it2->z) == roundOff(mZ)))
		{
			flag = 1;
			break;
		}
	}
	if (flag == 0)
	{
		if (!(mX != mX) && !(mY != mY) && !(mZ != mZ))	//To filter out NaN values
		{
			//TODO: Add only unique elements...
			addCoord(vtTemp, mX, mY, mZ);			
			addCoord(vtCenters, mX, mY, mZ);
		}
	}
}

void addTempToMainVector(vector<Coordinates> &vtNew, vector<Coordinates> vtTemp)
{	
	//removeDuplicates(vtTemp);
	vtNew.insert(vtNew.end(), vtTemp.begin(), vtTemp.end());
}

void msAlgo(vector<Coordinates> &vtNew)
{
	//New variable declarations...
	vector<Coordinates> vtInRange;
	vector<Coordinates> vtTemp;
	vector<Coordinates> ::iterator it, it2;

	vtInRange.clear();
	vtTemp.clear();

	//For each point in the image, check for all the points inside its surrounding square to find out the mean.
	for (it = vtNew.begin(); it != vtNew.end(); it++)
	{
		float startPntX = ((it->x) - SQUARE_DIM);
		float startPntY = ((it->y) + SQUARE_DIM);
		float endPntX = ((it->x) + SQUARE_DIM);
		float endPntY = ((it->y) - SQUARE_DIM);
		
		//Traversing each entry of the points vector to see if there is a match (If the point is present)...
		for (it2 = vtNew.begin(); it2 != vtNew.end(); it2++)
		{
				if (((it2->x) >= startPntX) && ((it2->x) <= endPntX) && ((it2->y) <= startPntY) && ((it2->y) >= endPntY))
				{
					//Adding the the element in the range to the vtInRange vector, for average calculation...
					//NOTE: these are the initial points...							
					addCoord(vtInRange, it2->x, it2->y, it2->z);
				}
		}
		averageFunc(vtInRange, vtNew, vtTemp);
		vtInRange.clear();
	}
	addTempToMainVector(vtNew, vtTemp);
}

//If return = 0, then not equal
//If return = 1, then equal
int areVecEqual(vector<Coordinates> &vtNew, vector<Coordinates> &vtUpdated)
{
	int iRet = 1;
	vector<Coordinates> ::iterator it, it2;

	if (vtNew.size() != vtUpdated.size())
	{
		return 0;
	}
	else
	{
		it2 = vtUpdated.begin();
		for (it = vtNew.begin(); it != vtNew.end(); )
		{
			if ((it->x != it2->x) || (it->y != it2->y) || (it->z != it2->z))
			{
				return 0;
			}
			it++;
			it2++;
		}
	}

	return iRet;
}

void writeToFile()
{
	vector<Coordinates> ::iterator it;
	ofstream myfile;
	
	float oldX = 0., oldY = 0., oldZ = 0.;
	int ittr = 0;

	myfile.open(OUTPUT_FILE_NAME);
	
	for (it = vtCenters.begin(); it != vtCenters.end(); it++, ittr++)
	{		
		//ittr is for filtering the input files...
		//oldX, oldY, and oldZ are for removing duplicate entries...
		if ((roundOff(oldX) != roundOff(it->x)) || (roundOff(oldY) != roundOff(it->y)) || (roundOff(oldZ) != roundOff(it->z)))
		{
			myfile << it->x << ", " << it->y << ", " << it->z << "\n";			
		}
		
		oldX = it->x;
		oldY = it->y;
		oldZ = it->z;
	}

	myfile.close();
}

int main(int argc, char **argv)
{
	try
	{
		//TODO: Input validations..

		ipFilePath = argv[1];
		cout << "The input file path = " << argv[1] << endl;

		SQUARE_DIM = atoi(argv[2]);
		cout << "Kernel Dimention = " << SQUARE_DIM << endl;

		ROUNDOFF = atoi(argv[3]);
		cout << "Roundoff value = " << ROUNDOFF << endl;

		//variable declarations...
		vector<Coordinates> vt, vtOld;

		vt.clear();
		vtOld.clear();

		readIpFile(vt);
		int inputSize = vt.size();

		chrono::time_point<chrono::system_clock> begin, end;
		begin = chrono::system_clock::now();

		while (1)
		{
			cout << "##### Algorithm:" << endl;

			//Old vt, before adding in the mean values...This is required for comparision...
			vtOld = vt;

			msAlgo(vt);

			if (1 == areVecEqual(vt, vtOld))
			{
				cout << "Breaking as the points have not changes...." << endl;
				//TODO: Write the points to output file...
				writeToFile();
				break;
			}
			/*else
			{
				displayCoord(vt);
				cout << endl;
			}*/
		}

		end = chrono::system_clock::now();
		chrono::duration<double> totaltime = (end - begin);
		cout << "Time Taken in sec = " << (float)totaltime.count() << std::endl;
	}
	catch (const exception& ex)
	{
		cout << "Exception occured: " << ex.what();
	}

	return 0;
}
