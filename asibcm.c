#include <stdio.h>

typedef enum { false, true } bool;

int size(char x[]) {
	int i;
	for (i = 0; x[i] != 0; i++);
	return i;
}

int exten(char x[]) {
	int track; //size of extension
	for (int i = size(x)-1; (x[i] != '.') && (i > 0); i--) {
		track++;
	}
	return size(x) - track;
}

int main(int argc, char * argv[]) {
	bool files[argc];
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
		printf("usage: %s files\n",argv[0]);
		return 1;
	}

	for (int i = 1; i < argc; i++) {
		if (files[i]) {
			for (int j = exten(argv[i]); argv[i][j] != '\0'; j++)
				printf("%c", argv[i][j]);
			printf("\n");
		}
	}

	return 0;
}
