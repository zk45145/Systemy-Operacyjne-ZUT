#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char **argv)
{
	if (argc != 2 || strlen(argv[1])>20)
	{
		fprintf (stderr, "nieprawidlowy argument: podaj 1 argument o dlugosci max 20 znakow!\n");
		return 1;
	}
	if (strlen(argv[1])==1) return((int)argv[1][0]);
	char lancuch1[10]="", lancuch2[10]="";
	int a=0, dlugosc = strlen (argv[1]);
	for (int i=0; i<dlugosc/2; i++)
	{
		lancuch1[i]	= argv[1][i];
	}
	for (int i=dlugosc/2; i<dlugosc; i++)
	{
		lancuch2[a] = argv[1][i];
		a++;
	}
	pid_t pid1, pid2;
	int status1, status2 = 0;
	pid1=fork();
	if (pid1>0)
	{
		pid2=fork();
		if(pid2>0) {
		    wait(&status1);
		    wait(&status2);
		}
	}
	if (pid1==0) execl (argv[0], argv[0], lancuch1, (char*) NULL);
	usleep(20000);
	if (pid2==0) execl (argv[0], argv[0], lancuch2, (char*) NULL);

	printf("%d %d      %s %c\n", getpid(), pid1, lancuch1, (char)WEXITSTATUS(status1));
	printf("%d %d      %s %c\n", getpid(), pid2, lancuch2, (char)WEXITSTATUS(status2));
	return WEXITSTATUS(status1)>WEXITSTATUS(status2) ? WEXITSTATUS(status1):WEXITSTATUS(status2);
}
