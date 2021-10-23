#include <stdio.h>
#include <stdlib.h>
#include "resolver.h"

int main(int argc, char* argv[]) {
	for (int i = 1; i < argc; i++)
	{
		if (resolve(argv[i]) != 0)
			return -1;
	}
	return 0;
}
