#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

sem_t *sem;
static int cnt = 0;

void p()
{
	cnt--;
	sem_post(sem);
}

void v()
{
	cnt++;
	sem_wait(sem);
}

int main(int argc, char **argv)
{
	const char *name = "posix_sem";
	unsigned int value = 8;

	sem = sem_open(name, O_CREAT, S_IRUSR | S_IWUSR, value);
	
	while(cnt >= 8)
	{
		if(cnt >= 8)
		{
			p();
			printf("decrease : %d\n", cnt);
		}
		else
		{
			v();
			printf("increase : %d\n", cnt);
			usleep(100);
		}
	}

	sem_close(sem);
	printf("sem_destroy return value : %d\n", sem_destroy(sem));

	sem_unlink(name);

	return 0;
}
