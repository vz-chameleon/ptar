/*
 * display.c
 *
 *  Created on: Oct 24, 2016
 *      Authors: Timothy Garwood and Valentina Zelaya
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ptar.h"

void displayFileNames(){
	currentNode=&node;
	for (; currentNode != NULL; currentNode = currentNode->next){
		printf("%s\n",currentNode->header.File_name);
	}

}
void displayFileData(){
	currentNode=&node;
	for (; currentNode != NULL; currentNode = currentNode->next){
		printf("%s\n",currentNode->data);
	}
}

