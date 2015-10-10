# Introduction
Guitar tuner application that runs on the Edison and Galileo
This application turn your Galileo/Edison to a Guitar tuner.

# Setup
You will need an Edison or Galileo with USB audio card and
Grove base shiled with button and LCD modules
                                                   +--------+
                                                   |  Mic   |
    +----------------------+      +---------+   +--+        |
    |                      +------+  USB Snd+---+  +--------+
    |                      |      |  card   +---+
    |    Edison/Galileo    |      +---------+   |
    |                      |                    |  +--------+
    |                      +--------+           +--+ Speaker|
    +--+-------------------+        |              |        |
       |                            |              +--------+
       |                            |
  +----+-----+        +-------------+--------+
  | button   |        |     I2C RGB LCD      |
  |          |        |                      |
  +----------+        +----------------------+

# Operation
The user interface consists of a button and LCD.
Clicking the button selects the target note that you want to
tune.

It has two modes of operation
* Visual mode
* Audio mode

In visual mode, you select the target note by clicking the button.
The notes are cycled every button click.
In visual mode, the audio is recorded from mic and estimation is made
and closest frequncy is matched and the backlight chages to green
from red when the target note is matched.
Audio mode is entered by the long press of the button.
In audio mode, piano note as set in visual mode is played continously
until button is clicked to exit the audio mode. 

