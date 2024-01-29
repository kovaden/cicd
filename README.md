# MapReduce example program

This program shows basic operation of map-reduce algorithm. It calcualates minimum required profix length to identify astring in an array.

Build:
1. mkdir build
2. cd build
3. cmake ..
4. cmake --build .

Usage:

    ./mapreduce <datafile> <nmappers> <nreducers>

Where:
    datafile is a sample text file, each line contains one name
    nmappers - number of map threads
    nreducers - number of reduce threads

To create a test file with names a python generator can be used. It makes test data file of size ~1.3 unique names, randomly distributed.
To create test data:

    python names.py

   