#include <stdio.h>
#include <stdlib.h>

#define MAX_PACKET_LEN 23

/*
 * Following 3 functions taken from gsmdecode-0.7bis, with permission - http://wiki.thc.org/gsm
 */

static int
hc2b(unsigned char hex)
{
	hex = tolower(hex);
	if ((hex >= '0') && (hex <= '9'))
		return hex - '0';
	if ((hex >= 'a') && (hex <= 'f'))
		return hex - 'a' + 10;
	return -1;
}

static int
hex2bin(unsigned char *out, unsigned char *in)
{
	/* unsigned char *end = in + strlen((char *)in); */
	int c;

	/* /\* Clamp to maximum packet size *\/ */
	/* if (end - in > MAX_PACKET_LEN*2) /\* As we're reading nibbles *\/ */
	/* 	end = in + MAX_PACKET_LEN*2; */

	int is_low = 0;
	int count = 0;
	for (; *in; in++)
	{
		c = hc2b(*in);
		if (c < 0)
			continue;

		if (is_low == 0)
			*out = c << 4;
		else {
			*out |= (c & 0x0f);
			out++;
			if (count++ >= MAX_PACKET_LEN)
				break;
		}
		is_low ^= 1;
	}

	return count;
}

int main()
{
	unsigned char buf[128];
	const char *hexstr = "4D 06 24 00 23 68 AB 4D 23 69 99 D5 22 DB 80 7D 23 65 83 75 83 2B 2B";

	printf("hex string: \n%s\n", hexstr);
	int cnt = hex2bin(buf, hexstr);
	printf("%d hexes:\n", cnt);
	int i;
	for (i=0; i<cnt; i++)
		printf("%02x ", buf[i]);

}
