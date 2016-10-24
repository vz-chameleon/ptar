/*
 * ptar.c
 *
 *  Created on: Oct 23, 2016
 *      Authors: Timothy Garwood and Valentina Zelaya
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "ptar.h"



void initialisation(){
	node.next = NULL; //node is a global variable declared in ptar.h
	currentNode=&node; //currentNode is a global variable declared in ptar.h
}


int readHeaderIntoNode(int fd, struct header_linkedList_node * node){
	return read(fd,&node->header,512);
}


int readDataIntoNode(int fd, struct header_linkedList_node * node){
	int file_size= octalToDecimal(node->header.File_size_in_bytes_octalB);
	int dep = (file_size/512+1)*512;
	node->data = malloc(dep);
	int rd=read(fd,node->data,dep);
	

	return rd;
}


void importInformation(int fd){
	int lecture=readHeaderIntoNode(fd, currentNode);
	int file_size=0;
	while (lecture!=0){
		file_size = octalToDecimal(currentNode->header.File_size_in_bytes_octalB);

		if(file_size!=0){

			int dep=(file_size/512+1)*512;
			readDataIntoNode(fd, currentNode);

		}
		else{
			currentNode->data = malloc(2);
			strcpy(currentNode->data, "\n"); //TODO: trouver quoi mettre pour un fichier vide.
		}
		currentNode->next=(struct header_linkedList_node *)malloc(sizeof(struct header_linkedList_node)); 																							//switching focus to next node
		lecture=readHeaderIntoNode(fd, currentNode->next);
		if (strlen(currentNode->next->header.File_name)==0){
			currentNode->next=NULL;
			break;
		}
		currentNode=currentNode->next;


	}
}


int octalToDecimal(char * octalString){

  int oct = atoi(octalString);
  int dec=0;
  int i=0;

  while(oct!=0){
    dec+= (oct%10)* pow(8,i);
    ++i;
    oct=oct/10;
  }
  
  return dec;
}

  
