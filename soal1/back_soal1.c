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
        printf("dari: %s\nke: %s\n",dari,file_baru);

      //wait, fork, exec
        pid_t child_id1;
        int status;
        
        child_id1 = fork();

        if (child_id1 == 0) {
          char *argv[4] = {"mv", dari, file_baru, NULL};
          execv("/bin/mv", argv);
        }
        else{
          while ((wait(&status)) > 0);
        }
        
      }
  }
  closedir(folder);