/* Shree Krishnaya Namaha */
/*
 * (c) Navin Bhaskar <navin.bhaskar.5@gmail.com>
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the  nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "portaudio.h"
#include "libfft.h"
#include <math.h>
#include "filter.h"
#include "config.h"
#include "note.h"
#include "mraa.h"
#include "upm/jhd1313m1.h"


#define SOUNDS "sounds"       /**< The folder where all the sound files are */
#define APLAY  "aplay "       /**< Aplay command to do the playback */

static int gCounter;          /**< Used to detect the button press */
static bool gbKeepGoing;      /**< Flag to indicate whether to continue with the application or not */

/**
 * The button interrupt handler 
 *
 */

void btnInterrupt(void * param)
{
    gCounter++;
}

/** A struct that will be used to define data to be used in 
    the callback to record the data */
typedef struct
{
    int           frameIndex;        /**< The current sample recording location */
    int           maxFrameIndex;     /**< Maximum ammount of samples buffer can hold */
    SAMPLE        *samples;          /**< The samples buffer */
} sampleBuffer;


/**
 * Signal handler, called whenever ctrl+c is pressed to exit the tuner
 */
void signalHandler(int signum)
{
    gbKeepGoing = false;
}

/**
 * The application's entry point.
 */

int main(void)
{
    PaStreamParameters  inputParameters;
    PaStream*           stream;
    PaError             err = paNoError;
    sampleBuffer        data;
    int                 i;
    int                 totalFrames;
    int                 numBytes;
    SAMPLE              max, val;
    double              average;
    float               idata[FFT_LENGTH];
    float               y[FFT_LENGTH];
    mraa_gpio_context   btn;
    upm::Jhd1313m1      *lcd;
    int                 currentNote = NOTE_NOT_FOUND;
    int                 btnStatus;
    bool                audioMode; 
    int                 note;
    char                aplay[50];
    int                 subLen;
  
  
    data.maxFrameIndex = totalFrames =  NUM_SECONDS_TO_REC* SAMPLING_RATE; /* Record for a few seconds. */
    data.frameIndex = 0;
    numBytes = totalFrames * sizeof(SAMPLE);
    data.samples = (SAMPLE *) malloc( numBytes ); 
    if( data.samples == NULL )
    {
        printf("Could not allocate record array.\n");
        goto done;
    }
    for( i=0; i<totalFrames; i++ ) data.samples[i] = 0;
  
    mraa_init();
  
    btn = mraa_gpio_init(6);
  
    if (btn == NULL) {
        return 1;
    }
  
    mraa_gpio_dir(btn, MRAA_GPIO_IN);
    mraa_gpio_isr(btn, MRAA_GPIO_EDGE_RISING, &btnInterrupt, NULL);
   
   
    audioMode = false;     // not in audio mode
   
    /* Set up the LCD */
    lcd = new upm::Jhd1313m1(0, 0x3E, 0x62);
    
   
    /* Intialize the port audio system */
    err = Pa_Initialize();
    if( err != paNoError ) goto done;

    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default input device.\n");
        goto done;
    }
    inputParameters.channelCount = 1;                    /* mono input */
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    /* Record some audio. -------------------------------------------- */
    err = Pa_OpenStream(
        &stream,
        &inputParameters,
        NULL,                  /* &outputParameters, */
        SAMPLING_RATE,
        FRAMES_PER_BUFFER,
        paClipOff,      /* we won't output out of range samples so don't bother clipping them */
        NULL, //recordCallback,
        NULL //&data 
        );
    if( err != paNoError ) goto done;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto done;
    printf("\nSystem up\n"); fflush(stdout);
  
    strcpy(aplay, APLAY);
    strcat(aplay, SOUNDS);
    strcat(aplay, "/");
  
    subLen = strlen(aplay);

    gbKeepGoing = true;
    /* register the signal handler */
    signal(SIGINT, signalHandler);
  
    while (1)
    {
        if (gbKeepGoing == false) break;
        
        if (audioMode == true)
        {
            if (isNoteValid(currentNote))
            {
                aplay[subLen] = '\0';
                strcat(aplay, getNoteFileName(currentNote));
                sleep(2);
                system(aplay);
            }
        }
        if (gCounter > 0 ) 
        {
           
            mraa_gpio_isr_exit(btn);
            sleep(1);         // debounce
            btnStatus = mraa_gpio_read(btn);
            if (btnStatus == 1)
            {
                while(mraa_gpio_read(btn) == 1);
                // long press, eneter audio mode 
                lcd->setCursor(0, 0);
                lcd->write("Audio mode  ");
                audioMode = true;
               
            }
            else
            {
                if (audioMode == true)
                {
                    audioMode = false;
                    lcd->setCursor(0, 0);
                    lcd->write("Target note: ");
                }
                else
                {
                    lcd->setCursor(0, 0);
                    lcd->write("Target note: ");
                    currentNote = cycleNote(currentNote);
                    lcd->setCursor(0, 13);
                    lcd->write(getNoteName(currentNote));
                }
            }
            gCounter = 0;
            /* Re-regitser the interrupt */
            mraa_gpio_isr(btn, MRAA_GPIO_EDGE_RISING, &btnInterrupt, NULL);
        }
        if (audioMode != true)
        {
            /* If not in audio mode, read the samples and do FFT and estimate the not */
            err = Pa_ReadStream(stream, data.samples, FRAMES_PER_BUFFER);
	    //printf ("Error: %d \n", err);
	    //printf ("\rError: %s \n", Pa_GetErrorText(err));
            if( err != paNoError ) continue;
	    //printf ("OK let's do our thing");
            iirDirect2(data.samples, y, FFT_LENGTH);
            void * fft = NULL;
            fft = initfft( 11 );
  

            for (i=0; i<FFT_LENGTH; i++)
            {
                idata[i] = 0;
            }
            
            /* Calculate FFT, followed by it's power spectrum */
            applyfft( fft, y, idata, false );
  
            int maxIndex = -1;
            float maxVal = -1;
            for( int j=0; j<FFT_LENGTH/2; ++j ) 
            {
                float v = sqrt(y[j] * y[j] + idata[j] * idata[j]) ;
                y[j] = sqrt(v);

                /* Look for the alpha freq */
                if( v > maxVal ) 
                {
                    maxVal = v;
                    maxIndex = j;
                }
            }
    
            float ex, low, high;
            if (maxIndex < 10)
            {
                /* Just a very bad hack to keep low freqency noise from messing up */
                note = NOTE_NOT_FOUND;
            }
            else
            {
                note = findNote(maxIndex, &ex, &low, &high);
            }
            printf ("\rmaxIndex: %d; note name %s  ", maxIndex, getNoteName(note));
            fflush(stdout);
            usleep(1000);

	    lcd->setCursor(1, 0);
            lcd->write("Est. note: ");
                    
            if (note != NOTE_NOT_FOUND)
            {
                lcd->setCursor(1, 11);
                lcd->write(getNoteName(note));
                if (note == currentNote)
                {
                    lcd->setColor(0, 0, 255);
                }
                else
                {
                    lcd->setColor(255, 0, 0);
                }

            }
            else
            {
                lcd->setColor(190, 50, 100);
                lcd->setCursor(1, 11);
                lcd->write("  ");
            }
            
        }

    }
    
    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto done;
   
  

done:
    /* Clear the LCD */
    lcd->clear();
    printf("Exiting...\n");
    
    if( err != paNoError )
    {
      fprintf( stderr, "An error occured while using the portaudio stream\n" );
      fprintf( stderr, "Error number: %d\n", err );
      fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
      err = 1;          /* Always return 0 or 1, but no other return codes. */
    }
  return err;
    Pa_Terminate();
    if( data.samples )       /* Sure it is NULL or valid. */
        free( data.samples );
    return err;
}
