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






void setInfo(struct header h){

	//Modify timestamp in extracted files


	if(h.Type_flag[0]=='2'){
		struct timeval tv[2];
		tv[0].tv_sec=octalToDecimal(h.Last_modification_time_in_numeric_Unix_time_format_OctalB);
		tv[0].tv_usec=0;
		tv[1]=tv[0];
		lutimes(h.File_name,tv);
	}
	else{
		struct utimbuf tb;
		tb.actime=octalToDecimal(h.Last_modification_time_in_numeric_Unix_time_format_OctalB);
		tb.modtime=tb.actime;
		utime(h.File_name,&tb);

	}


	//Change group permissions - same procedure for all files
	chown(h.File_name,h.Owner_user_name,h.Owner_group_name);
}


void setInfoAll(int source_fd, struct header h){
	lseek(source_fd,0,SEEK_SET); //move back to beginning of tar file.
	long fileSize = 0;
	int lectureHeader = read(source_fd,&h,512);

	while(lectureHeader != 0){
		fileSize = octalToDecimal(h.File_size_in_bytes_octalB);
		if(strlen(h.File_name)!=0)
			setInfo(h);

		//and read it
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

	//Modify timestamp in extracted files
	struct utimbuf t;
	t.actime=octalToDecimal(h.Last_modification_time_in_numeric_Unix_time_format_OctalB);
	t.modtime=t.actime;

	utime(h.File_name,&t);

	//Change group permissions - same procedure for all files
	chown(h.File_name,h.Owner_user_name,h.Owner_group_name);


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


