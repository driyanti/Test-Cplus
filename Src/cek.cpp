//============================================================================
// Name        : main.cpp
// Author      : Christina dwiriyanti
// Version     : v01
// Copyright   : D3I-solutions
// Description : The number of ranges that contains the given point in numbers.dat
//============================================================================
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <stdio.h>
#include <algorithm>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <chrono>

/**
 * read data from file
 */

int rowA = 0;
int colA = 0;
using namespace std;


int main ()
{
    string lineA, lineB;
    int x, xx, yy;
    int arrayA[500000][2] = {0};
    int arrayB[500000] = {0};

    //int **tmp;
    //tmp = new int*[200000];

    // Open the input files
    fstream  f1, f2;
    f1.open("ranges.txt", ios::in);

    f2.open("numbers.dat", ios_base::in);

    cout << "after opening filename" << endl;

    // Check for error
    if (f1.fail() && f2.fail()){
        cerr << "Error opening file" << endl;
        exit(1);
    }

    if (f1.good()){
        while(getline(f1, lineA)){
            istringstream streamA(lineA);
            colA=0;
            while(streamA >> x) {
                arrayA[rowA][colA] = x;
                colA++;
            }
            rowA++;
        }
    }

    //Read a file until you have reached the end
    int count = 0;
    while (f2 >> xx) {
        arrayB[count] = xx;
        count ++;
    }

    if (false) {
        for (int i=0; i<rowA; i++){
            for (int j=0; j<colA; j++){
                cout << left << setw(3) << arrayA[i][j] << " " ;
            }
            cout << endl;
        }
        for (int j=0; j<count; j++){
            cout << left << setw(2) << arrayB[j] << " " ;
        }
        cout << endl;
    }

    //Now we call the sort function

    cout << "rowA:" << rowA << endl;
    cout << "colA:" << colA << endl;
    cout << "Number:" <<  count << endl;
    ofstream outfile;// declaration of file pointer named outfile
    outfile.open("output.dat", ios::out); // opens file named "filename" for output
    for ( int j = 0; j < 10; j++ ) //loop numbers
    {
        int output=0;
        for ( int i = 0; i < 10; i++ ) // loop intervals
        {
            if (arrayB[j] <= arrayA[i][1] && arrayB[j]>=arrayA[i][0] )
            {
                output++;
            }
        }
        arrayA[j][0]=output;
        if (j >= count-10) {
            cout << "This number " << arrayB[j] << " appears " << arrayA[j][0] << " times in the list of ranges" << endl;
        }
        outfile << setw(2) << arrayA[j][0] << " ";

    }

    outfile.close();// closes
  
    cout << " Done " << endl;
    
    // close files
    
    f1.close();
    f2.close();
    
    return 0;
}
