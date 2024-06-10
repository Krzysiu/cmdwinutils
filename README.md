Collection of very simple tools for helping in command line everyday's tasks.

Applications
===

beep.c
---
Usage: beep [frequency] [duration]

Generate a system beep sound using current sound card.

Parameters:
* `frequency` - frequency of the beep sound in Hz (default: 1000 - like profanity censor bleep in TV)
* `duration` - duration of the beep sound in milliseconds (default: 500)

Tips:
* For \"infinite\" (maximum possible - 1 193 hours) duration, set duration parameter to -1
* beep.exe plays sound synchronously (it lasts as long as the sound) to give an easy means to interrupt sound.
* To interrupt sound, kill process or press CTRL+C
Examples:
* `beep `- plays 1000 Hz beep for 500 ms
* `beep 70` - plays 70 Hz beep for 500 ms
* `beep 300 20000` - plays 300 Hz beep for 20 000 ms (2 seconds)
* `beep 1000` -1 - plays 1 kHz beep forever

log.c
---
Usage: `log.exe filename message`

Creates file foo.txt in temp directory **or appends it if exists** with contents (timestamp will be current date/time):

`[2024-06-10 15:48:01] bar`



Example:
* `log %TEMP%\\foo.txt bar`

quiet.c
---
Run batch script without showing console. (note: why? I don't remember, but I needed it for something)

Usage:
`quiet.exe <path>` where <path> is a path to batch file.

Example:
* `quiet.exe c:\\autoexec.bat && calc` - execute c:\autoexec.bat and if no error code will be returned, run calculator.

Quirks and things to remember:
1) EXIT CODE of batch script WILL BE FORWARDED.
2) This tool DOESN'T FORWARD PARAMETERS
3) USER PRIVLEDGES ARE INHERITED BY SCRIPT. So, if you run quiet.exe as admin, script will be ran as admin as well.
4) If batch script opens separate console window, it will show any ways, but you got tool to avoid that, don't you?
5) Use responsibly! You won't be getting console output (duh) of what's happening and only way to interrupt script is to kill the task.


recycler.c
---
Moves file/directory to recycle bin in Windows. No console output, except error. If you want to be sure file is deleted, you can use exit code (0 for success, 1 for any error). Unlike _MaDdoG Software Recycle.exe_ you don't have to use `-f` parameter (you can always undo it, anyways), doesn't support wildcards and most importantly doesn't create window, thus in loops **no focus stealing** with each iteration and your loop will be **way faster**.

Usage: 
* recycler <file_path>

sleep.c
---
Usage:
* 'sleep <TIME>'\nwhere <TIME> is delay in milliseconds (1/1000th of a second)

Example:
* `sleep 2000` - sleep for 2 seconds

Sleeps/stops executing synchronous script for given time. No more timeout for second precise timeouts or ping hacks.



Compiling
---
For all: `gcc %filename%.c -o %outname% -Os -s -ffunction-sections -fdata-sections -Wl,--gc-sections` (replace %% variables!)

For the recycler: `gcc recycler.c -o recycler -lole32 -lshell32 -municode`
