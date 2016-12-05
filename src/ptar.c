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
#include <sys/time.h>
#include <sys/types.h>
#include <sys/param.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <utime.h>

#include <fcntl.h>

#include "ptar.h"

char * archive;									//archive name, initialised on launch

/* *
 *  The following variable declared are
 * initialised in main.c, IF -p option
 * is specified.
 * */
int multithread_mode;							// =1 if mode active, else =0
int num_threads;								//number of threads chosen at program startup
sem_t thread_semaphore; 						//semaphore to know if a thread is available (initialised with num_threads)
int * numbers;									//array of indices, ranging from 0 to num_threads (excluded)
pthread_mutex_t * mutex;						//array of num_threads mutexes
struct header * headers;						//array of num_threads headers
pthread_t * tid;								//array of num_threads pthreads




long octalToDecimal(char * octalString){

	long oct = atol(octalString);
	long dec=0;
	int i=0;

	while(oct!=0){
		dec+= (oct%10)* pow(8,i);
		++i;
		oct=oct/10;
	}
	return dec;
}



void setInfo(struct header* h){

	//Modify timestamp in extracted files


	if(h->Type_flag[0]=='2'){
		struct timeval tv[2];
		tv[0].tv_sec=octalToDecimal(h->Last_modification_time_in_numeric_Unix_time_format_OctalB);
		tv[0].tv_usec=0;
		tv[1]=tv[0];
		lutimes(h->File_name,tv);
	}
	else{
		struct utimbuf tb;
		tb.actime=octalToDecimal(h->Last_modification_time_in_numeric_Unix_time_format_OctalB);
		tb.modtime=tb.actime;
		utime(h->File_name,&tb);

	}


	//Change user and group owner- same procedure for all files
	lchown(h->File_name,strtol(h->Owner_s_numeric_user_ID,NULL,0),strtol(h->Group_s_numeric_user_ID,NULL,0));
}



void setInfoAll(int source_fd, struct header h){
	lseek(source_fd,0,SEEK_SET); //move back to beginning of tar file.
	long fileSize = 0;
	int lectureHeader = read(source_fd,&h,512);

	while(lectureHeader != 0){
		fileSize = octalToDecimal(h.File_size_in_bytes_octalB);
		if(strlen(h.File_name)!=0)
			setInfo(&h);
		//If the file contains data, skip to the next file header
		if(fileSize!=0){

			if (fileSize%512){
				fileSize += 512 - (fileSize % 512);
			}
			lseek(source_fd,fileSize,SEEK_CUR);
		}

		//and read next header
		lectureHeader = read(source_fd,&h,512);
	}
}




void extract(int source_fd, struct header h,long fileSize){
	//If it's a normal file...
	if (h.Type_flag[0]=='0'){
		int destination=open(h.File_name,O_CREAT|O_WRONLY,strtoul(h.File_mode,0,8));
		if (destination == -1) {
			printf("Erreur dans l'ouverture du fichier de destination pour %s", h.File_name);
			close(destination);
			exit(EXIT_FAILURE);
		}
		//sendfile writes data from source file descriptor to destination file descriptor
		int writing = sendfile(destination, source_fd, NULL, fileSize);

		if (writing == -1) {
			printf("Erreur dans l'ecriture du fichier de destination pour %s", h.File_name);
			close(destination);
			exit(EXIT_FAILURE);
		}


		fsync(destination);			//ensure data is on disk
		close(destination);

		//if the file's size isn't a multiple of 512 then place the cursor
		//after the end of file's data's last bloc
		if (fileSize%512)
			lseek(source_fd, 512-fileSize%512 ,SEEK_CUR);
	}
	//if file is a directory just create one (no need to replace the cursor)
	else if (h.Type_flag[0]=='5'){
		mkdir(h.File_name,strtoul(h.File_mode,0,8));
	}
	//if file is a symlink just create one (no need to replace the cursor)
	else if (h.Type_flag[0]=='2'){
		symlink(h.Name_of_linked_file,h.File_name);

	}
	else{
		puts("File type unknown !");
	}
}



void extractAll(int source_fd, struct header h){
	lseek(source_fd,0,SEEK_SET); //move back to beginning of tar file.
	long fileSize = 0;
	int lectureHeader = read(source_fd,&h,512);

	while(lectureHeader != 0){
		fileSize = octalToDecimal(h.File_size_in_bytes_octalB);
		if(strlen(h.File_name)!=0)
			extract(source_fd,h,fileSize);

		//and read it
		lectureHeader = read(source_fd,&h,512);
	}
	setInfoAll(source_fd,h);
}



off_t * offset;							//array of num_threads cursors offsets, initialised in tread_extractAll

void thread_extract_file_symlink(void* i){

	struct header* h = &headers[*(int *) i]; 		//this threads assigned header
	int fileSize = octalToDecimal(h->File_size_in_bytes_octalB);


	if (h->Type_flag[0]=='0'){		//If it's a normal file...
		int source = open(archive,O_RDONLY); 	//open source tar
		if (source == -1) {
					printf("Erreur dans l'ouverture du tar source pour %s", h->File_name);
					close(source);
					exit(EXIT_FAILURE);
				}

		int destination=open(h->File_name,O_CREAT|O_WRONLY,strtoul(h->File_mode,0,8)); // create destination file
		if (destination == -1) {
			printf("Erreur dans l'ouverture du fichier de destination pour %s", h->File_name);
			close(destination);
			exit(EXIT_FAILURE);
		}

		//sendfile writes data() from source file descriptor (starting from offset in 3rd argument)
		//to destination file descriptor
		int writing = sendfile(destination, source, &offset[*(int *) i], fileSize);


		if (writing == -1) {
			printf("Erreur dans l'ecriture du fichier de destination pour %s", h->File_name);
			close(destination);
			exit(EXIT_FAILURE);
		}

		fsync(destination);	//make sure the data is on disk
		close(destination);
		close(source);

	}
	//if file is a symlink just create one;
	else if (h->Type_flag[0]=='2'){
		symlink(h->Name_of_linked_file,h->File_name);

	}



	pthread_mutex_unlock(&mutex[*(int *)i]);	//unlock the thread, and associated data spaces
	sem_post(&thread_semaphore);				/*increment semaphore, effectively notifying parent program
												 of thread availability*/
}


void thread_extractAll(int source_fd, struct header h){
	lseek(source_fd,0,SEEK_SET); 					//move back to beginning of tar file.
	long fileSize = 0;
	int continueToRead = 1;
	offset = malloc(num_threads*sizeof(int));		//initialising offset array with proper size

	int i;


	while(continueToRead != 0){						//note : initialied with 1
		sem_wait(&thread_semaphore);
		for (i=0; (i<num_threads);i++){
			if ((pthread_mutex_trylock(&mutex[i])==0)){	//if mutex i unlocked (index available for all arrays)

				continueToRead = read(source_fd,&headers[i],512);	//copying header into headers[i]

				if (atol(headers[i].Checksum_for_header_record)==0){//if end-of-tar empty block reached
					continueToRead=0;
					pthread_mutex_unlock(&mutex[i]);
					sem_post(&thread_semaphore);
					//last locked mutex unlocked

					int j;
					for (j=0; j<num_threads;j++)
						pthread_join(tid[i], NULL); 	//making sure all threads terminate before exiting while loop
					break;
				}else{
				fileSize=octalToDecimal(headers[i].File_size_in_bytes_octalB);
				offset[i]=SEEK_CUR;						//copying offset into offset[i]


					if (headers[i].Type_flag[0]!='5'){	// if it's NOT a folder
						pthread_create( &tid[i], NULL, (void*)&thread_extract_file_symlink, (void*)&numbers[i]);	/*then
																							extracf this file or symlink*/

						//if there is data, place the cursor
						//after the end of file's data's last bloc
						if (fileSize!=0){
							if (fileSize%512){
								lseek(source_fd, fileSize-fileSize%512+512 ,SEEK_CUR);
							}else{
								lseek(source_fd, fileSize ,SEEK_CUR);
							}
						}

					}else{						//if file is a directory just create one (no need to replace the cursor)
						pthread_mutex_unlock(&mutex[i]); //we dont need the mutex and ressources anymore
						sem_post(&thread_semaphore);

						mkdir(headers[i].File_name,strtoul(headers[i].File_mode,0,8)); //make the directory
					}
				}
				i=num_threads;
			}
		}
	}
	setInfoAll(source_fd,h);					//set the correct timestamps and owners for everything

}


