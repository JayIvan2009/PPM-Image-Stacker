#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
using namespace std;

class Image {
public:
	Image() { 
		colors = nullptr;
		colorsAverage = nullptr;
	};
	~Image() {
	};

	void menu(Image& img) {																// menu function
		ifstream inFile;
		ofstream outFile;
		string inName, outName, allName;
		bool end = true;
		int i = 1;
		string num = to_string(i);

		cout << "Portable Pixmap (PPM) Image Stacker by Jay Ivan Gentallan" << endl << endl;
		cout << "Enter image file name: ";
		cin >> inName;														// gets input for inName
		cout << endl;

		while (end) {
			if (i >= 10) {
				allName = inName + "_" + "0" + to_string(i) + ".ppm";
			}
			else {
				allName = inName + "_" + "00" + to_string(i) + ".ppm";
			}
			inFile.open(allName);

			if (!inFile) {
				if (i == 1) {
					cout << "File not found! Please try again." << endl;
					end = false;
					inFile.close();
					menu(img);
				}
				else {
					end = false;
					inFile.close();
					img.fileCount--;
				}
			}

			else {
				cout << "Reading image: " << allName << endl;
				inFile >> img;
				inFile.close();
				if (img.fileCount >= 2) {
					stack(img);
					i++;
					img.fileCount++;
				}
				else {
					base(img);
					i++;
					img.fileCount++;
				}
			}
		}
		average(img);

		outFile.open(inName + "_good.ppm");										// creates outFile
		outFile << img;														// file data is passed through class ostream& operator and put into outFile
		outFile.close();													// closes file

		cout << endl << "Press any key to continue." << endl;
		cin.ignore();
		cin.get();	

		delete[] img.colors;
		delete[] img.colorsAverage;
		img.colors = nullptr;
		img.colorsAverage = nullptr;															// exits program
	};

	void base(Image& img) {
		int *init, *initAverage;
		int size = img.width * img.height * 3;

		init = img.colors;
		initAverage = img.colorsAverage;
		for (int i = 0; i < size; i++) {	
			*img.colorsAverage = *img.colors;
			img.colorsAverage++;
			img.colors++;
		}
		img.colors = init;
		img.colorsAverage = initAverage;
	};

	void stack(Image& img) {
		int *init, *initAverage;
		int size = img.width * img.height * 3;

		init = img.colors;
		initAverage = img.colorsAverage;
		for (int i = 0; i < size; i++) {
			*img.colorsAverage += *img.colors;
			img.colorsAverage++;
			img.colors++;
		}
		img.colors = init;
		img.colorsAverage = initAverage;
	}

	void average(Image& img) {
		int *initAverage;
		int size = img.width * img.height * 3;

		initAverage  = img.colorsAverage;
		for (int i = 0; i < size; i++) {
			*img.colorsAverage /= img.fileCount;
			img.colorsAverage++;
		}
		img.colorsAverage = initAverage;
	}

	friend istream& operator >> (ifstream& inFile, Image& img) {							// gets called once inFile is streamed into the class img
		int *reset, *resetAverage;
		string fileName, myString;
		char dump;
		static bool once = true;

		inFile >> img.type;																// extracts type from file

		inFile >> myString;																// extracts width from file and puts it into the private width member
		img.width = stoi(myString.c_str());

		inFile >> myString;																// extracts height from file and puts it into the private height member
		img.height = stoi(myString.c_str());

		inFile >> myString;																// extracts max color value from file and puts it into the private maxColor member
		img.maxColor = stoi(myString.c_str());

		if (img.height > 1000 || img.width > 1000) {									// makes sure image file isn't bigger than 1000 on both dimensions
			cout << "Image is too big! Please use another one." << endl;
			img.menu(img);
		}

		int size = img.width * img.height * 3;
		if (once) {
			img.colorsAverage = new int[size];
			img.colors = new int[size];
			once = false;
		}
		reset = img.colors;															// set initial pointer for colors

		inFile.read(&dump, 1);															// ignore newline
		for (int i = 0; i < size; i++) {												// loops image file to extract each pixel data
			inFile >> *img.colors;
			*++img.colors;
		}
		img.colors = reset;															// reset pointer location

		return inFile;
	};

	friend ostream& operator << (ofstream& outFile, Image& img) {						// gets called once img is streamed out into outFile
		int *reset, *resetAverage;
		int size = img.width * img.height * 3;											// this means height and width times 3 bytes

		resetAverage = img.colorsAverage;

		outFile << img.type << '\n' << img.width << ' ' << img.height << '\n' << img.maxColor << '\n';		// puts the headers into the outFile
		for (int i = 0; i < size; i++) {												// loops colors and puts it into outFile
			outFile << *img.colorsAverage << " ";
			*++img.colorsAverage;
		}
		img.colorsAverage = resetAverage;

		cout << endl << "Completed!" << endl;
		return outFile;
	};

private:
	string type;	// type of picture
	int width;		// width of picture
	int height;		// height of picture
	int maxColor;   // max color value of picture
	int *colors, *colorsAverage;
	int fileCount = 1;
};

int main() {
	Image img;
	img.menu(img);
}