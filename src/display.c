/*
 * display.c
 *
 *  Created on: Oct 24, 2016
 *      Authors: Timothy Garwood and Valentina Zelaya
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "ptar.h"

void displayFileNames(int fd,struct header file_header){
  int size = 0;
  int lecture = read(fd,&file_header,512);
   
  while(lecture != 0){
    size = octalToDecimal(file_header.File_size_in_bytes_octalB);
    //TODO 
    //Better the condition to print or to get out of the loop
    //Compare to a block of 512 zeros ?
    if(strlen(file_header.File_name)!=0)
      printf("%s\n",file_header.File_name);

    //If the file contains data, skip to the next file header
    if(size!=0)
      lseek(fd,(size/512+1)*512,SEEK_CUR);
    //and read it
    lecture = read(fd,&file_header,512);
    
	
}

}

void displayDetailedListing(int fd,struct header file_header){
  int size = 0;
  int lecture = read(fd,&file_header,512);
   
  while(lecture != 0){
    size = octalToDecimal(file_header.File_size_in_bytes_octalB);
    //TODO 
    //Better the condition to print or to get out of the loop
    //Compare to a block of 512 zeros ?

    if(strlen(file_header.File_name)!=0)
      //TODO: create string buffer with the right information 
      printf("%s\n",file_header.File_name);

    //If the file contains data, skip to the next file header
    if(size!=0)
      lseek(fd,(size/512+1)*512,SEEK_CUR);
    //and read it
    lecture = read(fd,&file_header,512);
  
 }  
}
