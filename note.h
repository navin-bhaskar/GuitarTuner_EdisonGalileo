

#ifndef __NOTE_H
#define __NOTE_H


enum note {NOTE_NOT_FOUND=-1, NOTE_C, NOTE_CS, NOTE_D, NOTE_DS, NOTE_E, 
    NOTE_F, NOTE_FS, NOTE_G, NOTE_GS, NOTE_A, NOTE_AS, NOTE_B};
    
    
int findNote(
    int index,               /**< Index in the FFT where the fundamental frequency is */
    float * exact,           /**< Measured frequency */
    float * low,             /**< Nearest lower frequency */
    float * high             /**< Nearest higher frequency */
    );

char* getNoteName(
    int noteNum                  /**< The note */
    );

int getNoteFreq(int );

char * getNoteFileName(
    int noteNum                /** The note */
    );

inline int cycleNote(int note)
{
    note++;
    return (note % 12);
}

inline int isNoteValid(int note)
{
    if (note >= 0 && note < 12)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

#endif
