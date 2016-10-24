#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "ptar.h"


int main(int argc, char** argv){
	initialisation();
	if (argc==1)
		printf("ERROR : Not enough arguments");
	else if (argc==2){

		char file[] ="";
		strcpy(file,argv[1]);
		int fd = open(file, O_RDONLY);

		importInformation(fd);
		displayFileNames();
		//displayFileData();


		/*
		int rh = readHeaderIntoNode(fd,currentNode);
		int rd = readDataIntoNode(fd,currentNode);

		printf("rh : %d\n rd: %d",rh,rd);
		printf("data : \n %s",currentNode->data);
*/


	}

	return 0;
} 
