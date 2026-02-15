#ifndef LIGHT_SEQUENCER
#define LIGHT_SEQUENCER

void buildLightSequence(int _pulseCount, unsigned long _timeSpan);
void flashSequence();
void replay();
unsigned long* calculateLightSpans(int pulseCount, unsigned long timeSpan);
void printLightSpan();

#endif