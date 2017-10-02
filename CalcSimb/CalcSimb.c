#include <stdio.h>

main(){
	long nc;
	nc = 0;
	while(getchar() != 'e')
		++nc;
	printf("%ld\n",nc);
}
