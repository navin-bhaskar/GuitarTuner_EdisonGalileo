

#ifndef __CONFIG_H
#define __CONFIG_H


/** The sampling rate that we need in Hz, should be supported by your audio card */
#define SAMPLING_RATE        (44100)               
/** The number of "frames" you want to capture */
#define FRAMES_PER_BUFFER    (1024*2)
/** The FFT length */
#define FFT_LENGTH           FRAMES_PER_BUFFER

/** The time you want to listen to the audio */
#define NUM_SECONDS_TO_REC   (1)

/** The sample type that will be used */
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
//#define SAMPLE_TYPE        paFloat32
/** Nothing to be recorded */
#define SAMPLE_SILENCE      (0.0f)

#endif
