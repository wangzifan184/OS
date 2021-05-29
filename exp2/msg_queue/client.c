#include "msgcomm.h"

int main()
{
	int qid = 0, pid = 0;
	struct msg_s snd;
	char buffer[BUFFER_SIZE] = {'\0'};
	char infile[] = "test_in.txt";
	int nread = 0;

	printf("client PID = %d.\n", getpid());

	FILE *in = fopen(infile, "r");

	while( (nread = fread(buffer, sizeof(char), BUFFER_SIZE - sizeof(long), in)) > 0 )
	{	
		qid = msgget(MSGKEY, IPC_CREAT | 0666);

		// msgtype of client -> server is C2S: `123`
		snd.msgtype = C2S;
		
		pid = getpid();

		// clear the snd.text
		memset(snd.text, 0, sizeof(snd.text));

		strcpy(snd.text, buffer);

		// snd.length = nread;
		snd.length = strlen(snd.text);

		printf("client send message : %s.\n", snd.text);

		// the maximun can be sended is BUFFER_SIZE - sizeof(long)
		msgsnd(qid, &snd, sizeof(snd.text), 0);

		// msgtype of server -> client is S2C: `456`
		msgrcv(qid, &snd, 512, S2C, MSG_NOERROR);
		
		memset(buffer, 0, sizeof(buffer));	
		memset(snd.text, 0, sizeof(snd.text));
	}

	fclose(in);
	
	return 0;
}

