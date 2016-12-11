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
  
  while((opt=getopt(argc,argv,"lxhzp:"))!=-1){
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
      num_threads=atoi(optarg);
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
    archive = malloc((strlen(argv[argc-1])+1)*sizeof(char));
    strcpy(archive,argv[argc-1]);
    int fd=open(archive,O_RDONLY,0);
  
    struct header file_header;


    //TODO: Think about the order of the flag verfication


    if(lflag)
      displayDetailedListing(fd,file_header); 	//if there is 'l' option, display the detailed listing
    
    else{
    	displayFileNames(fd,file_header);        //if there is no 'l' option, display the simple listing (default behaviour)
    }

    if (pflag){
    	multithread_mode=1;
    	sem_init(&thread_semaphore,0,num_threads);

    	numbers = malloc(sizeof(int)*num_threads);
    	headers=malloc(512*num_threads);
    	mutex = malloc(sizeof(pthread_mutex_t)*num_threads);
    	tid = malloc(sizeof(pthread_t)*num_threads);
    	int i;
    	for (i=0; i< num_threads; i++){
    		numbers[i]=i;
    		pthread_mutex_init(&mutex[i],NULL);
    	}



    	//TODO: set number of threads to use
    	//puts("thread setting : ");
    	//puts("number");

    }else{
    	multithread_mode=0;
    }

    if (zflag)
    	//TODO: make sure to consider the gzip compression in our algorithms
    	puts("gzip set");


    if (xflag){
    	if (multithread_mode)
    		thread_extractAll(fd,file_header);
    	else
    		extractAll(fd,file_header);
    }

  
  }
  free(archive);
  return 0;
} 
