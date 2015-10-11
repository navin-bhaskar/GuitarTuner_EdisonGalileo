
#include "config.h"
#include "note.h"
#include <stdio.h>

/* Frequencies of the notes in Hz */

static float frequencies[] = {
  /*                C     C#/Db     D     D#/Eb     E       F     F#/Gb     G     G#/Ab     A     A#/Bb     B   */
  /* 1 */         32.70,  34.65,  36.71,  38.89,  41.20,  43.65,  46.25,  49.00,  51.91,  55.00,  58.27,  61.74,
  /* 2 */         65.41,  69.30,  73.42,  77.78,  82.41,  87.31,  92.50,  98.00,  103.8,  110.0,  116.5,  123.5,
  /* 3 */         130.8,  138.6,  146.8,  155.6,  164.8,  174.6,  185.0,  196.0,  207.7,  220.0,  233.1,  246.9,
  /* Middle C */  261.6,  277.2,  293.7,  311.1,  329.6,  349.2,  370.0,  392.0,  415.3,  440.0,  466.2,  493.9,
  /* 5 */         523.3,  554.4,  587.3,  622.3,  659.3,  698.5,  740.0,  784.0,  830.6,  880.0,  932.3,  987.8,
  /* 6 */         1047,   1109,   1175,   1245,   1319,   1397,   1480,   1568,   1661,   1760,   1865,   1976,
  /* 7 */         2093,   2217,   2349,   2489,   2637,   2794,   2960,   3136,   3322,   3520,   3729,   3951,
  /* 8 */         4186,   4435,   4699,   4978,   5274,   5588,   5920,   6272,   6645,   7040,   7459,   7902,
};

/** The tolerence grade */
static float tolGrade[] = {0.5f, 1.0f, 2.0f, 3.0f, 8.0f, 20.0f, 40.0f, 100.0f, 180.0f}; 

/** The one octave note table */
char note[][2+1] = {"C ",  "C#",  "D ",  "D#",  "E ",  "F ",  "F#",  "G ",  "G#",  "A ",  "A#",  "B "}; 

/** Not note */
char emptyStr[] = "  ";

/** To note file lookup table */
char noteFile[][16+1] = 
{
    "Piano.pp.C4.wav",
    "Piano.pp.Db4.wav",
    "Piano.pp.D4.wav",
    "Piano.pp.Eb4.wav",
    "Piano.pp.E4.wav",
    "Piano.pp.F4.wav",
    "Piano.pp.Gb4.wav",
    "Piano.pp.G4.wav",
    "Piano.pp.Ab4.wav",
    "Piano.pp.A4.wav",
    "Piano.pp.Bb4.wav",
    "Piano.pp.B4.wav"
}; 


/**
 * Does a linear serach for nearest macthing frequency 
 */

int findNote(
    int index,               /**< Index in the FFT where the fundamental frequency is */
    float * exact,           /**< Measured frequency */
    float * low,             /**< Nearest lower frequency */
    float * high             /**< Nearest higher frequency */
    )
{
    int n, i;
    float estimatedFreq;
    int note;
    
    estimatedFreq = 1.0*index*SAMPLING_RATE/FFT_LENGTH;
    
    n = sizeof(frequencies)/sizeof(float);
    
    for (i=1; i<n; i++)
    {
        /* 
           This algorithm for finding notes was taken from here:
           http://www.stm32circle.com/projects/project.php?id=81
        */
        *exact = frequencies[i];
        *low = (frequencies[i] + frequencies[i-1])/2;
        *high = (frequencies[i] + frequencies[i+1])/2;
        if(estimatedFreq > *low && estimatedFreq < *high)
        {
            break;
        }
    }
    
    if (i == n)
    {
        // note not found
        note = NOTE_NOT_FOUND;
    }
    else
    {
        note = i % 12;
    }
         
    
    return note;
    
}
    
int getNoteFreq(int noteIdx)
{
    if (noteIdx < 0 || noteIdx > 12)
    {
        return -1;
    }
    
    return (int)(frequencies[12*3 + noteIdx]);
}


/**
 * Returns the note name given the NOTE enum
 */

char* getNoteName(
    int noteNum                  /**< The note */
    )
{
    
    if (noteNum < 0 || noteNum > 12)
    {
        return emptyStr;
    }
    else
    {
        return note[noteNum];
    }
}

char * getNoteFileName(
    int noteNum                /** The note */
    )
{
     if (noteNum < 0 || noteNum > 12)
    {
        return emptyStr;
    }
    else
    {
        return noteFile[noteNum];
    }
}




        
      

