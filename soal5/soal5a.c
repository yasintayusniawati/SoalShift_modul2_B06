#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

int main() {
  pid_t pid, sid,child;
  int status;

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

  if ((chdir("/home/bima/log")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

/*  FILE *getPID;
  getPID = fopen("/home/bima/log/getPID.txt", "w+");
  fprintf(getPID, "%d", getpid());
  fclose(getPID); */

  while(1) {
	time_t t=time(NULL);
	struct tm tm = *localtime(&t);
	char judul[100];
	char data[100];
	char dirpath[100]= "/home/bima/log/";
	static int i = 0;

	if(i%1800==0){
		sprintf(judul,"%02d:%02d:%d-%02d:%02d",tm.tm_mday,tm.tm_mon + 1,tm.tm_year + 1900,tm.tm_hour, tm.tm_min);
	}

	i++;
	child = fork();
	if(child==0){
		char *argv[] = {"mkdir", judul, NULL};
		execv("/bin/mkdir", argv);
	}

	strcat(dirpath,judul);
	strcat(dirpath,"/");

	child = fork();
	if(child==0){
		sprintf(data,"log%02d.log",i);
		strcat(dirpath,data);
		char *argv[] = {"cp", "/var/log/syslog",dirpath, NULL};
		    execv("/bin/cp", argv);
	}

  sleep(60);

  }

  exit(EXIT_SUCCESS);
}
