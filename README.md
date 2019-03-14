# Soal Shift Modul2 B06

## Oleh Kelompok B06 :
1. Yasinta Yusniawati   05111740000054
2. Bima Satria Ramadhan 05111740000081

## Jawaban

## No 1
Karena tidak diperbolehkan menggunakan crontab kita berasumsi menggunakan daemon agar file yang berekstensi .png dalam suatu folder dipindah ke /home/user/modul2/gambar secara realtime.

### Program C
```
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/dir.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

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

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
  DIR *folder;
  struct dirent *isi;
  chdir("/home/yasinta/");
  folder=opendir("Pictures");
  while((isi=readdir(folder)) != NULL){

      if(strstr(isi->d_name,".png")!=NULL){
        
        int panjang_string = strlen(isi->d_name);
        int panjang;
        panjang = panjang_string - 4;

        char file_baru[2000], tambahan[15];
        char dari[1000];

        memset(file_baru, '\0', sizeof(file_baru));
        memset(tambahan, '\0', sizeof(file_baru));
        memset(dari, '\0', sizeof(file_baru));
        
        strcpy(dari, "/home/yasinta/Pictures/");
        strcat(dari, isi->d_name);
        strcpy(file_baru, "/home/yasinta/modul2/gambar/");
        strncat(file_baru, isi->d_name, panjang);
        strcpy(tambahan, "_grey.png");
        strcat(file_baru, tambahan);
        // printf("dari: %s\nke: %s\n",dari,file_baru);

      //wait, fork, exec
        pid_t child_id;
        int status;
        
        child_id = fork();

        if (child_id == 0) {
          char *argv[4] = {"mv", dari, file_baru, NULL};
          execv("/bin/mv", argv);
        }
        else{
          while ((wait(&status)) > 0);
        }
        
      }
  }
  closedir(folder);
    sleep(1);
  }
  
  exit(EXIT_SUCCESS);
}
```
#### Penjelasan
**Main Program**
+ `DIR *folder;` variable dngan nama folder dengan tipe data DIR
+ `struct dirent *isi;`adalah variable di dalam variable yang mempunyai mempunyai tipe data tertentu dalam hal ini tipe data yang digunakan adalah d_name dan d_type
+ `chdir("/home/yasinta/");`merubah direktori ke suatu path spesifik
+ `folder=opendir("Pictures");`variable untuk membuka folder Picture yaitu direktory asalah dari gambar yang akan di pindah
+ `while((isi=readdir(folder)) != NULL)' mengecek apakah dalam folter tersebut ada isinya atau tidak jika ada akan menjalankan perintah di bawahnya`
