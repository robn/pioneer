#include "libs.h"
#include "Pi.h"
#include <signal.h>
#include <getopt.h>
#include "galaxy/SystemPath.h"
#include "galaxy/StarSystem.h"

void sigsegv_handler(int signum)
{
	if (signum == SIGSEGV) {
		printf("Segfault! All is lost! Abandon ship!\n");
		SDL_Quit();
		abort();
	}
}

int main(int argc, char **argv)
{
	const char *path = 0;

	int opt;
	while ((opt = getopt(argc, argv, "s:h?")) >= 0) {
		switch (opt) {
			case 's':
				path = optarg;
				break;

			default:
				printf("usage: %s [-s path]\n", argv[0]);
				return -1;
		}
	}

//	signal(SIGSEGV, sigsegv_handler);
	Pi::Init();

	if (path) {
		StarSystem::GetCached(SystemPath::Parse(path))->Dump();
		return 0;
	}

	for (;;) Pi::Start();
	return 0;
}
