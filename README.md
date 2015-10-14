# Introduction
Guitar tuner application that runs on the Edison and Galileo
This application turn your Galileo/Edison to a Guitar tuner.

# Setup
You will need an Edison or Galileo with USB audio card and
Grove base shiled with button and LCD modules
                                               
Edison/Galileo---->USB sound card---->mic/speaker
   
# Compilation and usage
This application requires portaudio(dev version, installing from source
is good ooption). Hence you should install it before you compile 
this application.
In console, type 

make 

To run the application, type following

./tuner


# Operation
The user interface consists of a button(connected to D6) and LCD.
Clicking the button selects the target note that you want to
tune.

It has two modes of operation
* Visual mode
* Audio mode

In visual mode, you select the target note by clicking the button.
The notes are cycled every button click.
In visual mode, the audio is recorded from mic and estimation is made
and closest frequency is matched and the back light chages to green
from red when the target note is matched.
Audio mode is entered by the long press of the button.
In audio mode, piano note as set in visual mode is played continuously
until button is clicked to exit the audio mode. 

