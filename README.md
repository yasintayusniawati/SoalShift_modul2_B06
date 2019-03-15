# Soal Shift Modul2 B06

## Oleh Kelompok B06 :
1. Yasinta Yusniawati   05111740000054
2. Bima Satria Ramadhan 05111740000081

## Jawaban

## No 1
Karena tidak diperbolehkan menggunakan crontab kita berasumsi menggunakan daemon agar file yang berekstensi .png dalam suatu folder dipindah ke /home/user/modul2/gambar secara realtime.

### Program C
```c
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
        memset(tambahan, '\0', sizeof(tambahan));
        memset(dari, '\0', sizeof(dari));
        
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
+ ` panjang = panjang_string - 4;` variabel untuk menampung panjang karakter dari nama gambar tanpa ekstensi .pgn.
+ `memset` membuat agar kondisi awal dari aray menjadi kosong
+ `strcpy(dari, "/home/yasinta/Pictures/");` path sumber dari file gambar yang akan di pindah
+ `strcat(dari, isi->d_name);` menggabungkan path sumber dan nama file
+ `strcpy(file_baru, "/home/yasinta/modul2/gambar/");` path tujuan dari file gambar
+ `strncat(file_baru, isi->d_name, panjang);` menggabungkan path tujuan dan nama file tanpa ekstensi. Misal /home/yasinta/Picture/topi
+ `strcpy(tambahan, "_grey.png");` untuk menambahkan karakter *_grey.png* di belakang nama gambar
+ `strcat(file_baru, tambahan);` variable file baru nantinya berisi path tujuan dan nama filenya. Misal /home/yasinta/modul2/gambar/topi_grey.png

**wait, fork dan exec dalam main program**
```c
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
```
+ `char *argv[4] = {"mv", dari, file_baru, NULL};` memindahkan file gambar dari direktori `/home/yasinta/Picture/(nama_file)` ke `/home/yasinta/modul2/gambar/(nama_file)_grey.png`
+ `while ((wait(&status)) > 0);` parent akan menunggu sampai proses pada child selesai

## No 2
Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file bernama “elen.ku” pada direktori “hatiku”. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat program C yang bisa mendeteksi owner dan group dan menghapus file “elen.ku” setiap 3 detik dengan syarat ketika owner dan grupnya menjadi “www-data”. Ternyata kamu memiliki kendala karena permission pada file “elen.ku”. Jadi, ubahlah permissionnya menjadi 777. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on.
Catatan: Tidak boleh menggunakan crontab

Berdasarkan soal no 2. Langkah yang dibutuhkan adalah :
<
