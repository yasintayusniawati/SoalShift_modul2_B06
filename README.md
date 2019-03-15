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
Garis besar soal :
  + Membuat direktori bernama **hatiku** `mkdir hatiku`dan membuat file bernama **elen.ku** `touch elen.ku`
  + Menghapus file **elen.ku** dalam program C dengan syarat owner dan groupnya bernama **www-data**
  + Untuk menghapus filenya permissionnya diubah menjadi 777
  + Karena tidak boleh menggunakan crontab maka menggunakan daemon yang berjalan setiap 3 detik

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
#include <grp.h>
#include <pwd.h>

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

  struct stat infofile;

  chdir("/home/yasinta/Documents/praktikum2/soal2/"); //menentukan programnya akan jalan dimana
  folder=opendir("hatiku");
  while((isi=readdir(folder)) != NULL){
      if(strcmp(isi->d_name,"elen.ku") == 0){

          stat("hatiku/elen.ku",&infofile); //mendapatkan informasi nomor user id dan nomor group id

          //ngambil nama user
          struct passwd *user; // berisi nama dari uid
          user = getpwuid(infofile.st_uid); //getpwuid untuk ngambil detail informasi dari user id dan harus disimpan di struct passwd, variabel pw_name
           //ngambil nama group
            struct group *grup; //berisi nama dari gid
            grup = getgrgid(infofile.st_gid);//getgrgid untuk ngambil detail informasi dari group id dan harus disimpan di struct group, variabel gr_name
            // printf("%s %s\n", user->pw_name,grup->gr_name);
            if( strcmp(user->pw_name,"www-data") == 0 && strcmp(grup->gr_name,"www-data") == 0){
              chmod("hatiku/elen.ku", 0777);
              remove("hatiku/elen.ku");
            }
      }    
  }
   closedir(folder);

   sleep(3);
  }
  
  exit(EXIT_SUCCESS);
}
```
#### Penjelasan 
+ `DIR *folder;` variable dengan nama **folder** dengan tipe data DIR
+ `struct dirent *isi;` isi adalah nama variable yang menampung file *elen.ku*
+ Untuk menentukan programnya akan dijalankan dimana gunakan chdir. karena program ini berada di */home/yasinta/Documents/praktikum2/soal2* dan path ini nantinya akan dibutuhkan untuk menjalankan program dibawahnya sehingga commandnya menjadi :
```c
chdir("/home/yasinta/Documents/praktikum2/soal2/");
```
+ `folder=opendir("hatiku");` variable untuk membuka folder **hatiku**
+ `strcmp(isi->d_name,"elen.ku")` digunakan untuk membandingkan nama file. Jika didalam folder hatiku ada file yang bernama elen.ku maka akan menjalankan program di bawahnya
+ Digunakan stat agar mendapatkan informasi dari suatu file yaitu elenku dalam hal ini kita membutuhkan informasi mengenai user id dan group id. Fungsi bawaan stat adalah hasilnya dimasukkan ke dalam struct stat
```c
stat("hatiku/elen.ku",&infofile);
```
+ Karena fungsi stat hanya memberikan informasi userId dan groupId sehingga diperlukan suatu fungsi untuk mengubah dari userID/groupId menjadi nama dari user dan groupnya, sehingga bisa menggunakan **struct passwd** :
  + untuk user
  ```c
  struct passwd *user; // berisi nama dari uid
  user = getpwuid(infofile.st_uid);
  ```
  getpwuid untuk ngambil detail informasi dari user id dan harus disimpan di struct passwd, variabel pw_name
  
  + untuk group
  ```c
   struct group *grup; //berisi nama dari gid
   grup = getgrgid(infofile.st_gid);
  ```
  getgrgid untuk ngambil detail informasi dari group id dan harus disimpan di struct group, variabel gr_name

+ selanjutnya gunakan strcmp untuk membandingkan jika file tersebut memiliki user dan group **www-data** akan di hapus dengan bantuan chmod. chmod 0777 dengan 0 didepan berati sebuah file dengan modenya dibuat menjadi read, write dan execute. Untuk menghapus file menggunakan remove.
```c
if( strcmp(user->pw_name,"www-data") == 0 && strcmp(grup->gr_name,"www-data") == 0){
   chmod("hatiku/elen.ku", 0777);
   remove("hatiku/elen.ku");
```
