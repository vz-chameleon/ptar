/*
 * ptar.h
 *
 *  Created on: Oct 23, 2016
 *      Author: Valentina Zelaya et Timothy Garwood
 */


#ifndef PTAR_H_
#define PTAR_H_


/****** Définition des structures utilisées*/


/*Structure for tar header in ustar format*/

struct header{
	char File_name[100]; 												//100
	char File_mode[8];													//108
	char Owner_s_numeric_user_ID[8];									//116
	char Group_s_numeric_user_ID[8];									//124
	char File_size_in_bytes_octalB[12];									//136
	char Last_modification_time_in_numeric_Unix_time_format_OctalB[12];	//148
	char Checksum_for_header_record[8];									//156
	char Type_flag[1];													//157
	char Name_of_linked_file[100];										//257
	char UStar_indicator[6];											//263
	char UStar_version[2];												//265
	char Owner_user_name[32];											//297
	char Owner_group_name[32];											//329
	char Device_major_number[8];										//337
	char Device_minor_number[8];										//345
	char Filename_prefix[155];											//500
	char rest_up_to_512[12];											//512
	};

#include <semaphore.h>
#include <pthread.h>
extern int multithread_mode;
extern int num_threads;
extern sem_t thread_semaphore;
extern int * numbers;
extern pthread_mutex_t *mutex;
extern struct header *headers;
extern pthread_t * tid;
extern char *archive;





/****** Functions in ptar.c ******/

/* octalString: string that represents file size in octal base
 *
 * Converts a number (described by a String) from octal to decimal base
 */
long octalToDecimal(char* octalString);


/*source_fd: file descriptor of a tar archive.
 * header h: header structure to read into from tar archive
 *
 * This function is called by main() when -x option is used and -p is not
 * It loops through all files in the archive and calls upon
 * extract(int source_fd, struct header h,long fileSize) function (below)
 * to extract files, directories and symlinks into working directory.
 *
 */
void extractAll(int source_fd, struct header h);


/* source_fd: file descriptor of a tar archive.
 * header h: header structure that contains the file's to extract specifications
 * fileSize: size of the file to extract
 *
 * Extracts files, directories and symlinks into working directory.
 * It uses the file header stored in 'h' for information about filetype, name ...
 * It uses the Cursor associated with the source_fd file descriptor to know
 * where to extract the data from.
 *
 */
void extract(int source_fd, struct header h,long fileSize);


/** source_fd: file descriptor of a tar archive.
 * header h: header structure to read into from tar archive
 *
 * This function is the thread-compatible version of 'extractAll'
 * This function is called by main() when -x AND -p options are used.
 * It loops through all files in the archive, creating directories
 * when encountered, and calls upon * thread_extract(void *) function (below)
 * in a new thread to extract files and symlinks.
 * It uses the global instances (initialised in main.c on usage of -p option)
 * 		- sem_t thread_semaphore;
 *  	- int * numbers;
 *		- extern struct header *headers;
 * 		- pthread_mutex_t *mutex;
 * 		- pthread_t * tid;
 *
 * 		in order to control threads and avoid data access conficts.
 *
 */
void thread_extractAll(int source_fd, struct header h);

/** i: int pointer, representing the current thread in all the global instances
 * mentioned above where it stores its data.
 *
 *
 * Extracts a normal file or a symlink into working directory into
 * the right folder.
 *
 * Calls upon fsync to make sure the data is physically extracted on the disk
 *
 * Frees its mutex and then a unit of the semaphore before ending.
 *
 */
void thread_extract_file_symlink(void * i);


/****** Functions in display.c ******/

void displayFileNames(int fd,struct header file_header);

void displayDetailedListing(int fd,struct header file_header);

#endif /* PTAR_H_ */
