#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;
	int status;

	system("ls -2");
	if((pid = fork()) < 0)
	{
		exit(127);
	}
	else if(pid == 0)
	{
		return 127;
	}
	else
	{
		waitpid(pid, &status, 0);
		printf("Exit Value : %d\n", WEXITSTATUS(status));
	}
	return 0;
}

