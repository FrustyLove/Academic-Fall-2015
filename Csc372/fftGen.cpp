#include<iostream>
#include<cmath>
#include<fstream>

using namespace std; 

//*****************************************************************************
//Author: Jared Johnson
//Description: A simple sin program to create data points for sinusoidal 
//             functions.
//
//*****************************************************************************/
int main(int argv, char *argc[] )
{
  double i = 0;
  int numsin = 0;
  double freq[10] = {0};
  double amp[10] = {0};
  double sum = 0;
  int n = 0;
  int k = 0;
  double shift[10] = {0};
  int j = 0;
  ifstream fin;
  ofstream fout;
  
  fin.open( argc[1] );
  fout.open( argc[2] );

  fin >> numsin;

  fin >> n >> k;
  fout << n << endl << k << endl;

  while( fin >> amp[j] >> freq[j] >> shift[j] )
  {
     j++;
  }

  for( i = 1; i <= 2*n; i++) 
  {
     for( j = 0; j < numsin; j++ )
     {
       sum += amp[j] * sin( freq[j] * i + shift[j]);
     }

    fout << sum << endl;
    sum = 0;
  }
 
  fout.close( );
  fin.close( );    

}
