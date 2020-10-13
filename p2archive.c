#include<stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<string.h>
#include<errno.h>
#include<dirent.h>
#include<time.h>

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
	
	struct stat buff;
	int fd, number, bytes, length, i=0;
	char full_name[254], periexomeno[512], *check, *name;
	
	while(1){
		check=fgets(full_name, 253, stdin);
		if(check==NULL){
			break;
		}
		full_name[(strlen(full_name))-1]='\0';
	
		fd=my_open(full_name,O_RDWR, S_IRWXU);
		if(fd==-1){
			printf("%s\n", strerror(errno));
			return 0;
		}
		else{
			if(fstat(fd, &buff)==-1){
				printf("%s\n", strerror(errno));
				return 0;
			}
			else{
				name=strchr(full_name,'/');
				length=strlen(name+1);//mikos onomatos
				if(name!=NULL){
					write(1, &length, sizeof(int));
					if(number==-1){
						printf("%s\n",strerror(errno));
					return 0;
					}
				
					my_write(1, name+1, length);
				}
				
				
				number=write(1, &buff.st_atime, sizeof(time_t));//teleutaia prosvash
				if(number==-1){
					printf("%s\n",strerror(errno));
					return 0;
				}
				number=write(1, &buff.st_mtime, sizeof(time_t));//teleutaia allagh
				if(number==-1){
					printf("%s\n",strerror(errno));
					return 0;
				}
				number=write(1, &buff.st_mode, sizeof(mode_t)); //protection
				if(number==-1){
					printf("%s\n",strerror(errno));
					return 0;
				}
				bytes=buff.st_size;//bytes periexomenou
				number=write(1, &bytes, sizeof(int));
				if(number==-1){
					printf("%s\n",strerror(errno));
					return 0;
				}
				while(i<bytes/512){
					my_read(fd, periexomeno, 512);
					my_write(1, periexomeno, 512);
					i++;
				}
				if(bytes%512!=0){
					my_read(fd, periexomeno, bytes%512);
					my_write(1, periexomeno, bytes%512);
				}
			}
		}
	}
	
	return 0;
	
}