#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class File
{
private:
	fstream file;
	string FileName;
	
public:
	void write(string text);
	string read();
	void open(string filename);
};

