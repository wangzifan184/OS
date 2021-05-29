// comm.c

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void sig_p(int dunno)
{
	printf("signal id: %d.\n", dunno);
}


int main()
{
	int pid, sum = 0, status = 0;

	char output[] = "output.txt";

	// bind the signal processing function and SIGUSR1
	signal(SIGUSR1, sig_p);

	while((pid = fork()) == -1);
	
	// father process
	if(pid)
	{
		char buffer[20] = {'\0'};
		int ret = 0;

		printf("This is the father process. Process PID = %d\n", getpid());
		
		printf("Calculation started.\n");
		// main task: calculating
		for(int i = 0; i < 100; ++i)
		{
			sum += i;
		}

		printf("Calculation completed.\n");

		// transport int to char
		sprintf(buffer, "%d", sum); 
		char str[] = "Sum =  ";
		char *final = (char*) malloc(strlen(str) + strlen(buffer) + 1);

		strcpy(final, str);
		strcat(final, buffer);

		FILE *out = fopen(output, "w");
                
        fwrite(final, sizeof(char), strlen(final), out);
		// remember to close the file
		fclose(out);

		printf("result saved.\n");

		// send signal to child
		printf("signal sent\n");
		kill(pid, SIGUSR1);

		printf("sleep 3.\n");
		sleep(3);

		// test child process finished or not
		if( (waitpid(pid, NULL, WNOHANG)) == 0)
		{
			if( (ret = kill(pid, SIGKILL)) == 0)
				printf("killed the child process: %d.\n", pid);
		}
		
		// pid = wait(&status);
		// printf("[Father] child process:%d, status:%d.\n", pid, status);

	}
	// children process
	else
	{
		printf("This is the child process. PID = %d; Parent PID = %d.\n", getpid(), getppid());

		// waiting for the signal
		pause();

		printf("signal received.\n");

		FILE *in = fopen(output, "r");
		char readout[50] = {'\0'};

		fread(readout, sizeof(char), 50, in);
		fclose(in);

		printf("file content :\n\t%s\n", readout);

		// printf("[Child] Child sleep.\n");
		// sleep(10);
		exit(3);
	}

	return 0;
}
