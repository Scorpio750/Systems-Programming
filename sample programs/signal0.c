#include <signal.h>
#include <stdio.h>
#include <unistd.h>

// Minimal signal handler demonstration. Handles user-caused SIGINT, then quits.

static void sigint_handler(int signo) {
	printf("Signal handler invoked. Delivered signal is %s.\n", sys_siglist[signo]);
}

int main() {
	struct sigaction action;

	printf("main() invokedi n process %d.\n", getpid());

	action.sa_flags = 0;
	action.sa_handler = sigint_handler; /* short form */
	sigemptyset(&action.sa_mask);		/* no additional signals blocked */
	sigaction( SIGINT, &action, 0);

	pause();							/* wait for a signal. any signal. */
	printf("Normal end.\n");
}
