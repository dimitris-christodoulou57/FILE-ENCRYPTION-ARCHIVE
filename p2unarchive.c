#include<stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<string.h>
#include<errno.h>
#include<dirent.h>
#include<time.h>
#include<utime.h>

int my_open(char *name, int flags, int perms) {
	
	int fd;
	
	fd=open(name, flags, perms);
	if(fd==-1){
		printf("%s\n", strerror(errno));
		return -1;
	}
	
	return (fd);	
}

int my_write(int fd, char *string, int size){
	
	int check;
	
	check=write(fd,string,size);
	if(check==-1){
		fprintf(stderr, "error in write at line %d, file_descriptor = %d\n", __LINE__, fd);
		return -1;
	}
	
	return (check);
}

int my_read(int fd, char *string, int size){
	
	int check;
	
	check=read(fd,string,size);
	if(check==-1){
		fprintf(stderr, "error in read at line %d, file_descriptor = %d\n", __LINE__, fd);
		return -1;
	}
	
	return (check);
	
}

int main(int argc, char *argv[]){
	
	int fd, check, length, file, bytes, i=0, protection;
	DIR *directory;
	char file_name[255], name_with_path[255], content[512];
	struct utimbuf times;
	struct stat buff;
	
	
	if(argc!=2){
		printf("NO DIRECTORY GIVEN\n");
		return 0;
	}
	fd=mkdir(argv[1], S_IRWXU);
	if(fd==-1){
		printf("%s\n", strerror(errno));
		return 0;
	}
	directory=opendir(argv[1]);
	if(directory==NULL){
		printf("%s\n", strerror(errno));
	}
	while (1){
	check=read(0, &length, sizeof(int));
	
	if(check==-1){
		printf("%s\n", strerror(errno));
		return -1;
	}
	if(check==0){
		break;
	}
	my_read(0, file_name, length);
	file_name[length]='\0';
	name_with_path[0]='\0';
	strcat(name_with_path, argv[1]);
	strcat(name_with_path,"/");
	strcat(name_with_path,file_name);
	
	file=my_open(name_with_path, O_CREAT | O_RDWR , S_IRWXU);

	check=read(0, &buff.st_atime, sizeof(time_t));
	if(check<=0){
		printf("%s\n", strerror(errno));
		return -1;
	}
	times.actime=buff.st_atime;

	check=read(0, &buff.st_mtime, sizeof(time_t));
	if(check<=0){
		printf("%s\n", strerror(errno));
		return -1;
	}
	

	times.modtime=buff.st_mtime;
	check=read(0, &protection, sizeof(mode_t));
	if(check<=0){
		printf("%s\n", strerror(errno));
		return -1;
	}
	check=chmod(name_with_path, protection);
	if(check==-1){
		printf("%s\n", strerror(errno));
		return -1;
	}
	check=read(0, &bytes, sizeof(int));
	if(check<=0){
		printf("%s\n", strerror(errno));
		return -1;
	}
	
	
	while(i<bytes/512){
		my_read(0, content, 512);
		my_write(file, content, 512);
		i++;
	}
	if(bytes%512!=0){
		my_read(0, content, bytes%512);
		my_write(file, content, bytes%512);
	}
	check=utime(name_with_path, &times);
	if(check==-1){
		printf("%s\n", strerror(errno));
		return -1;
	}
	}
	return 0;
}