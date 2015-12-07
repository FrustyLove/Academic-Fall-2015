
//****************************************************************************/
//Program: Prog2
//Author: Jared Johnson
//Course: Csc 372 Analysis of Algorithms
//Instructor(s): Dr. Logar , Dr. Corwin
//Due: 11/01/15
//Description: This program will recieve input consisting of first, an integer
//	       number for the number of points follwing, followed by the real
//	       number value pairs representing points on a 2-space plane that
//	       create a polygon. This program will first determine if the given
//	       points form a polygon that is convex or not and output a case 
//	       number and "No solution" if not. The program will otherwise
//	       determine the optimal vertical split point to seperate the
//	       convex polygon into two equal halves output the case number
//	       followed by the x-value of the split accurate to 5 decimal 
//	       places. This program will process cases until the end of file.
//Usage: Program uses an input file specified as the second argument
//	(i.e.): prog2 filename.in
//	 Program outputs to filename.out
//Bugs: 1). It may be that function findy returns a point innaccurate to a
//	degree. This may affect output for certain cases.
//	2). Very large real values will result in overflow error.	       
//****************************************************************************/
#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<iomanip>

using namespace std;

struct point{
	double x;
	double y;
};

int direction( point a, point b, point c );
bool convex( vector<point> list, int size );
bool intersect( point p1, point p2, point p3, point p4 );
double findy( double x, point p1, point p2 );
double findArea( vector<point> list, int size );
double bisect( vector<point> list, double &goal, double left,
 double right, bool &clockwise, double &miny, double &maxy );
bool precisionCheck( double a, double b );

//****************************************************************************/
//Function: main
//Author: Jared Johnson
//Description: function main parses input, finds min & max values for later
//	       computation, opens files for read and writing, calls bisection
//	       function, tracs cases, and writes solutions to given cases. 
//	       main also determines convexity of given polygon.
//Parameters:
//   int argc: argument count
//   char* argv[]: aruments
//****************************************************************************/
int main( int argc, char *argv[] )
{
vector<point> list;
double area = 0;
double areaGoal = 0;
double maxy = 0;
double miny = 0;
double minx = 0;
double maxx = 0;
bool clockwise = 0;
int numpoint = 0;
int dot = 0;
int i;
int j = 1;
ifstream fin;
ofstream fout;

string out = argv[1];
i = out.size( );
//Create name for output file
while( out[i] != '.' )
{
	dot++;
	i--;
}
out.replace( out.end( )-dot, out.end( ), ".out" ); 

fin.open( argv[1] );
fout.open( out.c_str( ) );

//Set fixed precision to 5
fout << fixed;
fout << setprecision(5);

//While there are values to read
//read the number of points for polygon
while( fin >> numpoint )
{
	list.resize(numpoint);
	for( i = 0; i < numpoint; i++ )
	{
		//Input points and set current min and max values
		fin >> list[i].x >> list[i].y;
		if( i == 0 )
		{
			miny = list[i].y;
			maxy = list[i].y;
			minx = list[i].x;
			maxx = list[i].x;
		}
		else
		{
			if( list[i].y > maxy )
			{
				maxy = list[i].y;	
			}
			else if( list[i].y < miny )
			{
				miny = list[i].y;
			}
		
			if( list[i].x > maxx )
			{
				maxx = list[i].x;
			}
			else if( list[i].x < minx )
			{
				minx = list[i].x;
			}
		}
	}

	//Determine if the given points form a convex polygon
	if( !convex( list, numpoint ) )
	{
		fout << "Case " << j << ": " << "No solution" << endl;	
	}
	else
	{
		//Check if our points are given in clockwise order	
		if( direction( list[0], list[1], list[2] )  < 0 )
		{
			clockwise = 1;
		}
	

		//Find area of whole polygon
		area = findArea( list, numpoint );
		areaGoal = area / 2;
		cout << area << "\n" << areaGoal << endl;
		//cin >> i;
		fout << "Case " << j << ": " <<
		 bisect( list, areaGoal, minx, maxx, clockwise,
		miny, maxy ) << endl;
	}

	//Increment case counter and reset necessary vairables
	j++;	
	list.clear( );
	minx = 0;
	maxx = 0;
	miny = 0;
	maxy = 0;
	clockwise = 0;
		
}

fout.close( );
fin.close( );
return 0;	
}

//****************************************************************************/
//Function: direction
//Author: Jared Johnson
//Description: function direction determines directon taken form a->b->c
//Parameters:
//    point a: starting point
//    point b: midpoint
//    point c: endpoint 
//****************************************************************************/
int direction(  point a, point b, point c )
{

point u;
point v;
	
//u sub x/y is found by determining difference between points a and b
u.x = b.x - a.x;
u.y = b.y - a.y;

//v sub x/y is found by dtermining difference between points b and c
v.x = c.x - b.x;
v.y = c.y - b.y;

//return cross product of u and v (u X v)
return ( u.x * v.y - u.y * v.x );

}


//****************************************************************************/
//Function: convex
//Author: Jared Johnson
//Description: function convex determines if a given set of points form a 
//	       convex polygon.
//Parameters:
//    vector<point> list: list of points that create a polygon
//    int size: number of points in the polygon
//****************************************************************************/
bool convex( vector<point> list, int size )
{

long int cdir = 1;
long int ldir = 1;
int i;

//Check that every turn when traverwing polygon points is the same
//Return false anytime a turn is different from the previous turn
for( i = 0; i <= size-3; i++ )
{
	cdir = direction( list[i], list[i+1], list[i+2] );
	if( ldir * cdir < 0 && i != 0 )
	{
		return false;	
	}
	ldir = cdir;
}	

return true;

}

//****************************************************************************/
//Function: findArea
//Author: Edward Corwin (taken form geometry.C)
//Description: funciton findArea, code taken form geometery.C, finds the area
//	       of a polygon given its points and size
//Parameters:
//    vector<point> list: list of points in polygon
//    int size: number of points in list
//****************************************************************************/
double findArea( vector<point> list, int size )
{

int i;
int j;
double result;

for( i = 0; i < size; i++ )
{
	j = (i + 1) % size;
	result += (( (list[i].x)) *( (list[j].y)) );
	result -= (( (list[i].y)) *( (list[j].x)) );
}
//cout << result << endl;
return fabs(result/2);
}

//****************************************************************************/
//Function: bisect
//Author: Jared Johnson
//Description: function bisect makes bisects a polygon between a left and right
//	       point on the x axis. Area is calculated on the left adn right
//	       side of the bisection. Areas found are then compared and the 
//	       larger half is further split towards the center fo the whole
//	       polygon.
//Parameters:
//    vector<point> list: list of points in polygon
//    double &goal: half the area of the whole polygon
//    double left: the left delimiting number  for our bisection
//    double right: the right delimiting number for out bisection
//    bool &clockwise: a boolean value for whether our points are given in 
//    			clockwise or counterclockwise order
//    double &miny: minimum y value among points
//    double &maxy: maximum y value among points
//****************************************************************************/
double bisect( vector<point> list,  double &goal, double left,
 double right, bool &clockwise, double &miny, double &maxy )
{

double cut = 0;
int i;
int j;
double yhi = miny;
double ylow = maxy;
double ytemp = 0;
bool solved  = false;
vector<point> lhalf;
vector<point> rhalf;
point p1;
point p2;
point newp;

while( solved == false )
{
	yhi = miny;
	ylow = maxy;
	ytemp = 0;

	//Find a midpoint to use (bisecting larger half)
	cut = (left + right) / 2;
	
	//Set x value for new point on left and right polygons
	newp.x = cut;
	
	//Make a line from p1 - p2 with y values greater than max and min
	//y's, make sure max and min y values are large enough for an accurate
	//findy calculation
	p1.x = cut;
	p1.y = maxy+10000;
	p2.x = cut;
	p2.y = miny-10000;

	cout << "CUT AT: " << cut << endl;	
	//Find our intersections and store out lower and
	//upper y values for the intersection
	for( i = 0; i < list.size( ); i++ )
	{
		j = (i+1)%list.size( );
		if( intersect( p1, p2, list[i], list[j] ) )
		{
			ytemp = findy( newp.x, list[i], list[j] );

			if( ytemp > yhi )
			{
				yhi = ytemp;
			}
		
			if( ytemp < ylow )
			{
				ylow = ytemp;
			}

		}
		
	}

	//if clockwise add upper point made by cut to left
	//first then lower
	//add lower point to right first then upper
	if( clockwise == 1 )
	{
		for( i = 0; i < list.size( ); i++ )
		{
			j = (i+1)%list.size( );

			if( list[i].x < cut )
			{
				lhalf.push_back( list[i] );
			
				if( list[j].x >= cut )
				{	
					newp.y = yhi;
					lhalf.push_back( newp );
					newp.y = ylow;
					lhalf.push_back( newp );
				}
			}
			else if( list[i].x > cut )
			{
				rhalf.push_back( list[i] );
			
				if( list[j].x <= cut )
				{
					newp.y = ylow;
					rhalf.push_back( newp );
					newp.y = yhi;
					rhalf.push_back( newp );
				}
			}

		}
	}
	
	//Left = lower then upper
	//Right = upper then lower
	if( clockwise == 0 )
	{
		for( i = 0; i < list.size( ); i++ )
		{
			j = (i+1)%list.size( );
			if( list[i].x < cut )
			{
				lhalf.push_back( list[i] );
			
				if( list[j].x >= cut )
				{	
					newp.y = ylow;
					lhalf.push_back( newp );
					newp.y = yhi;
					lhalf.push_back( newp );
				}
			}
			else if( list[i].x >= cut )
			{
				rhalf.push_back( list[i] );
			
				if( list[j].x < cut )
				{
					newp.y = yhi;
					rhalf.push_back( newp );
					newp.y = ylow;
					rhalf.push_back( newp );
				}
			}
		}
	}
/*	
	cout << "Left: " << findArea( lhalf, lhalf.size( ) ) << endl;
	
	for( i = 0; i < lhalf.size(); i++ )
	{
		cout << lhalf[i].x <<" "<< lhalf[i].y << endl;
	}
	cout << "Right: " << findArea( rhalf, rhalf.size( ) ) << endl;
	for( i = 0; i < rhalf.size( ); i++ )
	{
		cout << rhalf[i].x <<" "<< rhalf[i].y << endl;
	}
	cin >> i;
*/
	if(precisionCheck( findArea( lhalf, lhalf.size( ) ),goal) == true )
	{
		solved = true;
	}	
	if( findArea( lhalf, lhalf.size( )) >
	    findArea( rhalf, rhalf.size( )))
	{
		rhalf.clear( );
		lhalf.clear( );
		right = cut;
	}
	else
	{
		rhalf.clear( );
		lhalf.clear( );
		left = cut;
	}
}

return cut;

}

//****************************************************************************/
//Function: intersect
//Author: Jared Johnson
//Description: function intersect determines wheterh or not a line segment
//             p3-p4 straddles p1-p2. If it does we assume it itersects, 
//             because we, in this program, know that p1-p2 is a vertical cut
//             that exceeds minimum and maximum y values. This is not a true
//             intersection function for any given points.
//Parameters:
//    point p1: point p1 on lines segement p1-p2 uses min or max y value
//    point p2: point p2 on line segment p1-p2 uses min or max y value
//    point p3: point p3 on line segment p3-p4
//    point p4: point p4 on line segment p3-p4
//****************************************************************************/
bool intersect( point p1, point p2, point p3, point p4 )
{

long long int dir1 = 0;
long long int dir2 = 0;

//Because I am providing a line that has maximum and minimum values
//I can know that if p1-p2( maximum height line ) is straddled by p3-p4
//there is an intersection
dir1 = direction( p1, p2, p3 );
dir2 = direction( p1, p2, p4 );

if( dir1 * dir2 > 0 )
{
	return false;
}
else if ( dir1 * dir2 < 0 )
{
	return true;	
}
else
{
	return true;
}

}

//****************************************************************************/
//Function: findy
//Author: Jared Johnson
//Description: function findy determines the y value of the point of
//             intersection between two line segments.
//Parameters:
//    double x: x value for our point of intersection/ cut
//    point p1: point p1 on line segment intersected by cut
//    point p2: point p2 on line segment intesected by cut
//****************************************************************************/
double findy( double x, point p1, point p2 )
{

//Return a y value on the cut segment by using triangle ratio's
return (((( x - p1.x ) * ( p2.y - p1.y )) / ( p2.x - p1.x )) + p1.y);

}

//****************************************************************************/
//Function: precisionCheck
//Author: Jared Johnson
//Description: function precisionCheck determines how close the areas a and
//             b are to each other, if the areas are accurate enough return
//             true.
//Parameters:
//    double a: first area, our current area
//    double b: second area, the goal
//****************************************************************************/
bool precisionCheck( double a, double b )
{
//cout << "A" << a << " B " << b << endl;
//cout << a-b << endl;
//Check the absolute difference between our current area and goal area	
//Compare accuracy and return true if accurate enough
if( fabs( a - b ) >= 0.00001 )
{
	return false;
}
return true;

}
