hw2: dirlist p2archive p2unarchive p2crypt 
	gcc -Wall -g hw2.c -o hw2

dirlist: dirlist.c
	gcc -Wall -g dirlist.c -o dirlist

p2archive: p2archive.c
	gcc -Wall -g p2archive.c -o p2archive

p2unarchive: p2unarchive.c
	gcc -Wall -g p2unarchive.c -o p2unarchive

p2crypt: p2crypt.c
	gcc -Wall -g p2crypt.c -o p2crypt
