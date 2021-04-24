#ifndef RS12_H
#define RS12_H

/************************************************************************/
/* NAME                                                                 */
/*      rs12.h                                                          */
/* DESCRIPTION                                                          */
/*      Roland 12-bit-sampler disk format for the S-550, S-330, etc.).  */
/* AUTHOR                                                               */
/*      Copyright 1992 by Gary J. Murakami <gjm@research.att.com>       */
/*      V1.0 (4 May 1992) Freely distibutable for non-comercial use.    */
/*      All other rights reserved.                                      */
/************************************************************************/

#define SamplesPerSegment 12288

typedef struct systemProgram {
  char data[0176000]; /* 0xFC00 == 64512 */
} SystemProgram;

typedef struct patchParameter {
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
} PatchParameter;

typedef struct functionParameter {
  uint8_t masterTune;
  uint8_t dummy1[13];
  uint8_t audioTrig;
  uint8_t dummy2[1];
  uint8_t voiceMode;
  uint8_t multiMidiRxCh[8];
  uint8_t multiPatchNumber[8];
  uint8_t dummy3[9];
  uint8_t keyboardDisplay;
  uint8_t multiLevel[8];
  uint8_t diskLabel[60];
  uint8_t dummy4[4];
  uint8_t externalController;
  uint8_t dummy5[140];
} FunctionParameter;

typedef struct midiParameter {
  uint8_t dummy1[32];
  uint8_t rxChannel[8];
  uint8_t rxProgramChange[8];
  uint8_t rxBender[8];
  uint8_t rxModulation[8];
  uint8_t rxHold[8];
  uint8_t rxAfterTouch[8];
  uint8_t rxVolume[8];
  uint8_t rxBendRange[8];
  uint8_t dummy2[1];
  uint8_t systemExclusive;
  uint8_t deviceId;
  uint8_t dummy3[1];
  uint8_t rxProgramChangeNumber[32];
  uint8_t dummy4[124];
} MidiParameter;

typedef struct toneParameter {
  uint8_t toneName[8];
  uint8_t outputAssign;
  uint8_t sourceTone;
  uint8_t orgSubTone;
  uint8_t samplingFrequency;
  uint8_t origKeyNumber;
  uint8_t waveBank;
  uint8_t waveSegmentTop;
  uint8_t waveSegmentLength;
  uint8_t startPoint[3];
  uint8_t endPoint[3];
  uint8_t loopPoint[3];
  uint8_t loopMode;
  uint8_t tvaLfoDepth;
  uint8_t dummy1[1];
  uint8_t lfoRate;
  uint8_t lfoSync;
  uint8_t lfoDelay;
  uint8_t dummy2[1];
  uint8_t lfoMode;
  uint8_t oscLfoDepth;
  uint8_t lfoPolarity;
  uint8_t lfoOffset;
  uint8_t transpose;
  uint8_t fineTune;
  uint8_t tvfCutOff;
  uint8_t tvfResonance;
  uint8_t tvfKeyFollow;
  uint8_t dummy3[1];
  uint8_t tvfLfoDepth;
  uint8_t tvfEgDepth;
  uint8_t tvfEgPolarity;
  uint8_t tvfLevelCurve;
  uint8_t tvfKeyRateFollow;
  uint8_t tvfVelocityRateFollow;
  uint8_t dummy4[1];
  uint8_t tvfSwitch;
  uint8_t benderSwitch;
  uint8_t tvaEnvSustainPoint;
  uint8_t tvaEnvEndPoint;
  uint8_t tvaEnvLevel1;
  uint8_t tvaEnvRate1;
  uint8_t tvaEnvLevel2;
  uint8_t tvaEnvRate2;
  uint8_t tvaEnvLevel3;
  uint8_t tvaEnvRate3;
  uint8_t tvaEnvLevel4;
  uint8_t tvaEnvRate4;
  uint8_t tvaEnvLevel5;
  uint8_t tvaEnvRate5;
  uint8_t tvaEnvLevel6;
  uint8_t tvaEnvRate6;
  uint8_t tvaEnvLevel7;
  uint8_t tvaEnvRate7;
  uint8_t tvaEnvLevel8;
  uint8_t tvaEnvRate8;
  uint8_t dummy5[1];
  uint8_t tvaEnvKeyRate;
  uint8_t level;
  uint8_t envVelRate;
  uint8_t recThreshold;
  uint8_t recPreTrigger;
  uint8_t recSamplingFrequency;
  uint8_t recStartPoint[3];
  uint8_t recEndPoint[3];
  uint8_t recLoopPoint[3];
  uint8_t zoomT;
  uint8_t zoomL;
  uint8_t copySource;
  uint8_t loopTune;
  uint8_t tvaLevelCurve;
  uint8_t dummy6[12];
  uint8_t loopLength[3];
  uint8_t pitchFollow;
  uint8_t envZoom;
  uint8_t tvfEnvSustainPoint;
  uint8_t tvfEnvEndPoint;
  uint8_t tvfEnvLevel1;
  uint8_t tvfEnvRate1;
  uint8_t tvfEnvLevel2;
  uint8_t tvfEnvRate2;
  uint8_t tvfEnvLevel3;
  uint8_t tvfEnvRate3;
  uint8_t tvfEnvLevel4;
  uint8_t tvfEnvRate4;
  uint8_t tvfEnvLevel5;
  uint8_t tvfEnvRate5;
  uint8_t tvfEnvLevel6;
  uint8_t tvfEnvRate6;
  uint8_t tvfEnvLevel7;
  uint8_t tvfEnvRate7;
  uint8_t tvfEnvLevel8;
  uint8_t tvfEnvRate8;
  uint8_t afterTouchSwitch;
  uint8_t dummy7[2];
} ToneParameter;

typedef struct toneList {
  uint8_t toneName[8];
  uint8_t unknown[8];
} ToneList;

typedef struct waveData {
  uint8_t data[SamplesPerSegment * 3 / 2];
} WaveData;

typedef struct disk {
  SystemProgram systemProgram;         /*  0000 */
  PatchParameter patchParameter[16];   /*  FC00 */
  FunctionParameter functionParameter; /* 10C00 */
  MidiParameter midiParameter;         /* 10D00 */
  ToneParameter toneParameter[32];     /* 10E00 */
  ToneList toneList[32];               /* 11E00 */
  WaveData waveDataA[18];              /* 12000 */
  WaveData waveDataB[18];
} Disk;

#endif
