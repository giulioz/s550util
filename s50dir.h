#define SEGMENT 18432l
#define SEGMENT_WORDS 12288l
#define OFFSET_PATCH 0xfc00l
#define OFFSET_LABEL 0x10c33l
#define OFFSET_TONE 0x10e00l
#define OFFSET_WAVEA 0x12000l
#define OFFSET_WAVEB 0x12000l + 18 * SEGMENT

typedef struct {
  char name[8];
  uint8_t f1;
  uint8_t sourceTone;     // 0-31
  uint8_t subToneFlag;    // 1= sub tone
  uint8_t frequency;      // 0=30, 1=15 kHz
  uint8_t origKeyNumber;  // 11-120
  uint8_t waveBank;       // 0=A, 1=B
  uint8_t waveStart;      // 0-17 memory units
  uint8_t waveLen;        // 0-18 memory units
  uint8_t startPoint[3];  // H M L byte
  uint8_t endPoint[3];
  uint8_t loopPoint[3];
  uint8_t loopMode;  // 0,1,2,3=Fwd,Alt,1Shot,Reverse
  uint8_t f2[102];
} TONE;

typedef struct patchParameterS330 {
  uint8_t patchName[12];
  uint8_t bendRange;
  uint8_t dummy1[1];
  uint8_t afterTouchSense;
  uint8_t keyMode;
  uint8_t velocitySwThreshold;
  uint8_t toneToKey1[109];
  uint8_t toneToKey2[109];
  uint8_t copySource;
  uint8_t octaveShift;
  uint8_t outputLevel;
  uint8_t dummy2[1];
  uint8_t detune;
  uint8_t velocityMixRatio;
  uint8_t afterTouchAssign;
  uint8_t keyAssign;
  uint8_t outputAssign;
  uint8_t dummy3[12];
} patchParameterS330;

typedef struct patchParameterS50 {
  uint8_t patchName[12];
  uint8_t bendRange;
  uint8_t dummy1[1];
  uint8_t afterTouchSense;
  uint8_t dummy2[4];
  uint8_t keyMode;
  uint8_t velocitySwThreshold;
  uint8_t dummy3[19];
  uint8_t toneToKey1[128];
  uint8_t toneToKey2[128];
  uint8_t copySource;
  uint8_t octaveShift;
  uint8_t outputLevel;
  uint8_t modulationDepth;
  uint8_t detune;
  uint8_t velocityMixRatio;
  uint8_t afterTouchAssign;
  uint8_t dummy4[209];
} patchParameterS50;
