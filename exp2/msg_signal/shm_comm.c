#include "shm_comm.h"

int main()
{
	int memid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
	int semid_w = 0, semid_r = 0, semid_mutex = 0;
	int pid = 0;
	
	printf("create shared memory, id = %d.\n", memid);

	char * memaddr = shmat(memid, NULL, 0);

	do
	{
		semid_w = createsig(SEMKEY1, 1);
	}while(semid_w == -1);

	do
	{	
		semid_r = createsig(SEMKEY2, 0);
	}while(semid_r == -1);

	do
	{
		semid_mutex = createsig(SEMKEY3, 1);
	}while(semid_mutex == -1);

	printf("create semaphores : %d, %d, %d\n", semid_w, semid_r, semid_mutex);

	/*
	 * child process
	 * child reads the msg from memory and writes to the file
	 */
	if(!(pid = fork()))
	{
		printf("This is child process, PID = %d, parent PID = %d.\n", getpid(), getppid());

		P(semid_mutex);
		
		// write to the memory
		char tmpbuffer[SHM_SIZE + 1] = {'\0'};
		strcpy(tmpbuffer, memaddr);
		tmpbuffer[SHM_SIZE] = '\0';
		// strcpy(memaddr, childtext);
		// memset(memaddr, 0, SHM_SIZE);
		strcpy(memaddr, tmpbuffer);
		
		V(semid_mutex);
		
		char * output = "test_out.txt";
		char buffer_c[SHM_SIZE + 1] = {'\0'};

		FILE * out = fopen(output, "w");
		
		while (1)
		{
			// wait for the buffer to read
			P(semid_r);
			printf("child enter shared memory.\n");
			strcpy(buffer_c, memaddr);
			buffer_c[SHM_SIZE] = '\0';
			// printf("msg will be written to the file is: %s.\n", buffer_c);
			fwrite(buffer_c, sizeof(char), strlen(buffer_c), out);// SHM_SIZE, out);
			// fclose(out);
			
			printf("chile leave shared memory\n");
			// buffer can be written
			V(semid_w);
			// printf("[Child] Child leaves the area.The buffer can be written.\n");
			if(strlen(buffer_c) < SHM_SIZE)
				break;
			memset(buffer_c, 0, SHM_SIZE);
		}
		fclose(out);
	}
	/*
	 * father process
	 * father writes the momery
	 */
	else
	{
		printf("This is parent process, PID = %d.\n", getpid());
                
		
		P(semid_mutex);
                
		// write to the memory
		char tmpbuffer[SHM_SIZE + 1] = {'\0'};
		strcpy(tmpbuffer, memaddr);
		// strcpy(memaddr, fathertext);
		strcpy(memaddr, tmpbuffer);
		
		V(semid_mutex);
		
		
		char *input = "test_in.txt";

		FILE *in = fopen(input, "r");
		char buffer_f [SHM_SIZE] = {'\0'};

		// refresh the buffer of stdout
		fflush(stdout);

		int length = 0;

		while ( (length = fread(buffer_f, sizeof(char), SHM_SIZE, in)) > 0)
		{
			// wait for the buffer to write
			P(semid_w);
			printf("parent enter shared memory.\n");

			strcpy(memaddr, buffer_f);
			memset(buffer_f, 0, SHM_SIZE);

			printf("parent leave shared memory\n");
			// buffer can be read
			V(semid_r);
			// printf("[Father] Father leaves the area.The buffer can be read\n");
		}
		fclose(in);	
	}

	return 0;
}
