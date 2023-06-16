#define SWRAP_IMPLEMENTATION
#include <swrap.h>

#include <stdint.h>
#include <stdio.h> // for puts

#include <track.h> // for TRACK_SEND_BUFSIZE

char buf[TRACK_SEND_BUFSIZE];
int bufsiz = 0;

int main(int argc, char **argv) {
	char *host = NULL;
	char *port = "8200";

	if (argc > 1)
		port = argv[1]; // port

	if (argc > 2)
		host = argv[2]; // address

	if (swrapInit())
		return -1;

	int sock = swrapSocket(SWRAP_UDP, SWRAP_BIND, SWRAP_DEFAULT, host, port);

	if (sock == -1)
		return -2;

	while(1) {
		bufsiz = swrapReceive(sock, buf, TRACK_SEND_BUFSIZE);
		if (bufsiz < 0)
			break;
		buf[bufsiz] = 0;
		puts(buf);
	}

	swrapClose(sock);
	swrapTerminate();
	return 0;
}
