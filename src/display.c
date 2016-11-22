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
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "ptar.h"

void displayFileNames(int fd,struct header file_header){
	long jump_size = 0;
	int lecture = read(fd,&file_header,512);

	while(lecture != 0){
		jump_size = octalToDecimal(file_header.File_size_in_bytes_octalB);
		//TODO
		//Better the condition to print or to get out of the loop
		//Compare to a block of 512 zeros ?
		if(strlen(file_header.File_name)!=0)
			printf("%s\n",file_header.File_name);

		//If the file contains data, skip to the next file header
		if(jump_size!=0){
			
		 if (jump_size%512){
			 jump_size += 512 - (jump_size % 512);
		 }



		
			lseek(fd,jump_size,SEEK_CUR);
		}
		//and read it
		lecture = read(fd,&file_header,512);


	}

}


void setPermission(char i, char * permString){
	switch (i){
	case '0' : strcpy(permString,"---"); break;
	case '1' : strcpy(permString,"--x"); break;
	case '2' : strcpy(permString,"-w-"); break;
	case '3' : strcpy(permString,"-wx"); break;
	case '4' : strcpy(permString,"r--"); break;
	case '5' : strcpy(permString,"r-x"); break;
	case '6' : strcpy(permString,"rw-"); break;
	case '7' : strcpy(permString,"rwx"); break;
	}


}

void printPermissionString(char* file_mode){

	//printf("\n%d\n",(int)sizeof(permissionString));
	char userPerm[3]; setPermission(file_mode[4], userPerm);
	char groupPerm[3]; setPermission(file_mode[5], groupPerm);
	char allPerm[3]; setPermission(file_mode[6], allPerm);
	//printf("\n%c => %d\n", file_mode[4],atoi(&file_mode[4]));
	printf("%s",userPerm);
	printf("%s",groupPerm);
	printf("%s ",allPerm);
		//char * permissionString = "-" +userPerm+groupPerm+allPerm;
	//snprintf(permissionString, sizeof(permissionString), "%s%s%s%s", "-", userPerm, groupPerm, allPerm);
}

void printModifTime(char * path, long secSince1970){
	struct stat attr;
	stat(path,&attr);
	time_t t = secSince1970;
	struct tm *tm;
	tm=localtime(&t);


	printf("%d-%.2d-%.2d %.2d:%.2d:%.2d ",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);

}
void displayDetailedListing(int fd,struct header file_header){
	long jump_size = 0;
	int lecture = read(fd,&file_header,512);

	while(lecture != 0){
		jump_size = octalToDecimal(file_header.File_size_in_bytes_octalB);
		//TODO
		//Better the condition to print or to get out of the loop
		//Compare to a block of 512 zeros ?

		if(strlen(file_header.File_name)!=0){
			//TODO: create string buffer with the right information
			if (file_header.Type_flag[0]=='5') //printing 'd' if folder, else '-'
				printf("%s","d");
			else if (file_header.Type_flag[0]=='2') //printing 'l' if symlink, else '-'
				printf("%s","l");
			else printf("%s","-");
			//printf("%s ",PermissionString);
			printPermissionString(file_header.File_mode);
			printf("%ld/",octalToDecimal(file_header.Owner_s_numeric_user_ID));
			printf("%ld ",octalToDecimal(file_header.Group_s_numeric_user_ID));
			printf("%ld ", jump_size);
			printModifTime(file_header.File_name,octalToDecimal(file_header.Last_modification_time_in_numeric_Unix_time_format_OctalB));

			//printf("%s ",file_header.Last_modification_time_in_numeric_Unix_time_format_OctalB);
			printf("%s",file_header.File_name);
			if (file_header.Type_flag[0]=='2') //printing 'l' if symlink, else '-'
							printf(" -> %s",file_header.Name_of_linked_file);
			printf("\n");
		}

		if(jump_size!=0){

				 if (jump_size%512){
					 jump_size += 512 - (jump_size % 512);
				 }




					lseek(fd,jump_size,SEEK_CUR);
				}
		//and read it
		lecture = read(fd,&file_header,512);

	}
}



