//
// Author:		James Callahan
// Class:		CS 438
// File:		CS438 Homework 1.cpp
// Assignment:	Homework 1
// Due Date:	1-18-2018
//
// Problem Description:	We are given instructions that must be interpreted for execution by a robot arm.
//		The instructions tell the arm how to move blocks around.
//
//
// Notes: 
//		+The bockOut.txt file may not show up in visual studio, but it will be in the project folder:
//			location: CS438 Homework 1 -> CS438 Homework 1 -> blockOut.txt.
//		+ I will also leave a console outoput.
//		+ blockInAlt.txt is a text file of the data from the homework sheet, blockIn.txt is the one provided on moodle.


// Included Libraries
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <assert.h>
#include <string>
#include <list>

using namespace std;

// Prototypes
void moveAOntoB(int A, int B);
void moveAOverB(int A, int B);
void pileAOntoB(int A, int B);
void pileAOverB(int A, int B);
int findStackContainingBlock(int block);
void returnBlocksUntil(int stack, int block);
void movePile(int stackSource, int stackTarget, int stackSourceBlock);
void openFile(ifstream &infile, string filename);
int getLines(ifstream &in);
void printStacks(ofstream &out);

// Global variables
list<int> *stacksArr;
int numStacks;

// Main
int main()
{
	ifstream in;
	ofstream out;
	string inFilename = "blockIn.txt", outFilename = "blockOut.txt";
	string a, b, c, d;

	openFile(in, inFilename);
	out.open(outFilename);

	// Get the number of stacks
	getline(in, a);
	numStacks = stoi(a);
	
	// initialize stacks array
	stacksArr = new list<int>[numStacks];

	// initialize stacks in array
	for (int i = 0; i < numStacks; i++)
	{
		stacksArr[i].emplace_front(i);
	}

	// Parse the command apart
	while ( !in.eof() )
	{
		// Gets the 4 items for command
		getline(in, a, ' ');
		if (a != "quit")
		{
			getline(in, b, ' ');
			getline(in, c, ' ');
			getline(in, d, '\n');

			if ( (a == "move") && (c == "onto") )
			{
				moveAOntoB(stoi(b), stoi(d));
			}
			else if ( (a == "move") && (c == "over") ) 
			{
				moveAOverB(stoi(b), stoi(d));
			}
			else if ( (a == "pile") && (c == "onto") )
			{
				pileAOntoB(stoi(b), stoi(d));
			}
			else if ( (a == "pile") && (c == "over") )
			{
				pileAOverB(stoi(b), stoi(d));
			}
		}

	}

	// Print the output file and text to the terminal
	printStacks(out);
	system("pause");
	
	// Close files
	in.close();
	out.close();

    return 0;
}

// move A onto B
// puts A on B after returning any blocks stacked up to their initial positions
void moveAOntoB(int A, int B)
{
	// Get the stacks containing the blocks A and B
	int stackA = findStackContainingBlock(A), stackB = findStackContainingBlock(B);

	// Return blocks to their original stacks until we get to A and B
	returnBlocksUntil(stackA, A);
	returnBlocksUntil(stackB, B);

	// Put A on B
	stacksArr[stackB].emplace_back(stacksArr[stackA].back());
	stacksArr[stackA].pop_back();
}

// move A over B
// Puts A onto stack containing B after returning blocks on top of A
void moveAOverB(int A, int B) 
{
	// Get stacks containing A and B
	int stackA = findStackContainingBlock(A), stackB = findStackContainingBlock(B);

	// Removes any blocks on top of A
	returnBlocksUntil(stackA, A);

	// Put block A onto the stack containing block B
	stacksArr[stackB].emplace_back(stacksArr[stackA].back());
	stacksArr[stackA].pop_back();
}

// pile A onto B
void pileAOntoB(int A, int B)
{
	// Get stacks containing A and B
	int stackA = findStackContainingBlock(A), stackB = findStackContainingBlock(B);

	// Move blocks off of the stack B is in until B is reached
	returnBlocksUntil(stackB, B);

	// Move the pile on top of A onto the stack where B is located
	movePile(stackA, stackB, A);
}

// pile A over B
void pileAOverB(int A, int B)
{
	// Get stacks where A and B are located
	int stackA = findStackContainingBlock(A), stackB = findStackContainingBlock(B);

	// Move the pile wehre A is located to where B is located
	movePile(stackA, stackB, A);
}

// Finds the stack containing the desired block
int findStackContainingBlock(int block)
{
	// Go through all of the stacks
	for (int i = 0; i < numStacks; i++)
	{
		// Find the stack containing block
		if (find(stacksArr[i].begin(), stacksArr[i].end(), block) != stacksArr[i].end())
		{
			return i;
		}
	}

	return -1; // couldn't find block
}

// Moves blocks back to their original places until the desired block is reached
void returnBlocksUntil(int stack, int block)
{
	// Put all blocks back until we get to A
	while (stacksArr[stack].back() != block)
	{
		stacksArr[stacksArr[stack].back()].emplace_back(stacksArr[stack].back());
		stacksArr[stack].pop_back();
	}
}

// moves a pile up to a target block to another pile
void movePile(int stackSource, int stackTarget, int stackSourceBlock)
{
	list<int> tmp;

	// Remove blocks until we reach our target
	while (stacksArr[stackSource].back() != stackSourceBlock)
	{
		tmp.emplace_back(stacksArr[stackSource].back());
		stacksArr[stackSource].pop_back();
	}

	// get the last block
	tmp.emplace_back(stacksArr[stackSource].back());
	stacksArr[stackSource].pop_back();

	while (!tmp.empty())
	{
		stacksArr[stackTarget].emplace_back(tmp.back());
		tmp.pop_back();
	}

}

// Opens file, terminates program if file fails to open
void openFile(ifstream &infile, string filename) {
	infile.open(filename);
	
	if( !infile.is_open() )
	{
		exit(1);
	}
}

// Gets the number of lines in an input file
int getLines(ifstream &in) {
	int count = 0;
	string s;

	while (!in.eof())
	{
		getline(in, s);
		count++;
	}

	in.seekg(ios_base::beg);
	return count;
}

// Output and empty all stacks
void printStacks(ofstream &out) {
	int z = 0;

	for (int i = 0; i < numStacks; i++)
	{
		cout << i << ": ";
		out << i << ": ";

		while (!stacksArr[i].empty())
		{
			cout << stacksArr[i].front() << " ";
			out << stacksArr[i].front() << " ";
			stacksArr[i].pop_front();
		}

		cout << endl;
		out << endl;
	}
}