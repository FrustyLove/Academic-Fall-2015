//*****************************************************************************
//Program: Prog3
//Author: Jared Johnson
//Course: Csc 372 Analysis of Algorithms
//Instructor(s): Dr. Logar, Dr. Corwin
//Due: 12/01/15
//Description: This program will compute a sliding FFT by first computing an
//             fft on a subset of given data points. After that the points
//             resulting from the first fft can be modified to represent the
//             fft of the next subset of points. This program will compute k-1
//             fft's after the initial fft in linear time based on that fact
//             and add the absolute values of the results to a running total.
//             Finally the program will print the 5 greatest peaks from the
//             running total.
//Usage: Progarm used an input file specified as the second argument
//       (i.e.): prog2 filename.in
//       Program outputs to filename.out
//Bugs: 1). Should the running total values exceed the maximum value that
//      a double can represent, things wont go well.
//****************************************************************************/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <iomanip>
#include <string>
#include <complex>
using namespace std;

const int MAX = 10000;
const double EPSILON = 1.0e-12;
const complex<double> I (0, 1);
enum  direction {FORWARD, INVERSE};

//*****************************************************************************
//Function: fft
//Author: Taken from Dr. Corwin's code on the website
//Description: computes a fast fourier transform given a set of points
//Parameters:
//  complex<double> a[] - data points for fft
//  int n - number of points in the fft
//  complex<double> y[] - points found from fft
//  direction dir - direction of the fft, FORWARD or INVERSE
//****************************************************************************/
void fft(complex <double> a[], int n, complex <double> y[],
  direction dir)
  {
  complex  <double> even[n];
  complex  <double> even_fft[n];
  int      i;
  int      j;
  int      n2;
  complex  <double> odd[n];
  complex  <double> odd_fft[n];
  complex  <double> omega;
  complex  <double> omega_power;

  if (n == 1)
    {
    y[0] = a[0];
    return;
    }
  
  n2 = n / 2;
  j = 0;
  for (i = 0; i < n; i += 2)
    {
    even[j] = a[i];
    odd[j] = a[i + 1];
    j ++;
    }
    
  fft(even, n2, even_fft, dir);
  fft(odd, n2, odd_fft, dir);

  if (dir == FORWARD)
    omega = cos(-2.0 * M_PI / n) + I * sin(-2.0 * M_PI / n);
  else
    omega = cos(2.0 * M_PI / n) + I * sin(2.0 * M_PI / n);
  omega_power = 1;
  for (i = 0; i < n2; i ++)
    {
    y[i] = even_fft[i] + omega_power * odd_fft[i];
    y[i + n2] = even_fft[i] - omega_power * odd_fft[i];
    omega_power = omega * omega_power;
    }
  }

//*****************************************************************************
//Function: is_maximum
//Author: Jared Johnson
//Description: is_maximum determines if a value in a complex array is a local
//             maximum.
//Parameters:
//   int index - index of value is_maximum is looking at
//   complex<double> a[] - running total values of the k fft's
//   int n - number of values in a
//****************************************************************************/
bool is_maximum( int index, complex <double> a[], int n )
{
  if( index == n || index <= 1 || index >= n-1 )
  {
     return false;
  }
  
  if( a[index].real()  > a[index-1].real() && a[index].real()
  > a[index+1].real() )
  {
     //cerr << a[index] << endl;
    // c[index] = a[index] + a[index-1] + a[index-2] + a[index+1] + a[index+2];
     return true;
  }

  return false;
}

//*****************************************************************************
//Function: find_maxima
//Author: Jared Johnson
//Description: find_maxima find the five greatest values in the running total
//             for the k fft's
//Parameters:
//   complex<double> a[] -  the running total from the k fft's
//   int n               -  number of values in a
//   complex<double> b[] -  array of greatest values in a
//   int index[]         -  array of indexes corrisponding to greatest values
//****************************************************************************/
void find_maxima( complex <double> a[], int n, complex <double> b[],
                  int index[] )
{
  int i;
  //b[0] will always be the minimum value, b[4] the max

  for( i = 2; i < n/2 - 1; i++ )
  {
    if( is_maximum( i, a, n ) )
    {
      if( a[i].real( ) > b[4].real( ) )
      {
         
         b[0] = b[1];
         b[1] = b[2];
         b[2] = b[3];
         b[3] = b[4];
         b[4] = a[i]+a[i-1]+a[i-2]+a[i+1]+a[i+2];
         index[0] = index[1];
         index[1] = index[2];
         index[2] = index[3];
         index[3] = index[4];
         index[4] = i;  
      }
      else if( a[i].real( ) > b[3].real( ) )
      {
         b[0] = b[1];
         b[1] = b[2];
         b[2] = b[3];
         b[3] = a[i]+a[i-1]+a[i-2]+a[i+1]+a[i+2];
         index[0] = index[1];
         index[1] = index[2];
         index[2] = index[3];
         index[3] = i;
      }
      else if( a[i].real( ) > b[2].real( ) )
      {
         b[0] = b[1];
         b[1] = b[2];
         b[2] = a[i]+a[i-1]+a[i-2]+a[i+1]+a[i+2];
         index[0] = index[1];
         index[1] = index[2];
         index[2] = i;
      }
      else if( a[i].real( ) > b[1].real( ) )
      {
         b[0] = b[1];
         b[1] = a[i]+a[i-1]+a[i-2]+a[i+1]+a[i+2];
         index[0] = index[1];
         index[1] = i;
      }
      else if( a[i].real( ) > b[0].real( ) )
      {
         b[0] = a[i]+a[i-1]+a[i-2]+a[i+1]+a[i+2];
         index[0] = i;
      }
    }
  }
}


//***************************************************************************
//Function: sliding_fft
//Author: Jared Johnson
//Description: sliding_fft uses an initial fft on a smaller subset of
//             datapoints that evaluates and initial polynomial P(x), a
//             new polynomial Q(x) is found k times as new data points
//             are introduced and the oldest data points are removed
//Parameters:
//   complex<double> a[] - first set of points for the fft
//   int n               - number of points in each fft
//   int k               - number of fft's to compute
//   complex<double> y[] - running total of the fft's absolute values
//   complex<double> all[] - all points that may be used for the fft
//**************************************************************************/
void sliding_fft(complex <double> a[], int n, int k, complex <double> y[],
                 complex <double> all[])
{
  complex<double> omega = cos(2.0 * M_PI / n) + I * sin(2.0 * M_PI / n);
  complex<double> omega_pow[n];
  complex<double> omega_expn = 1;
  complex<double> curr_fft[n] = {0};
  int i = 0;
  int count = 0;
  
  omega_pow[0] = complex<double>( 1, 0 );
  for( i = 1; i < n; i++ )
  {
    omega_pow[i] = omega * omega_pow[i-1];
  }
  

  //Evaluate first fft
  //curr_fft is the current fft
  fft( a, n, curr_fft, FORWARD );

  //add the absolute values for our first fft into our running total
  for( i = 0; i < n; i++ )
  {
    y[i] = y[i] + abs(curr_fft[i])/k;
  }
  
  //Evaluate each polynomial window in constant time by subtracting a[0] 
  //adding a[n]*(omega^pow)^n-1, and dividing by omega^pow in the frequency
  //domain
  //do this k-1 times (accounting for initial fft, shifting the window
  //each time
  while( count < k-1 )
  {
    //For each slide, we evaluate a new polynomial at its n, nth roots of unity
    //Resulting in a DFT, we can evaluate each polynomial in constant time,
    //resulting
    //in K, O(n) DFT Calculations
    for( i = 0; i < n; i++ )
    {
       //Because each element in y is an evaluatoin of P(X), found by our
       //initial FFT, we can modify each element in the current
       //fft to find Q(x),the next fft
       //omega^n-1 is, by way of complex magic algebra, 1/omega_pow
       omega_expn = complex<double>(1, 0) / omega_pow[i];
       curr_fft[i] = curr_fft[i] - all[count];
       curr_fft[i] = curr_fft[i] / omega_pow[i];
       curr_fft[i] = curr_fft[i] + all[count+n] * omega_expn;

       //Add to the running total
       y[i] = y[i] +  abs(curr_fft[i])/k;      
    }
    //Increment our fft counter
    count++;
  }
}

//*****************************************************************************
//Function: main
//Author: Jared Johnson
//Description: function main parses input, and calls the sliding fft function
//             as well as the find maxima function to find the top 5 maximum
//             values from the sliding fft.
//Parameters:
//    int argc: argument count
//    char* argv[]: arguments
//*****************************************************************************
int main( int argc, char *argv[] )
  {
  complex<double>  maxima[4] = {0};
  complex<double>  a[MAX];
  complex<double>  b[MAX] = {0};
  complex<double>  tot[MAX];
  complex<double>  y[MAX] = {0};
  ifstream inf;
  double   inp;
  int      i = 0;
  int      q = 0;
  int      j = 0;
  int      z = 0;
  int      n = 0;
  int      k = 0;  
  int      index[4] = {0};
  ofstream otf;
  string   out;
  int      dot = 0;
  inf.open( argv[1] ); 
  out = argv[1];
  q = out.size();

  while( out[q] != '.' )
  {
    dot++;
    q--;
  }
  out.replace( out.end()-dot, out.end(), ".out" );

  otf.open( out.c_str( ) );

  //Get points
  inf >> n;
  //Get number of fft's to compute
  inf >> k;
  //Grab n points into complex array tot
  while( inf >> inp )
  {
    tot[i] = complex<double> ( inp, 0 );
    //copy first n points into complex array a
    if( i < n )
    {
      a[i] = tot[i];
    }
    
    i++;
  }

  inf.close( );

  //Perform the sliding fft
  sliding_fft( a, n, k, b, tot );

  //Find the top five maxima
  find_maxima( b, n, maxima, index );

  otf << fixed;
  otf << setprecision(2);
  
  /*
  for( i = 0; i < 5; i++ )
  {
    maxima[i] = b[index[i]] + b[index[i]+1] + b[index[i]+2]
              + b[index[i]-1] + b[index[i]-2];
  }
  */

  for( i = 4; i >= 0; i-- )
  {
    otf << index[i] << " " << abs(maxima[i]) << endl;
  }

  return 0;

 }
