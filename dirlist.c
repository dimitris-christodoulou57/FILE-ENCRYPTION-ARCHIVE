#include<stdio.h>
#include<dirent.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<errno.h>


int main(int argc, char *argv[]){
	
	DIR *directory;
	struct dirent *buff;
	char *path, *fullpath;
	int lenght, lenght1;
	
	if(argc!=1){
		path=argv[1];
		lenght=strlen(path);
		directory=opendir(path);
		if(directory!=NULL){
			buff=readdir(directory);
			while(buff !=NULL){
				if(buff->d_type==DT_REG){
					lenght1=strlen(buff->d_name);
					fullpath=strcat(path,"/");
					fullpath=strcat(fullpath, buff->d_name);
					write(1, fullpath, lenght+1+lenght1);
					write(1, "\n", 1);
					fullpath[lenght]='\0';
				}
				buff=readdir(directory);
			}
		}
		else{
			fprintf(stderr, "error in opendir at line %d\n", __LINE__);
			return 0;
		}
	}
	else{
		printf("there is no directory name given\n");
	}
	return 0;
}