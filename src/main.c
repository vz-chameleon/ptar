#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "ptar.h"


int main(int argc, char** argv){
  if (argc<2){
    printf("Error : ptar command requires one or more arguments \nIf you need help, try running ptar with -h option \n");
    exit(1);
  }
  
  char fileName[]="";
  strcpy(fileName,argv[argc-1]);
  int fd=open(fileName,O_RDONLY,0);
  
  struct header file_header;

  displayFileNames(fd,file_header);

  
  return 0;
} 
