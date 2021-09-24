//
//  extents.cpp
//  This code determines if a given input number falls within different ranges of values
//
//  Created by Dwi Riyanti on 07/05/18.
//  Copyright (c) 2018 Dwi Riyanti. All rights reserved.
//
#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>

/*
 * main program
 */
int main ()
{
    using namespace std;

    // Define all paramaters
    int first(0), second(0);
    vector<int> lower;
    vector<int> upper;
    vector<int> point;
    vector<int>:: iterator numElement_up;
    vector<int>:: iterator numElement_low;
    unsigned long int Total;

    //Open the extents and numbers files
    fstream  file_range, file_num;

    //Reading ranges
    file_range.open("ranges.txt", ios_base::in);

    //Reading element
    file_num.open ("numbers.dat", ios_base::in);

    // Check for error
    if (file_range.fail() && file_num.fail()){
        cerr << "Error opening files" << endl;
        exit(1);
    }

    //Read an input file of ranges until you have reached the end
    if (file_range.good()){
        while (file_range >> first >> second)
        {
            lower.push_back(first);
            upper.push_back(second);
        }
    }
    file_range.close();

    //Read a file of input values until you have reached the end
    if (file_num.good()){
        while (file_num >> first) point.push_back(first);
    }
    file_num.close();

    //Sorting the lower and upper range
    sort (lower.begin(), lower.end());
    sort (upper.begin(), upper.end());

    //Open output file
    ofstream file_out("output.txt");

    //Calculate the number of valid ranges per input value (where the input value falls within a given range)
    for ( int j = 0; j < point.size(); j++ )
    {
        //Apply upper_bound and lower_bound function
        numElement_up  = upper_bound(lower.begin(), lower.end(), point[j]);
        numElement_low = lower_bound(upper.begin(), upper.end(), point[j]);

        //Total number of elements
        Total  = (numElement_up - lower.begin()) - (numElement_low - upper.begin());
        file_out << Total << endl;
        cout << "This number " << point[j] << " is " << Total << " appears "<< endl;

    }

    //Close all files
    file_out.close();

    return 0;
}