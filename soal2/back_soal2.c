int main (){

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
}