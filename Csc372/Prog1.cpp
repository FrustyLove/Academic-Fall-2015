//****************************************************************************/
//Program: Prog1 -- Repeated Pairs
//Author: Jared Johnson
//Class: CSC 372 -- Analysis of Algorithms
//Instructor(s): Dr. Logar -- Dr. Corwin
//Date: 10 - 4 - 2015
//Description: Program recieves a string of characters of length 1 to 5000 and
//       by using a dynamic recursive solution determines whether or
//       not the string is a sequence of repeated pairs.
//Input: Several test cases may be provided each on a seperate line, each line
//       consising a string of 1 to 5000 characters, excuding spaces.
//       Program reads until end of file.
//Output: Output consists of an integer denoting case number, a space,
//       and a "Yes" or "No" as a solution to the repeated pairs problem,
//       each on a seperate, consecutive line for each test case.
//Compilation Instructions: Compilation expected on a Unix system
//Usage: Program expects either console input or redirected file input and
//       output, and uses standard input and output.
//Known bugs/Missing features: This program cannot solve for strings greater
//       than 5000 characters in length. This program also only uses standard
//       input.
//Modifications:
//Date		Comment
//
//****************************************************************************/
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

//********************************Global arrays*******************************/
std::vector<bool> lookx;
std::vector<bool> looky;
std::vector<bool> pairx;
std::vector<bool> pairy;

using namespace std;

//*******************************Function Prototypes**************************/
bool compPair( string mstr, int start, int end );

//****************************************************************************/
//Function: main
//Author: Jared Johnson
//Description: Recieves standard input for the problem, which consists of a
//	       string of 1 to 5000 characters, outputs "No" if string size
//	       is odd, calls the compPair funcion otherwise, and outputs "Yes"
//	       if compPair returns true and "No" if false. Each "Yes" or "No"
//	       is preceeded with a case number followed by a space. Resets 
//	       global arrays to 0 at every index for each case.
//Parameters: No Parameters
//Returns:    0 -- Program finished
//****************************************************************************/
int main( )
{
string str;
int i = 1;
int j = 0;
ifstream fin;
ofstream fout;

fin.open("pairs.in");
fout.open("pairs2.out");

while( fin >> str )
{
//	pairx.resize(5000, 0);
//	pairy.resize(5000, 0);
//	lookx.resize(5000, 0);
//	looky.resize(5000, 0);

	//if the size of the string is odd, a sequence of pairs is
	//impossible
	if( str.size( ) % 2 != 0 )
	{
		fout << i << " no" << endl;
	}
	//Otherwise run the recursive solution and output yes if 
	//the given string is a sequence of pairs and no if it is not
	else if(compPair( str, 0, str.size( )-1 ) == 1 )
	{
		fout << i << " yes" << endl;
	}
	else
	{
		fout << i << " no" << endl;
	}
		
	//Increment case counter
	i++;

	//Reset the global arrays
	pairx.clear( );
	pairy.clear( );
	lookx.clear( );
	looky.clear( );
}

fin.close( );
fout.close( );
return 0;
}

//****************************************************************************/
//Function: compPair
//Author: Jared Johnson
//Description: Uses dynamic recursion to solve for the repeated pairs problem
//	       by solving for pairs in current mstr, while recursing for pairs
//	       in a substring of mstr, returning a boolean value 
//	       when the empty string is 
//	       encountered, when the substring has been previously solved
//	       when the current string has no pairs, and when all pairs
//	       in a current string have been found.
//Parameters: [in] string mstr -- string/substring to solve for 
//	      [in] int start -- starting point for the string mstr
//	      [in] int end -- ending point for the string mstr
//Returns:    True  -- Empty String endcountered
//		    -- Substring previously computed as a pair
//		    -- All pairs found in current string, and 
//		       resursion returned True
//	      False -- Substring previoulsy computed and was not 
//	               nessisarily a pair
//	            -- No pairs found in current string
//	            -- Pairs found in current string but not in substring
//****************************************************************************/
bool compPair( string mstr, int start, int end )
{
string lstr;
string rstr;
string next;
int i;
bool isPair = false;
bool isNxt = false;
int j = 1;

//If the string or substring provided is empty at this point return true
if( mstr.empty( ) )
{
	return true;
}

//If the substring at this point in the recusion has be computed before
//and was a pair, return true
if( pairx[start] == true && pairy[end] == true )
{
	return true;
}

//If the substring at this point in the recursion has been computed before
//and was not a pair, return false
if( lookx[start] == true && looky[end] == true )
{
	return false;
}

//Step backwards through string/substring by 2 to find pairs of
//2, 4, 6 ... n length
//recursing with the remaining string when pairs are found 
for( i = mstr.size( )-2; i >= 0; i-=2 )
{
	//Cut the end n values of the string in half to detect a pair	
	lstr.assign(mstr, i, j );
	rstr.assign(mstr, i+j, j );

	if( lstr == rstr )
	{
		//Found pair cut pairs from string
		//and recurse with remaining substring
		next = mstr;
		next.erase( i, 2*j );
		//Table start and end value for the pair found
		pairx[i] = true;
		pairy[end] = true;
		//Set isPair to true to denote that a pair has been
		//found at this stage of the recursion
		isPair = true;
		//Recurse and store result into isNxt to denote that
		//a pair was found at the next stage of the recursion
		//or the empty list was encountered		
		isNxt = compPair( next, start, i-2 );
	}
	j++;

}

//table non-pair start and end values
lookx[start] = true;
looky[end] = true;

//If pairs were found in both the current stage of recursion and the previous
//stage of recursion return true
if( isPair == true && isNxt == true )
{
	return true;
}
//Otherwise return false
return false;
}
