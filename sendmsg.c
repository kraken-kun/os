#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MAX_MESSAGE_SIZE 256
#define MESSAGE_QUEUE_KEY 12345

struct message_buffer {
    long message_type;
    char message_text[MAX_MESSAGE_SIZE];
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./message_sender <message1> <message2> ...\n");
        return 1;
    }

    key_t key;
    int msgid;

    // Generate a unique key for the message queue
    key = ftok(".", MESSAGE_QUEUE_KEY);

    // Create a message queue or get the ID of an existing one
    msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
        return 1;
    }

    // Send messages to the message queue
    for (int i = 1; i < argc; i++) {
        struct message_buffer msg;
        msg.message_type = i;  // Use the argument index as the message type
        strncpy(msg.message_text, argv[i], MAX_MESSAGE_SIZE - 1);
        msg.message_text[MAX_MESSAGE_SIZE - 1] = '\0';

        // Send the message
        if (msgsnd(msgid, &msg, sizeof(struct message_buffer) - sizeof(long), 0) == -1) {
            perror("msgsnd");
            return 1;
        }
    }

    printf("Messages sent successfully.\n");

    return 0;
}
