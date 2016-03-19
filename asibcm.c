#include <stdio.h>

typedef enum { false, true } bool;

int size(char * x) {
	int i;
	for (i = 0; x[i] != 0; i++);
	return i;
}

char * exten(char * x) {
	int track; //size of extension
	for (int i = size(x)-1; (x[i] != '.') || (x[i] > 0); i--) {
		track++;
	}
	char * ext[track];
	int sz = size(x);
	int j = 0;
	for (int i = sz - track; i < sz; i++) {
		ext[j] = x[i];
		j++;
	}
	ext[track-1] = '\0';
	return ext;
}

int main(int argc, char * argv[]) {
	bool (*files)[argc];
	for (int i = 0; i < argc; i++) {
		files[i] = false;
	}
	bool fileGiven = false;
	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			if (argv[i][0] == '-');
			else {
				files[i] = true;
				fileGiven = true;
			}
		}
	}

	if (!fileGiven) {
		printf("usage: %s files",argv[0]);
		return 1;
	}

	return 0;
}
