#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
using namespace std;

int find(vector<string> & vec, string & x) {
	for (unsigned i = 0; i < vec.size(); i++) {
		if (vec[i].compare(x) == 0)
			return i;
	}
	return -1;
}

string hex(int x) {
	char size = 17;
	char ret[size];
	int r;
	int ind = 0;
	while (x > 0) {
		r = x % 16;
		x /= 16;
		if (r < 10)
			ret[ind] = 48 + r;
		else ret[ind] = 65 - 10 + r;
		ind ++;
		ret[ind] = 0;
	}
	char flip[size];
	for (int i = 0; i < size; i++) {
		ind--;
		flip[i] = ret[ind];
	}
	string conv = flip;
	if (conv.size() == 2)
		conv = "0" + conv;
	else if (conv.size() == 1)
		conv = "00" + conv;
	else if (conv.size() == 0)
		conv = "000";
	return conv;
}

int main (int argc, char * argv[]) {

//	string ** files = new string * [argc];
	
	string ** extns = new string * [argc];
	int ibcm = 0;

	for (int i = 0; i < argc; i++) {
		string conv = (string) argv[i];
		int extInd = conv.find_last_of('.');
		extns[i] = new string;
		for (int j = extInd + 1; argv[i][j] != '\0'; j++) {
			*extns[i] += argv[i][j];
		}
		if (*extns[i] == "ibcm") {
			ibcm = i;
//			printf("%s\n", extns[i]->c_str());
		}
	}

	if (!ibcm) {
		fprintf(stderr, "usage: %s file.ibcm\n",argv[0]);
		fprintf(stderr, "error: none of the files have .ibcm extension\n");
		return 1;
	}

	vector<string> labels;

	ifstream f;
	f.open(argv[ibcm]);
	
	for (int i = 1;;i++) {
		string line;
		getline(f,line);
		if (f.eof())
			break;
		string thisLabel = line.substr(0,line.find('\t'));
		if (thisLabel.size() > 0) {
			if (find(labels, thisLabel) != -1) {
				fprintf(stderr, "error: in line %d of %s\n",i,argv[ibcm]);
				fprintf(stderr, "\tmultiple definitions of label \'%s\'\n",thisLabel.c_str());
				fprintf(stderr, "terminating...\n");
				return 0;
			}
			//printf("%s\n", thisLabel.c_str());
		}
		labels.push_back(thisLabel);
//		printf("%d, \'%s\'\n",i-1, labels[i-1].c_str());
	}

	f.close();

	f.open(argv[ibcm]);

	for (int i = 0;; i++) {
		string line;
		getline(f,line);
		if (f.eof())
			break;
		
		string backline = line.substr(1 + line.find('\t'),line.size() - line.find('\t') - 1);
//		printf("%s\n",backline.c_str());

		string command;
		for (int j = 0; backline[j] != 0; j++) {
			if (((backline[j] >= 97) && (backline[j] <= 123)) || ((backline[j] >= 65) && (backline[j] <= 90)))
				command += backline[j];
			else break;
		}

		string address;
		for (int j = backline.find_first_of('\t') + 1; backline[j] != 0; j++) {
			if (((backline[j] >= 97) && (backline[j] <= 123)) || ((backline[j] >= 65) && (backline[j] <= 90)) || ((backline[j] >= 48) && (backline[j] <= 57)))
				address += backline[j];
			else break;
		}

		//cout << command << endl;

		if (command == "halt")
			cout << "0000\t" << hex(i) << '\t' << line << endl;
		else if (command == "readH")
			cout << "1000\t" << hex(i) << '\t' << line << endl;
		else if (command == "readC")
			cout << "1400\t" << hex(i) << '\t' << line << endl;
		else if (command == "printH")
			cout << "1800\t" << hex(i) << '\t' << line << endl;
		else if (command == "printC")
			cout << "1C00\t" << hex(i) << '\t' << line << endl;
		else if (command == "shiftL")
			cout << "2000\t" << hex(i) << '\t' << line << endl;
		else if (command == "shiftR")
			cout << "2400\t" << hex(i) << '\t' << line << endl;
		else if (command == "rotL")
			cout << "2800\t" << hex(i) << '\t' << line << endl;
		else if (command == "rotR")
			cout << "2C00\t" << hex(i) << '\t' << line << endl;
		else if (command == "load")
			if (address.size() != 0) {
				if (-1 < find(labels,address))
					cout << '3' << hex(find(labels,address)) << '\t' << hex(i) << '\t' << line << endl;
				else {
					fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
					fprintf(stderr, "\tundefined label \'%s\'\n",address.c_str());
				}
			}
			else {
				fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
				fprintf(stderr, "\texpected label after command \'load\'\n");
			}
		else if (command == "store")
			if (address.size() != 0) {
				if (-1 < find(labels,address))
					cout << '4' << hex(find(labels,address)) << '\t' << hex(i) << '\t' << line << endl;
				else {
					fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
					fprintf(stderr, "\tundefined label \'%s\'\n",address.c_str());
				}
			} else {
				fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
				fprintf(stderr, "\texpected label after command \'store\'\n");
			}
		else if (command == "add")
			if (address.size() != 0) {
				if (-1 < find(labels,address))
					cout << '5' << hex(find(labels,address)) << '\t' << hex(i) << '\t' << line << endl;
				else {
					fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
					fprintf(stderr, "\tundefined label \'%s\'\n",address.c_str());
				}
			} else {
				fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
				fprintf(stderr, "\texpected label after command \'add\'\n");
			}
		else if (command == "sub")
			if (address.size() != 0) {
				if (-1 < find(labels,address))
					cout << '6' << hex(find(labels,address)) << '\t' << hex(i) << '\t' << line << endl;
				else {
					fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
					fprintf(stderr, "\tundefined label \'%s\'\n",address.c_str());
				}
			} else {
				fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
				fprintf(stderr, "\texpected label after command \'sub\'\n");
			}
		else if (command == "and")
			if (address.size() != 0) {
				if (-1 < find(labels,address))
					cout << '7' << hex(find(labels,address)) << '\t' << hex(i) << '\t' << line << endl;
				else {
					fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
					fprintf(stderr, "\tundefined label \'%s\'\n",address.c_str());
				}
			} else {
				fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
				fprintf(stderr, "\texpected label after command \'and\'\n");
			}
		else if (command == "or")
			if (address.size() != 0) {
				if (-1 < find(labels,address))
					cout << '8' << hex(find(labels,address)) << '\t' << hex(i) << '\t' << line << endl;
				else {
					fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
					fprintf(stderr, "\tundefined label \'%s\'\n",address.c_str());
				}
			} else {
				fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
				fprintf(stderr, "\texpected label after command \'or\'\n");
			}
		else if (command == "xor")
			if (address.size() != 0) {
				if (-1 < find(labels,address))
					cout << '9' << hex(find(labels,address)) << '\t' << hex(i) << '\t' << line << endl;
				else {
					fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
					fprintf(stderr, "\tundefined label \'%s\'\n",address.c_str());
				}
			} else {
				fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
				fprintf(stderr, "\texpected label after command \'xor\'\n");
			}
		else if (command == "not")
			cout << "A000\t" << hex(i) << '\t' << line << endl;
		else if (command == "nop")
			cout << "B000\t" << hex(i) << '\t' << line << endl;
		else if (command == "jmp")
			if (address.size() != 0) {
				if (-1 < find(labels,address))
					cout << 'C' << hex(find(labels,address)) << '\t' << hex(i) << '\t' << line << endl;
				else {
					fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
					fprintf(stderr, "\tundefined label \'%s\'\n",address.c_str());
				}
			} else {
				fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
				fprintf(stderr, "\texpected label after command \'jmp\'\n");
			}
		else if (command == "jmpe")
			if (address.size() != 0) {
				if (-1 < find(labels,address))
					cout << 'D' << hex(find(labels,address)) << '\t' << hex(i) << '\t' << line << endl;
				else {
					fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
					fprintf(stderr, "\tundefined label \'%s\'\n",address.c_str());
				}
			} else {
				fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
				fprintf(stderr, "\texpected label after command \'jmpe\'\n");
			}
		else if (command == "jmpl")
			if (address.size() != 0) {
				if (-1 < find(labels,address))
					cout << 'E' << hex(find(labels,address)) << '\t' << hex(i) << '\t' << line << endl;
				else {
					fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
					fprintf(stderr, "\tundefined label \'%s\'\n",address.c_str());
				}
			} else {
				fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
				fprintf(stderr, "\texpected label after command \'jmpl\'\n");
			}
		else if (command == "brl")
			if (address.size() != 0) {
				if (-1 < find(labels,address))
					cout << 'F' << hex(find(labels,address)) << '\t' << hex(i) << '\t' << line << endl;
				else {
					fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
					fprintf(stderr, "\tundefined label \'%s\'\n",address.c_str());
				}
			} else {
				fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
				fprintf(stderr, "\texpected label after command \'brl\'\n");
			}
		else if (command == "dw") {
			if (address.size() != 0) {
				if (-1 == find(labels,address)) {
					int value;
					sscanf(address.c_str(), "%x", &value);
					printf("%04x", value);
					cout << '\t' << hex(i) << '\t' << line << endl;
				} else {
					//printf("%s, %d", address.c_str(), find(labels, address));
					
					printf("%04x", find(labels, address));
					cout << '\t' << hex(i) << '\t' << line << endl;
				}
			} else {
				fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
				fprintf(stderr, "\texpected label after command \'dw\'\n");
			}
		}
		else {
			fprintf(stderr, "error: on line \'%d\' in file %s\n",i+1,argv[ibcm]);
			fprintf(stderr, "\tunknown command \'%s\'\n",command.c_str());
		}

	}

	return 0;
}
