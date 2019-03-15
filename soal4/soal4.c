#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <sys/sysmacros.h>

int main() {
  pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

if ((chdir("/home/bima")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  int counter=1;

  while(1)
  {
   char dirpath[100]= "/home/bima/Documents/makanan/makan_enak.txt";
   struct stat makanEnak;
   stat(dirpath, &makanEnak);

   time_t now;
   time(&now);
   time_t lastAccess=makanEnak.st_atime;

   	if(difftime(now, lastAccess) <= 30)
   	{
       		char newFile[200];
		char addCounter[50];
       		strcpy(newFile, "/home/bima/Documents/makanan/makan_sehat");

       		sprintf(addCounter, "%d", counter);
       		strcat(newFile, addCounter);
       		strcat(newFile, ".txt");

		FILE *no4File;
       		no4File = fopen(newFile, "w");
       		fclose(no4File);
   	    	counter++;
		//sleep(5);
   	}
    sleep(5);
    }
    exit(EXIT_SUCCESS);
}
