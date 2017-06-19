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
// rootGen is used to generate 64 strings 000000, 000001,...
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
vector<char> triCon({ 's','l', 'n','f','w','W' });
int N = 4;

// Define the structure of "killer" function
// field words is the function, e.g. fffwwww
// field numUsed is the number of times a certain function has been used to kill a sub-box
// field wordLength is the length of the function, e.g. the wordLength of the function fffwwww is 7
// field transLength is the length of the translation of the function
struct killFun
{
	string words;
	int numUsed;
	int wordLength;
	ACJ transLength;
	killFun(ACJ t, string w = "", int n = 0, int wl = 0) : words(w), numUsed(n), wordLength(wl), transLength(t) {}
};

// Define the function to compare two killFuns
// first sort by the number of times these two functions used 
// second sort by the length of the functions
// finally sort by the length of the the translation
bool compare_entry(const killFun & e1, const killFun & e2) {
	if (e1.numUsed != e2.numUsed)
	{// sort in descending order
		return (e1.numUsed > e2.numUsed);
	}
	else if (e1.wordLength != e2.wordLength)
	{// sort in ascending order
		return (e1.wordLength < e2.wordLength);
	}// sort in ascending order
	return (absUB(e1.transLength) < absUB(e2.transLength));
}

vector <killFun> conList;
vector<killFun> newConList;
// funGen is used to generate all the killerwords with length less than or equal to K
// if newCon is true, then generated killerwords are put in newConList 
void funGen(string res, int k, bool newCon = false)
{
	killFun tempKill;
	tempKill.words = res;
	tempKill.numUsed = 0;
	tempKill.wordLength = size(res);
	if (k != 0)
	{
		if (newCon)
		{
			newConList.push_back(tempKill);
		}
		else
		{
			conList.push_back(tempKill);
		}
	}

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
			funGen(res, k + 1, newCon);
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
				funGen(res, k + 1, newCon);
				res.erase(res.end() - 1);
			}
			res.erase(res.end() - 1);
		}
	}
}
////////////////////////////////////////////////////
// initialCon is used to add the 6 trival conditions to the conList
void initialCon()
{
	for (int i = 0; i < 6; i++)
	{
		killFun temp;
		temp.words = triCon[i];
		temp.numUsed = 0;
		temp.wordLength = 1;
		conList.push_back(temp);
	}
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
bool checkInequal(vector <killFun> conList, char*where, int depth)
{
	int i = 0;
	while (i < int(conList.size()))
	{
		string temp = conList[i].words;
		if (i >= 6)
		{// skip the first six conditions: trival conditions
			// add left and right paranthesis to conList[i].second
			temp.insert(temp.begin() + 1, '(');
			temp.insert(temp.end(), ')');
		}
		const char *code = temp.c_str();
		if (inequalityHolds(code , where, depth))
		{
			(conList[i].numUsed)++;// update the used times 
			// find the translation length
			double pos[6], size[6], scale[6];
			for (int j = 0; j<6; j++) {
				pos[j] = 0;
				size[j] = 4;
				scale[j] = pow(2, (5 - j) / 6.0);
			}
			for (int d = 0; d<depth; d++) {
				size[d % 6] /= 2;
				if (where[d] == '0') {
					pos[d % 6] -= size[d % 6];
				}
				else if (where[d] == '1') {
					pos[d % 6] += size[d % 6];
				}
				else {
					assert(0);
				}
			}
			for (int j = 0; j<6; j++) {
				pos[j] *= scale[j];
				size[j] = (1 + 2 * EPS)*(size[j] * scale[j] + HALFEPS*fabs(pos[j]));
			}
			ACJ along((XComplex(pos[0], pos[3])), XComplex(size[0], size[3]), 0, 0, 0);
			ACJ ortho((XComplex(pos[1], pos[4])), 0, XComplex(size[1], size[4]), 0, 0);
			ACJ whirle((XComplex(pos[2], pos[5])), 0, 0, XComplex(size[2], size[5]), 0);
			SL2ACJ g(evaluateWord(code + 1, along, ortho, whirle));
			conList[i].transLength = length(g);
			////////////////////////////////////
			// sort the condition list
			// do not sort the first 6 conditions
			sort(conList.begin() + 6, conList.end(), compare_entry);
			return true;
			break;
		}
		i++;
	}
	return false;
}

int maxDepth = 200;

struct node
{
	char *where;
	int killIndex = 0;// 0 means not killed, 1 means killed 
	node *parent = NULL;
	node *left = NULL;
	node *right = NULL;
};


bool dividBox(node *subBox, int depth, vector<string> exceptBox)//true means killed, false means not killed
{
	// First, check if depth is too deep
	if (depth == maxDepth)
	{
		fprintf(stderr, "verify: fatal error at %s\n", subBox->where);
		exit(1);
	}
	// Then check if its parent is killed.
	if (((subBox->parent)->killIndex) == 1)
	{
		return true;
	}
	// If its parent is not killed 
	// then first check if it is one of the 11 regions
	if (checkExcept(subBox->where, exceptBox))
	{
		cout << "Find exceptional sub-box" << (subBox->where) << endl;
		return true;
	}// if not, then check if it can be killed by one of the conditions
	else if (checkInequal(conList,subBox->where, depth))
	{
		cout << "Killed" << subBox->where << endl;
		return true;
	}
	else {// if every inequalityHold returns 0
		  //then add a surfix to conditions that have been used 
		vector<killFun> newConList;
		for (int i = 6; i < conList.size(); i++)
		{
			if (conList[i].numUsed != 0)
			{
				funGen(conList[i].words, K-1,true);
			}
		}
		// use new condition to check the current sub-box and its 6 parents
		if (depth > 12)
		{
			vector<node*> tempBox;
			tempBox.push_back(subBox);
			for (int i = 1; i <= 6; i++)
			{
				tempBox.push_back((tempBox[i - 1])->parent);
			}
			int i = 6;
			while (i >= 0)
			{
				if (checkInequal(newConList,(tempBox[i])->where, depth - i))
				{
					(tempBox[i])->killIndex = 1;
					//kill all the descendents
					for (int j = 0; j < i; j++)
					{
						(tempBox[j])->killIndex = 1;
					}
					return true;
					break;
				}
				i--;
			}
		}
		for (int i = 0; i < newConList.size(); i++)
		{
			conList.push_back(newConList[i]);
		}
		sort(conList.begin() + 6, conList.end(), compare_entry);
		// if none of them can be killed by the new conditions
		// subdivide the current box
		// left child
		subBox->left = new node;
		string temp = string(subBox->where);
		temp += '0';
		subBox->left->where = &temp[0];
		subBox->left->parent = subBox;
		dividBox(subBox->left, depth + 1, exceptBox);
		// right child
		subBox->right = new node;
		string temp = string(subBox->where);
		temp += '1';
		subBox->right->where = &temp[0];
		subBox->right->parent = subBox;
		dividBox(subBox->right, depth + 1, exceptBox);
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
	initialCon();
	//
	K = 10;
	funGen("", 0);
	rootGen();
	for (int i = 0; i < int(root.size()); i++)
	{
		if (root[i][3] == 1)
		{
			continue;
		}
		node *box;
		box->where = &root[i][0];
		dividBox(box, root[i].size(), exceptBox);
	}
	return 0;
}