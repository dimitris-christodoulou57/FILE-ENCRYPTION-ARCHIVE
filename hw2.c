//spyridoni eirini 2182 christodoulou dimitris 2113

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	
	char directory_name[255], p2cryptar[255], file_name[255], elexos[9];
	int fd, check, pipe1[2], pipe2[2];
	pid_t pid1, pid2, pid3, w;
	if(argc!=5){
		return 0;
	}
	
	strcpy(directory_name,argv[2]); //onoma fakelou pou periexei ta arxeia
	strcpy(p2cryptar,argv[3]); //kleidi gia apokriptografhsh
	strcpy(file_name,argv[4]);// onoma arxeiou
	


	if(strcmp(argv[1],"-E")==0){
		fd=open(file_name, O_RDWR | O_CREAT | O_EXCL, S_IRWXU);
		if(fd==-1) {
			fprintf(stderr, "error in open %d\n", __LINE__);
			return (-1);
		}
		else{
			check=write(fd, "P2CRYPTAR", 9);
			if(check==-1){
				fprintf(stderr, "error in write %d\n", __LINE__);
				return (-1);
			}
		}
		check=pipe(pipe1);
		if(check==-1){
			fprintf(stderr, "error in pipe %d\n", __LINE__);
			return -1;
		}
		
		pid1=fork(); 
		if(pid1<0){
			fprintf(stderr, "error in pid %d\n", __LINE__);
			return -1;
		}
		
		if( pid1==0) {
		
			
			check=dup2(pipe1[1],STDOUT_FILENO);
			if(check==-1){
				fprintf(stderr, "error in dup2 %d\n", __LINE__);
				return -1;
			}
			close(pipe1[1]);
			close(pipe1[0]);
			
			check=execlp("./dirlist","dirlist", directory_name,NULL);
			if(check==-1){
				fprintf(stderr, "error in execlp %d\n", __LINE__);
				return -1;
			}
			perror("1 execlp");
			return (1);
		}
		close(pipe1[1]);
		
		check=pipe(pipe2);
		if(check==-1){
			fprintf(stderr, "error in pipe %d\n", __LINE__);
			return -1;
		}
		pid2=fork();
		if(pid2<0){
			fprintf(stderr, "error in pid %d\n", __LINE__);
			return -1;
		}
		
		
		if(pid2==0){
	
			check=dup2(pipe1[0], STDIN_FILENO);
			if(check==-1){
				fprintf(stderr, "error in dup2 %d\n", __LINE__);
				return -1;
			}
			check=dup2(pipe2[1],STDOUT_FILENO);
			if(check==-1){
				fprintf(stderr, "error in dup2 %d\n", __LINE__);
				return -1;
			}
			check=execlp("./p2archive","p2archive", NULL);
			if(check==-1){
				fprintf(stderr, "error in execlp %d\n", __LINE__);
				return -1;
			}
			perror("2 execlp");
			
			return (2);
		}
		close(pipe1[0]);
		close(pipe2[1]);
		
		pid3=fork();
		if(pid3<0){
			fprintf(stderr, "error in pid %d\n", __LINE__);
			return -1;
		}
		if(pid3==0){
			check=dup2(pipe2[0],STDIN_FILENO);
			if(check==-1){
				fprintf(stderr, "error in dup2 %d\n", __LINE__);
				return -1;
				
			}
			check=dup2(fd, STDOUT_FILENO);
			if(check==-1){
				fprintf(stderr, "error in dup2 %d\n", __LINE__);
				return -1;
			}
			close(pipe2[0]);
			close(pipe2[1]);
			check=execlp("./p2crypt","p2crypt", p2cryptar , NULL);
			if(check==-1){
				fprintf(stderr, "error %s in execlp %d\n", strerror(errno), __LINE__);
				return -1;
			}
			perror("3 execlp");

			return (3);
		}
		
		
		w=waitpid(pid1, NULL, 0);
		if(w==-1){
			fprintf(stderr, "error in waitpid %d\n", __LINE__);
			return -1;
		}
		w=waitpid(pid2, NULL, 0);
		if(w==-1){
			fprintf(stderr, "error in waitpid %d\n", __LINE__);
			return -1;
		}
		w=waitpid(pid3, NULL, 0);
		if(w==-1){
			
			fprintf(stderr, "error in waitpid %d\n", __LINE__);
			return -1;
		}
	}
	else if (strcmp(argv[1],"-D")==0){
		
		fd=open(file_name, O_RDWR, S_IRWXU);
		if(fd==-1){
			printf("%s\n", strerror(errno));
			return -1;
		}
		check=lseek(fd, 0, SEEK_SET);
		if(check==-1){
			return -1;
		}
		check=read(fd, elexos, 9);
		if(check==-1){
			printf("%s\n", strerror(errno));
			return -1;
		}
		elexos[9]='\0';
		if(strcmp(elexos,"P2CRYPTAR")!=0){
			printf("WRONG SECRET KEY WORD\n");
			return -1;
		}
		check=pipe(pipe1);
		if(check==-1){
			fprintf(stderr, "error in write %d\n", __LINE__);
			return (-1);
		}
		pid1=fork(); 
		if(pid1<0){
			fprintf(stderr, "error in pid %d\n", __LINE__);
			return -1;
		}
		if(pid1==0){
			check=dup2(fd, STDIN_FILENO);
			if(check==-1){
				fprintf(stderr, "error in dup2 in line %d\n", __LINE__);
			}
			check=dup2(pipe1[1], STDOUT_FILENO);
			if(check==-1){
				fprintf(stderr, "error in dup2 in line %d\n", __LINE__);
			}
			close(pipe1[1]);
			close(pipe1[0]);
			
			check=execlp("./p2crypt","p2crypt", p2cryptar, NULL);
			if(check==-1){
				fprintf(stderr, "error in execlp %d\n", __LINE__);
				return -1;
			}
			perror("1 execlp");
			
			return 1;
		}
		close(pipe1[1]);
		
		pid2=fork();
		if(pid2<0){
			fprintf(stderr, "error in pid %d\n", __LINE__);
			return -1;
		}
		if(pid2==0){
			check=dup2(pipe1[0], STDIN_FILENO);
			if(check==-1){
				fprintf(stderr, "error in dup2 in line %d\n", __LINE__);
			}
			check=execlp("./p2unarchive","p2unarchive", directory_name, NULL);
			if(check==-1){
				fprintf(stderr, "error in execlp %d\n", __LINE__);
				return -1;
			}
			perror("execlp\n");
			return 2;
		}
		w=waitpid(pid1, NULL, 0);
		if(w==-1){
			fprintf(stderr, "error in waitpid %d\n", __LINE__);
			return -1;
		}
		w=waitpid(pid2, NULL, 0);
		if(w==-1){
			fprintf(stderr, "error in waitpid %d\n", __LINE__);
			return -1;
		}
			
			
		
		
		

	}
	
	return 0;
}