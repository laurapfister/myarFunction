/*
  Name: Laura Pfister
  Email: pfisterl@onid.oregonstate.edu
  Class: CS344-400
  Assignment: Homework 3

*/


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>
#include <getopt.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ar.h>
#include <dirent.h>


void t_table(char *archive);
/*Print out the file name of each file stored in archive
  Requires: char *archive representing a file name.
  file must be an ar file.
  Returns: Nothing
*/

void v_table(char *archive);
/*Print out the file name, time file was added to the
  archive, user id, group id, permissions and file size.
  Requires: char *archive representing a file name.
  file must be an ar file.
  Returns: Nothing
*/


void q_append(char *archive, int argc, char** argv, int optind);
/*
  Appends files specified in argv to the file name specified in
  archive.
  Requires: char *archive representing a file name.
  file must be an ar file. Creates the file if it
  doesn't exist.
  int argc: # of arguemnts in argv
  char **argv: a array of char* of file names to add
  to the archive. Files must be "regular"
  Regular = a file whose mode, obtained via a call
  to stat, returns true on st_mode & I_FREG.
  int optind: integer respresenting place where file
  name arguments start in argv.
  Returns: Nothing
*/
void x_extract(char *archive, char** argv, int argc);
/*
  Extracts files specifed in argv into the current directory.
  If no files are specified, extracts all files in archive.
  Archive remains unaltered.
  Requires: char *archive: representing a file name. File must
  be an ar file. 
  char **argv: representing files to be extracted.
  int argc: #of arguments in argv
  Returns: Nothing
*/

void d_delete(char *archive, char** argv, int argc);
/* 
   Deletes files specified in argv from archive.
   If no files specified archive remains unaltered.
   Requires: char *archive: representing a file name. File must
   be an ar file. 
   char **argv: representing files to be deleted.
   int argc: #of arguments in argv
   Returns: Nothing
*/
   
void A_fullAppend(char *archive);
/* 
   Appends all regular files to archive, except archive.
   If archive does not exist, it is created.
   Regular = a file whose mode, obtained via a call
   to stat, returns true on st_mode & I_FREG.
   Requires: char *archive: representing a file name. If archive
   already exists, but be an ar file.
   Returns: Nothing
*/
   
void print_name(char *file_name);
/* 
   Prints file_name to stdout.
   Requires: char *file_name. file_name must terminate with /.
   Returns: nothing
*/

void print_mode(mode_t mode);
/*
  Prints permission in rwxrwxrwx form.
  Requires: mode_t mode: an 8 digit octal number.
  Returns: nothing
*/

void print_date(time_t l_time);
/*
  Prints date and time in local time.
  Example format: Oct 23 16:21 2014
  Requires: a 12 digit long representing seconds from epoch.
  Returns: nothing
*/

int create_file(struct ar_hdr file_head);
/*
  Creates a file in the current directory using information from
  struct ar_hdr file_head.
  Requires: struct ar_hdr file_head: A filled ar_hdr struct.
  Returns: int representing file descriptor of newly created file.
*/

void xtract_helper(struct ar_hdr archive_head, int fd);
/*
  Extracts a single file from fd to the current directory.
  Requires: struct ar_hdr archive_head: ar_name MUST be filled.
  int fd: file descriptor, representing archive.
  file must be of type ar.
  Returns: nothing
*/
int check_member(char* file_name, char** memberOpts, int optNum);
/*
  Checks if file_name is in char** memberOpts. If file is found
  removes it from memberOpts.
  Requires: char* file_name: the name of a file.
  char** memberOpts: an array of file names.
  optNum: number of entries in memberOpts.
  Returns: 1 if file_name is found, 0 if it is not found.
*/

void file_skip(struct ar_hdr archive_head, int fd);
/*
  Skips over archive_head entry in fd, so that offset is
  pointing to the first character after the entry specified in
  archive_head.
  Requires: struct ar_hdr archive_head: information contained
  in archive header for entry we want to skip.
  ar_size must be present.
  int fd: file descriptor of the file we want to 
  skip through.
  Returns: Nothing
*/

void fill_archive(struct ar_hdr* archive_head, int fd);
/*
  Fills archive_head with information from file descriptor fd.
  Requires: struct ar_hdr: passed by reference.
  int fd: File descriptor refering to open file archive.
  offset must be pointed to start of an archive entry.
  Returns: nothing.
*/

void content_copy(int from, int toFile, long long file_size);
/*
  Copies content of size file_size from "from" to "toFile".
  Requires: int from: a file descriptor of a non empty archive file.
  int toFile: a file descriptor of an archive file.
  long long file_size: Number of bytes to read from "from" to
  "toFile".
  Returns: nothing.
*/

void copy_arEntry(struct ar_hdr archive_head, int fd, int fd2);
/*
  Copies the archive header from archive_head into the file
  represented by file descriptor fd2. Copies contents of archive
  entry from fd to fd2.
  Requires: struct ar_hdr archive_head.
  int fd: a none empty archive file.
  int fd2: an archive file.
  Returns: Nothing
*/
void append_helper(int fd, char * file_name, struct stat sb);
/*
  Appends a single file, file_name to the archive represented by \
  file descriptor fd, using file information from sb.
  Requires: int fd: file descripto or archive file.
  char * file_name: a regular file. 
  Regular = a file whose mode, obtained via a call
  to stat, returns true on st_mode & I_FREG.
  struct stat sb: a stat struct with information about
  file_name.
  Returns: nothing
*/
  
int check_format(int fd, int use_pread);
/*
  Checks if file specified by file descriptor fd is of ar format.
  Requires: int fd: file descriptor.
  int use_pread: if 0, start reading at SEEK_CUR, file offset
  is changed. if 1, start reading at SEEK_SET, file offset is
  not changed.
  Return: 1 if file is ar format, 0 if file is not correct format.
*/

int main(int argc, char **argv){
  
	int c;
	char * archive;

	c = getopt(argc, argv, "q:x:t:v:d:A:");
	archive = optarg;
  
	if(c == 'q'){
		q_append(archive, argc, argv, optind);
	}
	else if(c == 'x'){
		x_extract(archive, argv, argc);
	}
	else if(c == 't' && argc == 3){
		t_table(archive);
	}
	else if(c == 'v' && argc == 3){
		v_table(archive);
	}
	else if(c == 'd'){
		d_delete(archive, argv, argc);
	}
	else if(c == 'A'){
		if(argc != 3)
			printf("Warning: additional arguments ignored.\n");
		A_fullAppend(archive);
	}
	else{
		printf("myar: no operation specified\n");
	}

	return 0;
}

void t_table(char *archive){

	int fd;
	int numread;
	struct ar_hdr archive_head;
  

	fd = open(archive, O_RDONLY, 666);
	if(fd == -1){
		exit(EXIT_FAILURE);
	}
  
	/*Check file format*/
	if(check_format(fd, 0) == 0){
		printf("myar: %s: File format not recognized\n", archive);
		exit(EXIT_FAILURE);
	}
  
	while((numread = read(fd, archive_head.ar_name, 16)) != 0){
    
		if(numread == -1){
			exit(EXIT_FAILURE);
		}

		print_name(archive_head.ar_name);
		printf("\n");
  
		file_skip(archive_head, fd);
	}
  
	if(close(fd) == -1)
		exit(EXIT_FAILURE);

}

void v_table(char *archive){

	int fd;
	int numread;
	struct ar_hdr archive_head;
	long l_uid, l_gid;
	mode_t l_mode;
	time_t l_time;
	long long file_size_num;

	fd = open(archive, O_RDONLY, 666);
	if(fd == -1){
		exit(EXIT_FAILURE);
	}

	/*Check file format*/
	if(check_format(fd, 0) == 0){
		printf("myar: %s: File format not recognized\n", archive);
		exit(EXIT_FAILURE);
	}

	while((numread = read(fd, archive_head.ar_name, 16)) != 0){
    
		if(numread == -1){
			exit(EXIT_FAILURE);
		}
		fill_archive(&archive_head, fd);
  
		file_size_num = strtoll(archive_head.ar_size, NULL, 10);
		l_uid = strtol(archive_head.ar_uid, NULL, 10);
		l_gid = strtol(archive_head.ar_gid, NULL, 10);
		l_mode = strtol(archive_head.ar_mode, NULL, 8);
		l_time = strtoll(archive_head.ar_date, NULL, 10);

		if((lseek(fd, 2, SEEK_CUR)) == -1){
			exit(EXIT_FAILURE);
		}
    
    
		if(lseek(fd, file_size_num, SEEK_CUR) == -1){
			exit(EXIT_FAILURE);
		}
    
		if(((int)file_size_num % 2) != 0){
			if(lseek(fd, 1, SEEK_CUR) == -1){
				exit(EXIT_FAILURE);
			}
		}
   
		print_mode(l_mode);
		printf("%ld/%ld %6lld ", l_uid, l_gid, file_size_num);
		print_date(l_time);
		printf(" ");
		print_name(archive_head.ar_name);
		printf("\n");
    
	}
  
	if(close(fd) == -1)
		exit(EXIT_FAILURE);


}
void print_name(char *file_name){

	char c;
	int place = 0;
	c = file_name[place];

	while(c != '/'){
		printf("%c", c);
		place++;
		c = file_name[place];
	}

}

void print_date(time_t l_time){
  
	struct tm *time_epoch;
	char date_string[18];

	time_epoch = localtime(&l_time);
  
	strftime(date_string, 18, "%b %d %H:%M %Y", time_epoch);

	printf("%s", date_string);

}

void print_mode(mode_t mode){

	printf("%c", ((mode & S_IRUSR) ? 'r' : '-'));
	printf("%c", ((mode & S_IWUSR) ? 'w' : '-'));
	printf("%c", ((mode & S_IXUSR) ? 'x' : '-'));
	printf("%c", ((mode & S_IRGRP) ? 'r' : '-'));
	printf("%c", ((mode & S_IWGRP) ? 'w' : '-'));
	printf("%c", ((mode & S_IXGRP) ? 'x' : '-'));
	printf("%c", ((mode & S_IROTH) ? 'r' : '-'));
	printf("%c", ((mode & S_IWOTH) ? 'w' : '-'));
	printf("%c ", ((mode & S_IXOTH) ? 'x' : '-'));
}

void q_append(char *archive, int argc, char** argv, int optind){

	int fd;
	struct stat sb;

	if((fd = open(archive, O_RDWR | O_CREAT | O_APPEND, 00666)) == -1){
		exit(EXIT_FAILURE);
	}

	stat(archive, &sb);
	if(sb.st_size == 0){
		if((write(fd, ARMAG, SARMAG)) == -1){
			exit(EXIT_FAILURE);
		}
	}
  
	while(optind < argc){
		char * file_name = argv[optind];
    
		if((stat(file_name, &sb)) == -1){
			exit(EXIT_FAILURE);
		}
		if(!(sb.st_mode & S_IFREG)){
			printf("myar: %s: File format not recognized\n", file_name);
			exit(EXIT_FAILURE);
		}
		append_helper(fd, file_name, sb);
		optind++;
	}
	if(close(fd) == -1)
		exit(EXIT_FAILURE);
}

void append_helper(int fd, char * file_name, struct stat sb){
	struct ar_hdr archive_head;
	int fd2;
	int written;

	if((written = write(fd, file_name, strlen(file_name))) == -1){
		exit(EXIT_FAILURE);
	}
	if((write(fd, "/", 1)) == -1){
		exit(EXIT_FAILURE);
	}
	written++;
	while(written < 16){
		if((write(fd, " ", 1)) == -1){
			exit(EXIT_FAILURE);
		}
		written++;
	}
    
	sprintf(archive_head.ar_date, "%-12ld", sb.st_mtime);
    

	if((write(fd, archive_head.ar_date, sizeof(archive_head.ar_date))) == -1){
		exit(EXIT_FAILURE);
	}
    
	sprintf(archive_head.ar_uid, "%-6d", sb.st_uid);
	if((write(fd, archive_head.ar_uid, sizeof(archive_head.ar_uid))) == -1){
		exit(EXIT_FAILURE);
	}
    
	sprintf(archive_head.ar_gid, "%-6d", sb.st_gid);
	if((write(fd, archive_head.ar_gid, sizeof(archive_head.ar_gid))) == -1){
		exit(EXIT_FAILURE);
	}
    
	sprintf(archive_head.ar_mode, "%-8o", sb.st_mode);
	if((write(fd, archive_head.ar_mode, sizeof(archive_head.ar_mode))) == -1){
		exit(EXIT_FAILURE);
	}
    
	sprintf(archive_head.ar_size, "%-10lld", (long long) sb.st_size);
	if((write(fd, archive_head.ar_size, sizeof(archive_head.ar_size))) == -1){
		exit(EXIT_FAILURE);
	}
    
	sprintf(archive_head.ar_fmag, "%2s", ARFMAG);
	if((write(fd, archive_head.ar_fmag, sizeof(archive_head.ar_fmag))) == -1){
		exit(EXIT_FAILURE);
	}
	if((fd2 = open(file_name, O_RDONLY)) == -1){
		exit(EXIT_FAILURE);
	}
    
	content_copy(fd2, fd, sb.st_size);
 
	if((sb.st_size % 2) != 0){
		if((write(fd, "\n", 1)) == -1){
			exit(EXIT_FAILURE);
		}
	}
  
	if(close(fd2) == -1)
		exit(EXIT_FAILURE);
}

void A_fullAppend(char *archive){
  
	struct dirent *dp;
	DIR *dirp;
	struct stat sb;
	int fd;

	if((fd = open(archive, O_RDWR | O_CREAT | O_APPEND, 00666)) == -1){
		exit(EXIT_FAILURE);
	}
	stat(archive, &sb);
	if(sb.st_size == 0){
		if((write(fd, ARMAG, SARMAG)) == -1){
			exit(EXIT_FAILURE);
		}
	}
	else{
		if(check_format(fd, 1) == 0){
			printf("myar: %s: File format not recognized\n", archive);
			exit(EXIT_FAILURE);
		}
	}

	/*Open current directory*/
	if((dirp = opendir(".")) == NULL){
		exit(EXIT_FAILURE);
	}
  
	/*Read contents of current directory*/
	while((dp = readdir(dirp)) != NULL){
		char * file_name = dp->d_name;
		if(strcmp(file_name, archive) == 0)
			continue;
		stat(file_name, &sb);
		if(sb.st_mode & S_IFREG){
			printf("%s\n", dp->d_name);
			print_mode(sb.st_mode);
			append_helper(fd, file_name, sb);
		}
	}
}


void x_extract(char *archive, char** argv, int argc){
  
	int fd;
	struct ar_hdr archive_head;
	int numread;
	int i;
  
	int optNum = argc - 3;
	char* options[optNum];
  
	for(i = 0; i < optNum; i++){
		options[i] = argv[i+3];
	}
  

	fd = open(archive, O_RDONLY, 00666);
	if(fd == -1){
		exit(EXIT_FAILURE);
	}

	if(check_format(fd, 0) == 0){
		printf("myar: %s: File format not recognized\n", archive);
		exit(EXIT_FAILURE);
	}
  
	while((numread = read(fd, archive_head.ar_name, 16)) != 0){
    
		char *file_name = strtok(archive_head.ar_name, "/");
		if(argc == 3 || check_member(file_name, options, optNum)){
			if(numread == -1){
				exit(EXIT_FAILURE);
			}
			xtract_helper(archive_head,  fd);
		}
		else{
			file_skip(archive_head, fd);
		}
	}
	if(close(fd) == -1)
		exit(EXIT_FAILURE);
  
}

void content_copy(int from, int toFile, long long file_size){
	int i;
	char buffer[1];

	for(i = 0; i < file_size; i++){
		if((read(from, buffer, 1)) == -1){
			exit(EXIT_FAILURE);
		}
		if((write(toFile, buffer, 1)) == -1){
			exit(EXIT_FAILURE);
		}
	}
}


void d_delete(char *archive, char** argv, int argc){
  
	int fd, fd2;
	struct ar_hdr archive_head;
	int numread;
	int i;
  
	/*If no arguments specified return*/
	if(argc == 3){
		return;
	}

	int optNum = argc - 3;
	char* options[optNum];
  
	for(i = 0; i < optNum; i++){
		options[i] = argv[i+3];
	}

	fd = open(archive, O_RDONLY, 00666);
 
	if(fd == -1){
		exit(EXIT_FAILURE);
	}
   
	/*Check file format*/
	if(check_format(fd, 0) == 0){
		printf("myar: %s: File format not recognized\n", archive);
		exit(EXIT_FAILURE);
	}
	unlink(archive);

	fd2 = open(archive, O_WRONLY | O_CREAT, 00666);
  
	if(fd2 == -1){
		exit(EXIT_FAILURE);
	}
	if((write(fd2, ARMAG, SARMAG)) == -1){
		exit(EXIT_FAILURE);
	}
  
	while((numread = read(fd, archive_head.ar_name, 16)) != 0){
    
		char *file_name = strtok(archive_head.ar_name, "/");
    
		if(check_member(file_name, options, optNum) == 1){
			/*Delete archive entry*/
			file_skip(archive_head, fd);
		}
		else{
			/*Copy archive entry to new file*/
			fill_archive(&archive_head, fd);
			sprintf(archive_head.ar_fmag, "%2s", ARFMAG);
			if((lseek(fd, 2, SEEK_CUR) == -1)){
				exit(EXIT_FAILURE);
			}
			copy_arEntry(archive_head, fd, fd2);
		}
	}
	if(close(fd) == -1)
		exit(EXIT_FAILURE);
	if(close(fd2) == -1)
		exit(EXIT_FAILURE);
}
 
void copy_arEntry(struct ar_hdr archive_head, int fd, int fd2){
  
	int written;
	char * file_name = strtok(archive_head.ar_name, "/");
  
	if((written = write(fd2, file_name, strlen(file_name))) == -1){
		exit(EXIT_FAILURE);
	}
	if((write(fd2, "/", 1)) == -1){
		exit(EXIT_FAILURE);
	}
	written++;
	while(written < 16){
		if((write(fd2, " ", 1)) == -1){
			exit(EXIT_FAILURE);
		}
		written++;
	}
	if((write(fd2, archive_head.ar_date, 12)) == -1){
		exit(EXIT_FAILURE);
	}
	if((write(fd2, archive_head.ar_uid, 6)) == -1){
		exit(EXIT_FAILURE);
	}
	if((write(fd2, archive_head.ar_gid, 6)) == -1){
		exit(EXIT_FAILURE);
	}
	if((write(fd2, archive_head.ar_mode, 8)) == -1){
		exit(EXIT_FAILURE);
	}
	if((write(fd2, archive_head.ar_size, 10)) == -1){
		exit(EXIT_FAILURE);
	}
	if((write(fd2, archive_head.ar_fmag, 2)) == -1){
		exit(EXIT_FAILURE);
	}
	long long file_size = strtoll(archive_head.ar_size, NULL, 10);
	content_copy(fd, fd2, file_size);
	if((file_size % 2) == 1){
		if((write(fd2, "\n", 1)) == -1){
			exit(EXIT_FAILURE);
		}
		if((lseek(fd, 1, SEEK_CUR)) == 1){
			exit(EXIT_FAILURE);
		}
	}
}  

int check_member(char* file_name, char** memberOpts, int optNum){
  
	int i;
	for(i = 0; i < optNum; i++){
		if(strcmp(file_name, memberOpts[i]) == 0){
			memberOpts[i] = "";
			return 1;
		}
	}
	return 0;
}

void file_skip(struct ar_hdr archive_head, int fd){
  
	/*skip past date, uid, gid, mode*/
	if((lseek(fd, 32, SEEK_CUR)) == -1){
		exit(EXIT_FAILURE);
	}
	if((read(fd, archive_head.ar_size, 10)) == -1){
		exit(EXIT_FAILURE);
	}
	long file_size_num = strtoll(archive_head.ar_size, NULL, 10);
  
	if((lseek(fd, 2, SEEK_CUR)) == -1){
		exit(EXIT_FAILURE);
	}
  
	/*skip pastfile contents*/
	if((lseek(fd, file_size_num, SEEK_CUR)) == -1){
		exit(EXIT_FAILURE);
	}
	if((file_size_num % 2) == 1){
		if(lseek(fd, 1, SEEK_CUR) == -1){
			exit(EXIT_FAILURE);
		}
	}
}

void xtract_helper(struct ar_hdr archive_head, int fd){

	fill_archive(&archive_head, fd);
  
	int fd2 = create_file(archive_head);
  
	long file_size_num = strtoll(archive_head.ar_size, NULL, 10);
  
	if((lseek(fd, 2, SEEK_CUR)) == -1){
		exit(EXIT_FAILURE);
	}

	/*Write file contents*/
	content_copy(fd, fd2, file_size_num);
  
	if(close(fd2) == -1)
		exit(EXIT_FAILURE);

	if(((int)file_size_num % 2) != 0){
		if(lseek(fd, 1, SEEK_CUR) == -1){
			exit(EXIT_FAILURE);
		}
	}
}

int create_file(struct ar_hdr file_head){
  
	int fd;
	struct utimbuf file_time;

	char *file_name = strtok(file_head.ar_name, "/");
	mode_t l_mode = strtol(file_head.ar_mode, NULL, 8);
	time_t modtime = strtoll(file_head.ar_date, NULL, 10);
	uid_t owner = strtol(file_head.ar_uid, NULL, 10);
	gid_t group = strtol(file_head.ar_gid, NULL, 10);
  
	/*CREATE FILE*/
	if((fd = open(file_name,  O_WRONLY | O_CREAT | O_TRUNC, l_mode)) == -1){
		exit(EXIT_FAILURE);
	}
  
	file_time.actime = modtime;
	file_time.modtime = modtime;

	if((utime(file_name, &file_time)) == -1){
		exit(EXIT_FAILURE);
	}
	chown(file_name, owner, group);
  
	return fd;
}

void fill_archive(struct ar_hdr* archive_head, int fd){
	if((read(fd, archive_head->ar_date, 12)) == -1){
		exit(EXIT_FAILURE);
	}  
  
	if((read(fd, archive_head->ar_uid, 6)) == -1){
		exit(EXIT_FAILURE);
	}
  
  
	if((read(fd, archive_head->ar_gid, 6)) == -1){
		exit(EXIT_FAILURE);
	}
  
  
	if((read(fd, archive_head->ar_mode, 8)) == -1){
		exit(EXIT_FAILURE);
	}
  
	/*Read file size*/
	if((read(fd, archive_head->ar_size, 10)) == -1){
		exit(EXIT_FAILURE);
	}
}

int check_format(int fd, int use_pread){
  
	char match[SARMAG];
  
	if(!use_pread){
		if((read(fd, match, SARMAG)) == -1){
			exit(EXIT_FAILURE);
		}
	}
	else{
		if((pread(fd, match, SARMAG, SEEK_SET)) == -1){
			exit(EXIT_FAILURE);
		}
	}  
	if(strcmp(match, ARMAG) == 0){
		return 1;
	}
	return 0;
}
