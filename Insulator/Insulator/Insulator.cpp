#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <sstream>
#include <cstdio>
#include <algorithm>

#include "ACJ.h"
#include "Codes.h"
#include "Complex.h"
#include "roundoff.h"
#include "SL2ACJ.h"



using namespace std;


vector<string> root;
char divid[2] = { '0','1' };
void rootGen(string res = "", int k = 0)
{
	if (k != 0)	root.push_back(res);
	if (k == 6)
	{
		return;
	}
	for (int i = 0; i < 2; i++)
	{
		res += divid[i];
		rootGen(res, k + 1);
		res.erase(res.end() - 1);
	}
}
/////////////////////////////////////////
int K;
vector<char> killWord({ 'f', 'w', 'F', 'W' });
vector<char> intCon({ 'L','O','2' });
int N = 4;
// the first element of conList is the ranking of the second element
vector<pair<int, string>> conList; 
// funGen is used to generate all the killerwords with length less than or equal to K
void funGen(string res, int k)
{
	if (k != 0)	conList.push_back(make_pair(0, res));
	if (k == K)
	{
		return;
	}
	int newN;
	vector<char> newarr;
	if (res.length() != 0) // generate killerwords given a prefix
	{
		newN = N - 1;
		for (int i = 0; i < N; i++)
		{
			if ((res.back() == killWord[i]) || (toupper(res.back()) != toupper(killWord[i])))
			{ // a function can not be directly followed by its inverse
				newarr.push_back(killWord[i]);
			}
		}
		for (int i = 0; i < newN; i++)
		{
			res += newarr[i];
			funGen(res, k + 1);
			res.erase(res.end() - 1);
		}
	}
	else { // generate killerwords from empty string
		newarr = killWord;
		newN = N;
		for (int j = 0; j < 3; j++)
		{
			res += intCon[j];
			for (int i = 0; i < newN; i++)
			{
				res += newarr[i];
				funGen(res, k + 1);
				res.erase(res.end() - 1);
			}
			res.erase(res.end() - 1);
		}
	}
}
////////////////////////////////////////////////////

void appendFun()
{

}

// check if where is one of the 11 exceptional sub-boxes
bool checkExcept(char*where, vector<string> exceptBox)
{
	int i = 0;
	while (i < 11)
	{
		if (where == exceptBox[i])
		{
			return true;
			break;
		}
		i++;
	}
	return false;
}

 //check if one of the candidate conditions can kill where
 //and sort the candidate conditions based on their ``usefulness"
bool checkInequal(char*where, int depth)
{
	int i = 0;
	while (i < int(conList.size()))
	{
		string temp = conList[i].second;
		if (i >= 6)
		{// skip the first six conditions: trival conditions
			// add left and right paranthesis to conList[i].second
			temp.insert(temp.begin() + 1, '(');
			temp.insert(temp.end(), ')');
		}
		const char *code = temp.c_str();
		if (inequalityHolds(code , where, depth))
		{
			(conList[i].first)++;
			// sort the condition list
			// do not sort the first 6 conditions
			sort(conList.rbegin(), conList.rend() - 6);
			return true;
			break;
		}
		i++;
	}
	return false;
}

int maxDepth = 50;

vector<string> failBox;

void subBox(char*where, int depth, vector<string> exceptBox)
{
	if (depth == maxDepth)
	{// check if depth is too deep
		failBox.push_back(where);
		fprintf(stderr, "verify: fatal error at %s\n", where);
		exit(1);
	}
	if (checkExcept(where, exceptBox))
	{
		cout << "Find exceptional sub-box" << where << endl;
	}
	else if (checkInequal(where, depth))
	{
		cout << "Killed" << where << endl;
	}
	else {// if every inequalityHold returns 0 then subdivide
		  // and add new conditions with prefix being used condtions


		  // subdivide the current box
		// left child
		string temp = string(where);
		temp += '0';
		where = &temp[0];
		subBox(where, depth + 1, exceptBox);
		// right child
		temp[temp.size() - 1] = '1';
		where = &temp[0];
		subBox(where, depth + 1, exceptBox);
	}
}


int main()
{
	// codes for all 11 exceptional sub-boxes
	vector<string> exceptBox({
		"001000110111110001 101001010101011001 011011010111101101 100001101101000111 010001110101100101 1101110111110100",
		"001001110110110000 101000010100011000 011010010110101100 100000101100000110 010000110100100100 1101100111100100",
		"001000110001110110 011101000110111110 100010110000100011 101101001101001000 110101011000000100 000",
		"001000110101010010 101010110001100101 110111100001101010 111100100000010001 111100",
		"111000000001000110 011011101101011000 111101011110001100 111111100110110000 0000100010100010",
		"111000000001000110 011001001111101010 011110110110111101 100011111110110110 10000111101",
		"111000000001000110 011001001111101010 111110010110011101 000011011110010110 00000101101",
		"001000110001110111 001111000101111111 101111100111001111 000001111011110111 1",
		"001001110000110110 001110000100111110 101110100110001110 000000111010110110 1",
		"111000000001000111 111111110101001111 011111010111111111 110001001011000111 0",
		"111001000000000110 111110110100001110 011110010110111110 110000001010000110 0" });
	// get rid of spaces in the strings
	for (int i = 0; i < 11; i++)
	{
		exceptBox[i].erase(remove_if(exceptBox[i].begin(), exceptBox[i].end(), isspace), exceptBox[i].end());
	}
	
	/////////////////////////
	// initialize the conList
	conList.push_back(make_pair(0, "s"));
	conList.push_back(make_pair(0, "l"));
	conList.push_back(make_pair(0, "n"));
	conList.push_back(make_pair(0, "f"));
	conList.push_back(make_pair(0, "w"));
	conList.push_back(make_pair(0, "W"));
	K = 5;
	funGen("", 0);
	rootGen();

	for (int i = 0; i < int(root.size()); i++)
	{
		if (root[i][3] == 1)
		{
			continue;
		}
		subBox(&root[i][0], root[i].size(), exceptBox);
	}

	return 0;
}