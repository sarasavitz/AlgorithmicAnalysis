/*======================================================== 
COT4400: Analysis of Algorithms - Spring 2017
Project 2: Dynamic Programming
Group 6: Tiffany Jackson, Marilyn Salvatierra, Sara Savitz
========================================================*/
#include <iostream>														
#include <cstdlib>	
#include <fstream>													 
																																																						
using namespace std;

//---Declarations---//
unsigned long long** chocBar; // 2D Array
unsigned long long Chomp(unsigned long long row, unsigned long long col); // Iterative DP Function

/*=================================================
Helper Function: Initializes 2D Array to store data
Calls iterative DP function Chomp
=================================================*/ 
int main()
{
	int num;
	unsigned long long row, col;		  // Dimensions of 2D Array 
	unsigned long long smallChomp = 0; 	 // Count of states with exactly r rows and c columns
	unsigned long long totalChomp = 0;	// Count of states bounded by r rows and c columns
	
	ifstream inFile;
	ofstream outFile;

	inFile.open("input.txt");
	outFile.open("output.txt");

	// Get number of problem instances
	inFile >> num;
	
	// Iterate through each problem instance
	for(int i = 0; i < num; ++i)
	{
		// Get number of rows
		inFile >> row;
		// Get number of columns
		inFile >> col;

		//---Constructing---//
		chocBar = new unsigned long long*[row];

		for(unsigned long long i = 0; i < row; ++i)
			chocBar[i] = new unsigned long long[col];

		//---Initializing---// 
		for(unsigned long long i = 0; i < row; ++i) 
		{
			for(unsigned long long j = 0; j < col; ++j)
			{
				chocBar[i][j] = 0;
			}
		}	

		//---Calling Function---// 
		smallChomp = Chomp(row - 1 , col - 1); 
		totalChomp = Chomp(row, col);

		outFile << smallChomp << " " << totalChomp << endl;

		//---Destructing---//
		for(unsigned long long i = 0; i < row; ++i)
		{
			delete [] chocBar[i];
		}

		delete [] chocBar;
	} 

	inFile.close();
	outFile.close();

	return 0;
}

// Iterative Dynamic Programming Function
/*=================================================================================
Calculates the total number of states a chocolate bar can have in the game of Chomp
Input: row, col: number of rows and columns in a bar
Output: number of possible states 
=================================================================================*/
unsigned long long Chomp(unsigned long long row, unsigned long long col)
{	
	for(unsigned long long i = 0; i < row; ++i)
	{
		for(unsigned long long j = 0; j < col; ++j)
		{	
			//---Base Case---//
			if(i == 0 && j == 0)
			{
				chocBar[i][j] = 2;	
			}
			//---Edge Case 1---//
			else if(i == 0)
			{
				chocBar[i][j] = (chocBar[i][j-1]) + 1; // No 'UP' Dependency
			}
			//---Edge Case 2---//	
			else if(j == 0)
			{
				chocBar[i][j] = (chocBar[i-1][j]) + 1; 	// No 'LEFT Dependency
			}
			//---Final Case---//				
			else
			{
				chocBar[i][j] = chocBar[i-1][j] + chocBar[i][j-1]; // Sum 'UP' and 'LEFT' Dependencies
			}
		}
	}

	return chocBar[row-1][col-1];
}


