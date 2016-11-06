#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#include "ptar.h"


int main(int argc, char** argv){
  if (argc<2){
    printf("Error : ptar command requires one or more arguments \nIf you need help, try running ptar with -h option \n");
    exit(1);
  }
    
  int opt=0;
  int hflag=0;
  int lflag=0;
  int xflag=0;
  int zflag=0;
  int pflag=0;
  
  while((opt=getopt(argc,argv,"lxzph:"))!=-1){
    switch(opt){
    case 'h':
      hflag=1;
      break;
     
    case 'l':
      lflag=1;
      break;

    case 'x':
      xflag=1;
      break;

    case 'z':
      zflag=1;
      break;

    case 'p':
      pflag=1;
      break;

    case '?':
      if (optopt == 'p')
        fprintf(stderr,"Option -%c requires an argument.\n", optopt);
      else if (isprint(optopt))
        fprintf(stderr,"Unknown option '-%c'.\n", optopt);
      else
        fprintf(stderr,"Unknown option character '\\x%x'.\n",optopt);

      return 1;

    default:
      abort();
  }

  }
  if(hflag){
    puts("\nptar command can list and extract the contents (directories and files) of a tar archive in the current directory.");
    puts("Its default behaviour is to simply list the contents of a tar file. \nTo do so, please execute ./ptar archive.tar ");
    puts("To have a complete listing, use the -l option : ./ptar -l archive.tar ");
    puts("To extract the files listed by default, use the -x option: ./ptar -x archive.tar");
    puts("All options can be combined");
    puts(" ");
    puts("Thank you for using ptar !");
    puts(" ");
  }

  else{
    
    //char fileName[strlen(argv[argc-1])];
    //strcpy(fileName,argv[argc-1]);
    
    int fd=open(argv[argc-1],O_RDONLY,0);
  
    struct header file_header;
    
    if ((!lflag)&&(!xflag)&&(!zflag)&&(!pflag)){
        //if there is no option, display the simple listing (default behaviour)
        displayFileNames(fd,file_header);
    }
  
    else if(lflag)
      //TODO: complete this function in display.c
      displayDetailedListing(fd,file_header);
  
  }
  
  return 0;
} 
