#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

int main(){
	pid_t child_id, child2_id, child3_id;
	int status;
        int p[4];
	char content[99999];

    	child_id = fork();

    	if(child_id == 0){
		char *argv[3] = {"unzip", "campur2.zip", NULL};
      		execv("/usr/bin/unzip", argv);
    	}else{
		while(wait(&status)>0);
        	pipe(p);
       	 	pipe(p+2);

        	child2_id = fork();

        	if(child2_id == 0){
          		close(p[0]);
          		close(p[2]);
          		close(p[3]);
          		dup2(p[1], STDOUT_FILENO);
          		close(p[1]);

          		char *argv[3] = {"ls", "campur2", NULL};
	  		execv("/bin/ls", argv);
        	}else{

            		child3_id = fork();

            		if(child3_id == 0){
              			close(p[1]);
              			dup2(p[0],STDIN_FILENO);
              			close(p[0]);
              			close(p[2]);
              			dup2(p[3],STDOUT_FILENO);
              			close(p[3]);
	      			char *argv[3] = {"grep", ".txt$", NULL};
              			execv("/bin/grep", argv);
            		}else{
              			close(p[1]);
              			close(p[0]);
              			close(p[3]);
              			int x = read(p[2],content,sizeof(content));
              			close(p[2]);

				FILE* list;
              			list = fopen("daftar.txt","w+");
              			fputs(content,list);
              			fclose(list);
            		}
        	}
    	}
}
