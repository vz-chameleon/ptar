/*
 * ptar.h
 *
 *  Created on: Oct 23, 2016
 *      Author: Valentina Zelaya et Timothy Garwood
 */

#ifndef PTAR_H_
#define PTAR_H_

/*Structure for tar header in ustar format*/
struct header_posix_ustar{
	char File_name[100];
	char File_mode[8];
	char Owner_s_numeric_user_ID[8];
	char Group_s_numeric_user_ID[8];
	char File_size_in_bytes_octalB[12];
	char Last_modification_time_in_numeric_Unix_time_format_OctalB[12];
	char Checksum_for_header_record[8];
	char Type_flag[1];
	char Name_of_linked_file[100];
	char UStar_indicator[6];
	char UStar_version[2];
	char Owner_user_name[32];
	char Owner_group_name[32];
	char Device_major_number[8];
	char Device_minor_number[8];
	char Filename_prefix[155];
	}ptar_header;







#endif /* PTAR_H_ */
