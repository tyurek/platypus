#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <inttypes.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include <time.h>

int decdigits[] = {
        1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5,
        5, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10,
        10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 15,
        15, 15, 16, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19, 19,
        20
};

#define mo_hex  0x01
#define mo_dec  0x02
#define mo_oct  0x03
#define mo_raw  0x04
#define mo_uns  0x05
#define mo_chx  0x06
#define mo_mask 0x0f
#define mo_fill 0x40
#define mo_c    0x80
static int doing_for_all = 0;
unsigned int highbit = 63, lowbit = 0;
int mode = mo_hex;

void rdmsr_on_cpu(uint32_t reg, int cpu)
{
	uint64_t data;
	int fd;
	char *pat;
	int width;
	char msr_file_name[64];
	unsigned int bits;

	sprintf(msr_file_name, "/dev/cpu/%d/msr", cpu);
	fd = open(msr_file_name, O_RDONLY);
	if (fd < 0) {
		if (errno == ENXIO) {
			if (doing_for_all)
				return;
			fprintf(stderr, "rdmsr: No CPU %d\n", cpu);
			exit(2);
		} else if (errno == EIO) {
			fprintf(stderr, "rdmsr: CPU %d doesn't support MSRs\n",
				cpu);
			exit(3);
		} else {
			perror("rdmsr: open");
			exit(127);
		}
	}

	if (pread(fd, &data, sizeof data, reg) != sizeof data) {
		if (errno == EIO) {
			fprintf(stderr, "rdmsr: CPU %d cannot read "
				"MSR 0x%08"PRIx32"\n",
				cpu, reg);
			exit(4);
		} else {
			perror("rdmsr: pread");
			exit(127);
		}
	}

	close(fd);

	bits = highbit - lowbit + 1;
	if (bits < 64) {
		/* Show only part of register */
		data >>= lowbit;
		data &= (1ULL << bits) - 1;
	}

	pat = NULL;

	width = 1;		/* Default */
	switch (mode) {
	case mo_hex:
		pat = "%*llx\n";
		break;
	case mo_chx:
		pat = "%*llX\n";
		break;
	case mo_dec:
	case mo_dec | mo_c:
	case mo_dec | mo_fill | mo_c:
		/* Make sure we get sign correct */
		if (data & (1ULL << (bits - 1))) {
			data &= ~(1ULL << (bits - 1));
			data = -data;
		}
		pat = "%*lld\n";
		break;
	case mo_uns:
		pat = "%*llu\n";
		break;
	case mo_oct:
		pat = "%*llo\n";
		break;
	case mo_hex | mo_c:
		pat = "0x%*llx\n";
		break;
	case mo_chx | mo_c:
		pat = "0x%*llX\n";
		break;
	case mo_oct | mo_c:
		pat = "0%*llo\n";
		break;
	case mo_uns | mo_c:
	case mo_uns | mo_fill | mo_c:
		pat = "%*lluU\n";
		break;
	case mo_hex | mo_fill:
		pat = "%0*llx\n";
		width = (bits + 3) / 4;
		break;
	case mo_chx | mo_fill:
		pat = "%0*llX\n";
		width = (bits + 3) / 4;
		break;
	case mo_dec | mo_fill:
		/* Make sure we get sign correct */
		if (data & (1ULL << (bits - 1))) {
			data &= ~(1ULL << (bits - 1));
			data = -data;
		}
		pat = "%0*lld\n";
		width = decdigits[bits - 1] + 1;
		break;
	case mo_uns | mo_fill:
		pat = "%0*llu\n";
		width = decdigits[bits];
		break;
	case mo_oct | mo_fill:
		pat = "%0*llo\n";
		width = (bits + 2) / 3;
		break;
	case mo_hex | mo_fill | mo_c:
		pat = "0x%0*llx\n";
		width = (bits + 3) / 4;
		break;
	case mo_chx | mo_fill | mo_c:
		pat = "0x%0*llX\n";
		width = (bits + 3) / 4;
		break;
	case mo_oct | mo_fill | mo_c:
		pat = "0%0*llo\n";
		width = (bits + 2) / 3;
		break;
	case mo_raw:
	case mo_raw | mo_fill:
		fwrite(&data, sizeof data, 1, stdout);
		break;
	case mo_raw | mo_c:
	case mo_raw | mo_fill | mo_c:
		{
			unsigned char *p = (unsigned char *)&data;
			int i;
			for (i = 0; i < sizeof data; i++) {
				printf("%s0x%02x", i ? "," : "{",
				       (unsigned int)(*p++));
			}
			printf("}\n");
		}
		break;
	default:
		fprintf(stderr, "Impossible case\n");
		exit(127);
	}

	if (width < 1)
		width = 1;

	if (pat) {
		if (doing_for_all)
			printf("CPU %d: ", cpu);
		printf(pat, width, data);
	}
	return;
}

int main() {
	FILE *fp;
	char str[20];
	//fp = popen("./rdmsr -u 1593", "r");
	for(int i = 1; i < 30000000L; i++){
		//fp = popen("./rdmsr -u 1593", "r");
		//fgets(str, 12, fp);
		//puts(str);
		nanosleep((const struct timespec[]){{0, 10000L }}, NULL);
		rdmsr_on_cpu(1593, 0);
	}
	return(0);
}
