#include "File.h"

void File::write(string text) {
	this->file.open(FileName, ios::out | ios::in);
	this->file << text;
	this->file.close();
}

string File::read() {
	string text;
	this->file.open(this->FileName, ios::out | ios::in);
	this->file >> text;
	this->file.close();
	return text;
}

void File::open(string filename) {
	this->FileName = filename;
}