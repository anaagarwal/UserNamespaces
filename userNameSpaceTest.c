#define _GNU_SOURCE
#include <fcntl.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <linux/fs.h>
#include <linux/capability.h>

int childFunc();
int childFunc()
{
	printf("Am in child");
        int val = 0;
	//long fd = (long) arg;
	sleep(2);    
	unshare(CLONE_NEWNS);
	mkdir("/mnt/mntpts",0777);
	mkdir("/mnt/mntpts/proc",0777);
	mkdir("/mnt/mntpts/bin",0777);
	mkdir("/mnt/mntpts/dev",0777);
	mkdir("/mnt/mntpts/etc",0777);
	mkdir("/mnt/mntpts/lib",0777);
	mkdir("/mnt/mntpts/root",0777);
	mkdir("/mnt/mntpts/lib64",0777);
        mkdir("/mnt/mntpts/usr",0777);
	val = mount("/bin","/mnt/mntpts/bin","none",MS_BIND,NULL);
        if(val<0) {
          perror("binerror");
          printf("lib64 %d",val);
        }
	val = mount("/dev","/mnt/mntpts/dev","none",MS_BIND,NULL);
	val = mount("/etc","/mnt/mntpts/etc","none",MS_BIND,NULL);
	val = mount("/lib","/mnt/mntpts/lib","none",MS_BIND,NULL);
	val = mount("/root","/mnt/mntpts/root","none",MS_BIND,NULL);
	val = mount("/proc","/mnt/mntpts/proc","none",MS_BIND,NULL);
	val = mount("/lib64","/mnt/mntpts/lib64","none",MS_BIND,NULL);
	val = mount("/usr","/mnt/mntpts/usr","none",MS_BIND,NULL);	
	val = chroot("/mnt/mntpts");
	if(val<0) {
          perror("chrooterror");
          printf("lib64 %d",val);
        }
	chdir("/");

	system("/bin/bash"); // XXX: use execve
    return 0;
}

static int childFunc3(void *arg)
{
    int val;
    long euid, egid, uid, gid;

    sleep(1); // XXX: wait on a pipe

    euid = (long)geteuid();
    egid = (long)getegid();
    uid = (long)getuid();
    gid = (long)getgid();
    printf("childFunc(): EUID  = %ld\n",euid);
    printf("childFunc(): EGID = %ld\n", egid);
    printf("childFunc(): UID  = %ld\n",uid);
    printf("childFunc(): GID = %ld\n", gid);

    if (uid == 0) {
	val = childFunc();      
    }
    else {
	printf("user does not have root permissions.Exiting from the child process");
	}


sleep(3);
return 0;
}

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];   

int main(int argc, char *argv[])
{
    pid_t child_pid;
    int i =0;
    long fd =3;
    char procfname[20];
    FILE * procfile;
    child_pid = clone(childFunc3, child_stack + STACK_SIZE, CLONE_NEWUSER | SIGCHLD, (void *) fd); // XXX: create a process
    if (child_pid == -1)
        perror("clone error");

    sprintf(procfname, "/proc/%d/uid_map", child_pid);
    procfile = fopen(procfname, "w");
    fprintf(procfile, "%d %d %d", 0, getuid(), 1);
    fclose(procfile);
    sprintf(procfname, "/proc/%d/gid_map", child_pid);
    procfile = fopen(procfname, "w");
    fprintf(procfile, "%d %d %d", 0, getgid(), 1);
    fclose(procfile);

    if (waitpid(child_pid, NULL, 0) == -1)  
        printf("\n waitpid \n");
    for(i=0;i<5;i++){

	printf("\n Back to parent process\n");
		
    }
    exit(EXIT_SUCCESS);
}

