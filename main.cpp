#include <iostream>
#include <string>
#include <fstream>
#include "Data.h"
#include <vector>
#include <time.h>
#include <stdlib.h>
using namespace std;

void createOutput(const vector<Data> dataVector, string label_line);
void parseString(Data&);
void insertionSort(vector<Data>& list, char criterion);
void merge(vector<Data>& list, char criterion, int first, int middle, int last);
void mergeSort(vector<Data>& list, char criterion, int first, int last);


int main(int argc, char **argv) {
	clock_t time = clock();
	vector<Data> dataVector;
	Data inputData;
	ifstream inputFile;
	string fileData, label_line;
	char algorithm, criterion;
	int size;

	for (int i = 1; i < argc; i++) {//taking command line arguments regardless of the order
		if ((string)argv[i] == "-algo") {
			algorithm = *argv[i + 1];
			i += 2;
			if (i >= argc) break;
		}
		if ((string)argv[i] == "-feature") {
			criterion = *argv[i + 1];
			i += 2;
			if (i >= argc) break;
		}

		if ((string)argv[i] == "-size") {
			size = atoi(argv[i + 1]);
			i += 2;
			if (i >= argc) break;
		}
		i--;//if not all arguments taken, decrease i to check variable names again.
	}

	inputFile.open("log_inf.csv");

	if (inputFile.good()) {
		getline(inputFile, fileData);//keep label line.
		label_line = fileData;
		for (int i = 0; i < size; i++) {
			getline(inputFile, fileData);
			inputData.set_line(fileData);
			parseString(inputData);
			dataVector.push_back(inputData);
			if (inputFile.eof()) break;
		}
		size = dataVector.size();
	}
	inputFile.close();

	if(algorithm == 'i')
		insertionSort(dataVector, criterion);
	else if(algorithm == 'm')
		mergeSort(dataVector, criterion, 0, size - 1);

	time = clock() - time;

	if(criterion == 'p')
		cout << "Sorted " << size << " data according to last_price value in " << (float)time / CLOCKS_PER_SEC << " seconds with ";
	else if (criterion == 't')
		cout << "Sorted " << size << " data according to time_stamp value in " << (float)time / CLOCKS_PER_SEC << " seconds with ";

	if (algorithm == 'm')
		cout << "Merge Sort algorithm.";
	else if (algorithm == 'i')
		cout << "Insertion Sort algorithm.";

	createOutput(dataVector, label_line);

	return 0;
}

void parseString(Data& data) {
	string str = data.get_line();
	string input;
	unsigned int index1 = str.find_first_of(',');//find the index of the first occurence of ','.
	unsigned int index2;

	input.append(str, 0, index1);
	data.set_timeStamp(input);
	input.clear();

	index1 = str.find_first_of(',', index1 + 1);//find the index of the second occurence of ','.
	index2 = str.find_first_of(',', index1 + 1);//find the index of the third occurence of ','.

	input.append(str, index1 + 1, index2 - index1 - 1);//take the value between second ',' and third ','.
	
	data.set_lastPrice(atof(input.c_str()));
}

void insertionSort(vector<Data>& list, char criterion) {
	Data key;
	int j;
	if (criterion == 'p') {
		for (int i = 1; i < list.size(); i++) {
			key = list[i];
			j = i - 1;
			while (j >= 0 && list[j].get_lastPrice() > key.get_lastPrice()) {
				list[j + 1] = list[j];
				j--;
			}
			list[j + 1] = key;
		}
	}
	else if (criterion == 't') {
		for (int i = 1; i < list.size(); i++) {
			key = list[i];
			j = i - 1;
			while (j >= 0 && list[j].get_timeStamp() > key.get_timeStamp()) {
				list[j + 1] = list[j];
				j--;
			}
			list[j + 1] = key;
		}
	}

}

void merge(vector<Data>& list, char criterion, int first, int middle, int last) {
	int size1 = middle - first + 1;
	int size2 = last - middle;
	vector<Data> arr1, arr2;

	for (int i = 0; i < size1; i++) {
		arr1.push_back(list[first + i]);
	}

	for (int i = 0; i < size2; i++) {
		arr2.push_back(list[middle + i + 1]);
	}

	int j = 0;
	int k = 0;

	for (int i = first; i <= last; i++) {
		if (j>=size1) {
			while (k<size2) {
				list[i] = arr2[k];
				k++;
				i++;
			}
			if (i > last) break;
		}

		if (k>=size2) {
			while (j<size1) {
				list[i] = arr1[j];
				j++;
				i++;
			}
			if (i > last) break;
		}

		if (criterion == 'p') {
			if (arr1[j].get_lastPrice() < arr2[k].get_lastPrice()) {
				list[i] = arr1[j];
				j++;
			}
			else {
				list[i] = arr2[k];
				k++;
			}
		}

		else if (criterion == 't') {
			if (arr1[j].get_timeStamp() < arr2[k].get_timeStamp()) {
				list[i] = arr1[j];
				j++;
			}
			else {
				list[i] = arr2[k];
				k++;
			}
		}
	}
}

void mergeSort(vector<Data>& list, char criterion, int first, int last) {
	int middle = (last + first) / 2;
	if (last > first) {
		mergeSort(list, criterion, first, middle);
		mergeSort(list, criterion, middle + 1, last);
		merge(list, criterion, first, middle, last);
	}
}

void createOutput(const vector<Data> dataVector, string label_line) {
	ofstream output;
	output.open("sorted.csv");
	output << label_line << endl;
	for (int i = 0; i < dataVector.size() - 1; i++) {
		output << dataVector[i].get_line() << endl;
	}
	output << dataVector[dataVector.size()-1].get_line();
}