#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

int system(const char *cmd)
{
	pid_t pid;
	int status;

	if((pid = fork()) < 0)
	{
		status = -1;
	}
	else if(pid == 0)
	{
		execl("/bin/sh", "sh", "-c", cmd, (char *)0);
		_exit(127);
	}
	else 
	{
		while(waitpid(pid, &status, 0) < 0)
		if(errno != EINTR)
		{
			status = -1;
			break;
		}
	}
	printf("\n%d\n", status);

	return status;
}

extern char** environ;

int main(int argc, char **argv)
{
	while(*environ)
		printf("%s\n", *environ++);

	system("who");
	system("nocommand");
	system("cal");
	system("ls -2");

	int stt = system("who");
	
	int stt2 = system("nocommand");
	printf("\n %d  %d \n", stt, stt2);
	return 0;
}	

