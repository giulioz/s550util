#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef NEED_STDINT_H
#include <stdint.h>
#endif

#ifdef IS_BIG_ENDIAN
/* Macros for swapping constant values in the preprocessing stage. */
#define LE_16(x) ((((x)&0xff00) >> 8) | (((x)&0x00ff) << 8))

#define LE_32(x) ((LE_16(x) << 16) | (LE_16((x) >> 16)))
#else
#define LE_16(x) (x)
#define LE_32(x) (x)
#endif

#include "rs12.h"
#include "s50dir.h"

#define MYNAME "s550util"

#ifdef DEBUG
void dumpData(uint8_t *data, int len) {
  int i, pos;
  pos = 0;
  while (len > 0) {
    printf("%03x: ", pos);
    for (i = 0; i < 16; i++)
      if (i < len)
        printf("%02x ", data[pos + i]);
      else
        printf("   ");
    printf("'");
    for (i = 0; i < 16; i++)
      if (i < len)
        printf("%c", isprint(data[pos + i]) ? data[pos + i] : '.');
      else
        printf(" ");
    printf("'\n");
    pos += 16;
    len -= 16;
  }
}
#endif

enum { S_50, S_550, S_330, S_NONE };

struct {
  char cRIFF[4];       /* RIFF */
  uint32_t lentotal;   /* total length */
  char cWAVEfmt[8];    /* WAVEfmt */
  uint32_t lenfmt;     /* 16 */
  uint16_t format;     /* 1=PCM */
  uint16_t mode;       /* 1=mono */
  uint32_t sfreq;      /* sample frequency */
  uint32_t bps;        /* byte/s */
  uint16_t byte_p_spl; /* sample size in bytes */
  uint16_t bit_p_spl;  /* sample size in bit */
  char cdata[4];       /* data */
  uint32_t datalen;    /* data length */
} wavHeader = {"RIFF", 0, "WAVEfmt ", LE_32(16), LE_16(1), LE_16(1),
               0,      0, LE_16(2),   LE_16(16), "data",   0};

char gDiskLabel[12];
ToneList gToneList[32];
ToneParameter gToneParameters[32];
int rawmode = 0;

int disktype(FILE *f) {
  uint8_t buf[8];
  rewind(f);
  if ((fread(buf, 1, 8, f)) != 8) return S_NONE;
#ifdef DEBUG
  printf("First 8 bytes of disk or image\n");
  dumpData(buf, 8);
#endif

  if (*(uint32_t *)buf != 0) return S_NONE;
  if (!strncmp(buf + 4, "S-50", 4)) return S_50;
  if (!strncmp(buf + 4, "S550", 4)) return S_550;
  if (!strncmp(buf + 4, "S330", 4)) return S_330;
  return S_NONE;
}

/* read disk label and tone parameters from file */

int readSDiskDirectory(FILE *f) {
  rewind(f);
  if (fseek(f, OFFSET_LABEL, SEEK_SET)) return 1;
  if (fread(gDiskLabel, 1, 12, f) != 12) return 1;
  if (fseek(f, OFFSET_TONE, SEEK_SET)) return 1;
  if (fread(&gToneParameters, sizeof(ToneParameter), 32, f) != 32) return 1;
  return 0;
}

void help(void) {
  printf(MYNAME " v. 1.01 (" __DATE__
                ") (c) 2004-2006 Peter Heitzer\nUsage: " MYNAME
                "  -h|-l <file>|[-r] -{x|X} <file> [s1 .. sn]\n");
  printf(
      "  -h: show help\n"
      "  -r: raw mode, ignore tone start and end points\n"
      "  -l <file>:  show info for image <file>\n"
      "  -x <file> <s1> .. <sn> extract samples s1 .. sn\n"
      "  -X <file> : as -x but overwrite files\n");
  printf(
      "Sample numbers are in Roland notation, e.g. 11 for first sample.\n"
      "A range of sample numbers can be given as <first>-<last>, e.g. 14-16. "
      "Only\n"
      "original tones can be extracted.\n");
}

/* convert 3 byte long to long */

int32_t b32l(uint8_t b[]) {
  return ((uint32_t)b[0] << 16) | ((uint32_t)b[1] << 8) | ((uint32_t)b[3]);
}

/*
  read one 12bit sample from wave data

00 01 02 03 04 05 06 07 08
hh ll hh hh ll hh hh ll hh
s0    s1 s2    s3 s4    s5

i=n+n/2;
even sample:  high(n)=w[i]; low(n)=w[i+1]>>4;
odd sample:   high(n)=w[i+1]; low(n)=w[i]& 0xf;
*/

uint16_t getw12(uint8_t wave[], long n) {
  int i;

  i = n + n / 2;
  if (n & 1)
    return ((uint16_t)wave[i + 1] << 4) | wave[i] & 0xf;
  else
    return ((uint16_t)wave[i] << 4) | (wave[i] >> 4);
}

void toneInfo(FILE *f) {
  int i;
  ToneParameter *tp;

  for (i = 0, tp = gToneParameters; i < 32; i++, tp++) {
    if (tp->waveSegmentLength) {
      printf("%d%d: '%.8s' %s %s,", i / 8 + 1, i % 8 + 1, tp->toneName,
             tp->orgSubTone ? "subtone, " : "origtone,",
             tp->samplingFrequency ? "15kHz" : "30kHz");
      printf(" bank: %c, ", tp->waveBank ? 'B' : 'A');
      printf("S:%06ld, E:%06ld, L:%06ld (%d,%d)\n", b32l(tp->startPoint),
             b32l(tp->endPoint), b32l(tp->loopPoint), tp->waveSegmentTop,
             tp->waveSegmentLength);
    }
  }
}

int diskinfo(char *fn) {
  FILE *f;
  int stype;

  if ((f = fopen(fn, "rb")) == NULL) return 2;

  switch ((stype = disktype(f))) {
    case S_50:
      printf("S-50");
      break;
    case S_550:
      printf("S-550");
      break;
    case S_330:
      printf("S-330");
      break;
    default:
      printf("Unknown");
  }
  printf(" disk");

  if (stype != S_NONE && !readSDiskDirectory(f)) {
    printf(" '%.12s'\n", gDiskLabel);
#ifdef DEBUG
    printf("Disk label:\n");
    dumpData(gDiskLabel, 12);
    printf("Tones:\n");
    dumpData((uint8_t *)gToneParameters, sizeof(gToneParameters));
#endif
    toneInfo(f);
  }
  fclose(f);
  printf("\n");
  return 0;
}

/* write wav header for tone */

void writeWavHeader(FILE *f, int frequency, long len) {
  /*
  ToneParameter *tp;

  tp=&gToneParameters[num];

  if (tp->samplingFrequency) {
    wavHeader.sfreq=LE_32(15000);
          wavHeader.bps=LE_32(30000);
          }
  else {
    wavHeader.sfreq=LE_32(30000);
          wavHeader.bps=LE_32(60000);
          }
  wavHeader.datalen=LE_32(2*(b32l(tp->endPoint)-b32l(tp->startPoint)));
  */
  wavHeader.datalen = LE_32(len);
  wavHeader.sfreq = LE_32(frequency);
  wavHeader.bps = LE_32(2 * frequency);
  wavHeader.lentotal = LE_32(wavHeader.datalen + 44);
  fwrite(&wavHeader, 1, sizeof(wavHeader), f);
}

/* convert tone number in Roland notation to integer */
int rtoi(int rnum) { return 8 * ((rnum - 11) / 10) + (rnum - 11) % 10; }

/* extract one tone to current directory  */

void extractTone(FILE *f, int num, int overwrite) {
  long numSamples, startPoint, endPoint;
  char buf[16], tmp[16], *p;
  uint8_t *wavedata;
  FILE *tf;
  int i;
  ToneParameter *tp;
  uint16_t w16;

  tp = &gToneParameters[num];

  if (num < 0 || num > 31 || (tp->waveSegmentLength == 0)) return;
  if (tp->orgSubTone) return;
  if (rawmode) {
    startPoint = 0;
    endPoint = tp->waveSegmentLength * SEGMENT_WORDS;
  } else {
    startPoint = b32l(tp->startPoint);
    endPoint = b32l(tp->endPoint);
  }

  numSamples = endPoint - startPoint;
  sprintf(buf, "%.8s", tp->toneName);
  for (i = 7; i && buf[i] == ' '; i--) buf[i] = 0;
  while (p = strchr(buf, ' ')) *p = '_';
  sprintf(tmp, "%s.wav", buf);
  if (!access(tmp, F_OK) && !overwrite) {
    sprintf(tmp, "%d%d%s.wav", num / 8 + 1, num % 8 + 1, buf);
    if (!access(tmp, F_OK) && !overwrite) return;
    strcpy(buf, tmp);
  } else
    strcpy(buf, tmp);
  if (access(buf, F_OK) || overwrite) {
    printf("%d%d -> %s\n", num / 8 + 1, num % 8 + 1, buf);
    if ((wavedata = malloc((long)tp->waveSegmentLength * SEGMENT)) == NULL)
      return;
    if (!fseek(f,
               (tp->waveBank ? OFFSET_WAVEB : OFFSET_WAVEA) +
                   (long)tp->waveSegmentTop * SEGMENT,
               SEEK_SET)) {
      if (fread(wavedata, SEGMENT, tp->waveSegmentLength, f) ==
          tp->waveSegmentLength) {
        if (tf = fopen(buf, "wb")) {
          writeWavHeader(tf, tp->samplingFrequency ? 15000 : 30000,
                         numSamples * 2);
          for (i = startPoint; i < endPoint; i++) {
            w16 = LE_16(getw12(wavedata, i) << 4);
            fwrite(&w16, 1, 2, tf);
          }
          fclose(tf);
        }
      }
    }
    free(wavedata);
  }
}

/* extract range of tones to current directory */

void extractToneRange(FILE *f, int first, int last, int overwrite) {
  for (; first <= last; first++) extractTone(f, first, overwrite);
}

int main(int argc, char *argv[]) {
  int opt, i, extract = 0, overwrite = 0;
  FILE *f;
  char *samdisk, *p;

  while ((opt = getopt(argc, argv, "hl:x:X:r")) != -1) {
    switch (opt) {
      case 'h':
        help();
        return 0;
      case 'l':
        return diskinfo(optarg);
      case 'r':
        rawmode = 1;
        break;
      case 'x':
        samdisk = optarg;
        break;
      case 'X':
        samdisk = optarg;
        overwrite = 1;
        break;
      default:
        return 1;
    }
  }

  if (optind == 1) {
    help();
    return 0;
  }

  if ((f = fopen(samdisk, "rb")) && disktype(f) != S_NONE) {
    if (!readSDiskDirectory(f))
      for (i = optind; i < argc; i++) {
        if ((p = strchr(argv[i], '-')) != NULL) {
          *p = 0;
          extractToneRange(f, rtoi(atoi(argv[i])), rtoi(atoi(p + 1)),
                           overwrite);
        } else
          extractTone(f, rtoi(atoi(argv[i])), overwrite);
      }
    fclose(f);
    return 0;
  }
  return 1;
}
