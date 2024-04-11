/*

Compile: g++ -O2 geotiff_filemaker.cpp -o GT
RUN: ./GT

*/


// basic file operations
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atof */
#include <math.h>
#include <iomanip>

using std::vector;

using namespace std;






struct coordinatedata{
  int X;
  int Y;
  double LONGI; //X coordinate
  double LATI; //Y coordinate
  double elev;
  //double Neerslag //??????????????
} schaap[1250][1000];



//maximale grootte matrix:
const int MMAX = 1000; //column = X
const int NMAX = 1250; //row = Y


double LONG[1250][1000];
double LAT[1250][1000];
double elevation[1250][1000];


class dataset{
    public:
		dataset(string file);	//als parameter een string mee voor bestandsnaam
				
		
    private:		

};


//INITIALISATIE
dataset::dataset(string file){	
	//Variables:
	int i;
	int j;
	int p;
	int q;
	//cout << "Hallo?" << endl;
	//Counter coordinates:
	double ULLong = 5.8971694444444;
	double ULLat = 51.982016666667;
	double URLong = 5.9699583333333;
	double URLat = 51.981677777778;
	double LLLong = 5.8965333333333;
	double LLLat = 51.925836111111;
	double LRLong = 5.9692277777778;
	double LRLat = 51.925497222222;
	
	double pixelstep_lefttorightLONG = (5.9699583333333 - 5.8971694444444) / 1000;
	double pixelstep_lefttorightLAT = (51.981677777778 - 51.982016666667) / 1000;
	double pixelstep_toptobottomLONG = (5.8965333333333 - 5.8971694444444) / 1250;
	double pixelstep_toptobottomLAT = (51.925836111111 - 51.982016666667) / 1250;


	//2D Array = int bla[rows][columns]
	
	
	cout << "Hallo?" << endl;
  
	//Longitude 
	for(p=0; p<NMAX; p++){
		for(q=0; q<MMAX; q++){
			if((LONG[p][q] == 0) || (LAT[p][q] == 0)){
			
				if((p == 0) && (q == 0)){
					LONG[p][q] = ULLong;
					LAT[p][q] = ULLat;
				}
				else if((p == 0) && (q == MMAX-1)){
					LONG[p][q] = URLong;
					LAT[p][q] = URLat;
				}
				else if((p == NMAX-1) && (q == 0)){
					LONG[p][q] = LLLong;
					LAT[p][q] = LLLat;
				}
				else if((p == NMAX-1) && (q == MMAX-1)){
					LONG[p][q] = LRLong;
					LAT[p][q] = LRLat;
				}
				
				
				else{
					if(p == 0){
						LONG[p][q] = LONG[p][q-1] + pixelstep_lefttorightLONG;
						LAT[p][q] = LAT[p][q-1] + pixelstep_lefttorightLAT;
					}
					else if(q == 0){
						LONG[p][q] = LONG[p-1][q] + pixelstep_toptobottomLONG;
						LAT[p][q] = LAT[p-1][q] + pixelstep_toptobottomLAT;
					}
					else{
						LONG[p][q] = LONG[p-1][q-1] + pixelstep_lefttorightLONG + pixelstep_toptobottomLONG;
						LAT[p][q] = LAT[p-1][q-1] + pixelstep_lefttorightLAT + pixelstep_toptobottomLAT;
					}
				}
			}
		}		
	}




///////////////////////////////////////////////////////////////////////////
  	

	ifstream invoer;
	invoer.open(file.c_str(), ios::in);
	
	int getal; //houdt het ingelezen getal tijdelijk vast.
	string banaan;
	int row = 0; //row (1250)
	int col = 0; //col (1000)
	
	if(invoer.fail()){
		cout << "Unable to open the file." << endl;
		exit(1);
	}//Geef een foutmelding als het programma niet in staat is de file te openen
	
		
	else{//lees anders de tabel in.
		while(getline(invoer, banaan)){
	        stringstream linestream(banaan);
	        string temp;
	        
	        while(getline(linestream, temp, ',')){       
		        schaap[row][col].elev = atof(temp.c_str());
		        //cout << temp << endl;
		        
		        
		        schaap[row][col].X = col;
		        
		        
		        schaap[row][col].Y = row;
		        
		        
		        schaap[row][col].LONGI = LONG[row][col];
		        
		        
		        schaap[row][col].LATI = LAT[row][col];
		        
		        
				col++;
			}
			
		
			row++;
	    	col = 0;
		}//while
	}//else
	
	
	
	ofstream myfile;
	myfile.open("geotiff.csv");

	

	for(i=0; i<NMAX; i++){
		for(j=0; j<MMAX; j++){
			//myfile << schaap[i][j].X << "," << schaap[i][j].Y << "," << schaap[i][j].LONGI << ","
			//<< schaap[i][j].LATI << "," << schaap[i][j].elev << endl;
			
			myfile << schaap[i][j].X << "," << schaap[i][j].Y << ",";
			myfile << fixed << setprecision(13) << schaap[i][j].LONGI << ",";
			myfile << fixed << setprecision(13) << schaap[i][j].LATI << ",";
			myfile << fixed << setprecision(13) << schaap[i][j].elev << endl;
		}
	}

	myfile.close();
	
}//Inlees-functie


int main () {
	//cout << "Hallo?" << endl;
	dataset banaan("test1.csv");
		
	
	
	return 0;

}






/*Converting Degrees, Minutes, Seconds (DMS) to Decimal Degrees (Longitude & Latitude):

- Degrees: Stay the same 

- Minutes: <minutes>' / 60

- Seconds: <seconds>" / 3600

- SUM: Degrees + Minutes + Seconds

_____________________________________________________
Latitude = Breedtegraad
Longitude = Lengtegraad

GDAL displays: Longitude, Latitude = X, Y
Google Maps hanteert echter: Latitude, Longitude = Y, X = Breedtegraad, Lengtegraad.

Size image = 1000 x 1250 pixels 

Corner Coordinates:
Upper Left  (  190000.000,  443750.000) (  5d53'49.81"E, 51d58'55.26"N) = 5,8971694444444 ; 51,982016666667

Lower Left  (  190000.000,  437500.000) (  5d53'47.52"E, 51d55'33.01"N) = 5,8965333333333 ; 51,925836111111

Upper Right (  195000.000,  443750.000) (  5d58'11.85"E, 51d58'54.04"N) = 5,9699583333333 ; 51,981677777778

Lower Right (  195000.000,  437500.000) (  5d58' 9.22"E, 51d55'31.79"N) = 5,9692277777778 ; 51,925497222222


Center      (  192500.000,  440625.000) (  5d55'59.60"E, 51d57'13.55"N) = 5,9332222222222 ; 51,953763888889

---------------------------
(Upper Right - Upper Left) / 1000 = Longitude stap per Pixel.

X-range per pixel from corners:
0,0727888888889 / 1000 = 7,27888888889E-05 = 0,0000727888888889 = X
-0,00033888888899725 / 1000 = -0,00000033888888899725 = Y

Zoveel naar rechts is zoveel omlaag blijkbaar 





----------------------------
(Lower Left - Upper Left) / 1000 = Latitude stap per Pixel.

Y-range per pixel from corners:
*/

