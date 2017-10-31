#include <stdio.h>

int main() {

	int esi = 0;
	int edx = 0;

	for (int i = '0'; i <= 'z'; i++) {
		for (int j = '0'; j <= 'z'; j++) {

			esi = 0;
			edx = 0;

			esi = i;
			esi += edx;
			esi *= 0x772;
			edx = esi;
			edx *= esi;
			esi += edx;
			esi |= esi;
			esi *= 0x474;
			esi += esi;
			edx = esi;

			esi = j;
			esi += edx;
			esi *= 0x772;
			edx = esi;
			edx *= esi;
			esi += edx;
			esi |= esi;
			esi *= 0x474;
			esi += esi;
			edx = esi;
	
			esi >>= 16;

			if (esi == 0x5D88) {
				printf("%c%c\n",i,j);
			}
		}
	}

	return 0;
}