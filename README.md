# rs2016-GarwoodZelaya
   Introduction to UNIX System Programming Project 
   
   * ptar: an efficient and parallel archive file extractor 
   2nd year project at TELECOM Nancy
   
ptar command can list and extract files and directories of a tar archive file.
Its default behaviour is to list the file names of the archive.
   
Use the following options accordingly with what you want to do with ptar:
   
  -  -h
   Get some guidance and quick help
   
  -  -l
  Display a detailed list of files containing access rights, file_size and last modification date
   
  -  -x
  Extract files from tar archive into current directory
   
  -  -p <number>
  Specify a number of threads with which you want to extract files from tar
   
 This program uses fsync() to write the extracted files on the disk, so they are durably extracted.
