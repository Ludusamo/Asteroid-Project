#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string output;

int main() {
	string buffer;
	ifstream fin("aluminum-ambient2.dat");
	ofstream fout("ExcelReady/aluminum-ambient2");
	while (getline(fin, buffer)) {
		output = "";
		if (buffer[0] != ' ') {
			for (int i = 3; i < buffer.length(); i++) {
				if (buffer[i] != '{' && buffer[i] != '}')
					output += buffer[i];
			}
			fout << output << endl;
		}
	}
	fin.close();
	fout.close();

	return 0;
}