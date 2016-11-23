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




/****** Functions in ptar.c ******/

/* octalString: string that represents file size in octal base
 *
 * Converts a number (described by a String) from octal to decimal base
 */
long octalToDecimal(char* octalString);


/*source_fd: file descriptor of a tar archive.
 * header h: header structure to read into from tar archive
 *
 * This function is called by main() when -x option is used
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
 *
 */
void extract(int source_fd, struct header h,long fileSize);


/****** Functions in display.c ******/

void displayFileNames(int fd,struct header file_header);

void displayDetailedListing(int fd,struct header file_header);

#endif /* PTAR_H_ */
