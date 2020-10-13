#include<stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<string.h>
#include<errno.h>

int main (int argc, char *argv[]){
	
	int size, i=0 ,j=0 ,n;
	char file[512], crypt_file[512];
	
	if(argc!=2){
		fprintf(stderr, "not give key fot crypt %d\n", __LINE__);
		return -1;
	}
	size=strlen(argv[1]);
	while(1) {
		n = read(0, file, 512);
		if(n==-1){
			printf("%s\n", strerror(errno));
			return -1;
		}
		
		if (n==0){
			break;
		}
		
		for(i=0; i<n; i++,j++){
			crypt_file[i]=(file[i]^argv[1][j]);
			write(1, &crypt_file[i], sizeof(char));
			if(j==size){
				j=0;
			}
		}
	}
	return 0;
}