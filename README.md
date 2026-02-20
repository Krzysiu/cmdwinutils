Collection of very simple tools for helping in command line everyday's tasks. Despite the name of the pack, **they work in non-Windows OS** - it's just that they replace missing Linux commands in Windows, so it's most useful there.


Changes
===

### Icons legend:
* :beetle: bug fix
* :hatching_chick: new application
* :unamused: minor change
* :arrow_up: major change
---

[release in progress]
---
* :hatching_chick: **pausex** - a nicer brother of Windows's `pause`

2026 release (tag r2)
---
* :hatching_chick: **quietps**, **phppause** - New applications added to the collection.
* :arrow_up: **sleep** (v0.2.0) - **Breaking change**: lost backward compatibility with v0.0.1.
* :arrow_up: **sleep** (v0.2.0) - completely rewritten, added GNU sleep compatibility.
* :arrow_up: **quiet** (v0.1.0) - Now forwards parameters and exit codes correctly.
* :unamused: **sleep** (v0.2.0) - Removed useless debug messages.
* :unamused: Global code unification and style cleanup across all tools.

2024, initial Release (tag 0.0.1)
---
* :hatching_chick: Initial version including **beep**, **log**, **quiet**, **recycler**, and **sleep**.

Manual
===


**beep.c**
---
Usage: `beep [frequency] [duration]`

Generate a system beep sound using the current sound card.

Parameters:
* `frequency` - frequency of the beep sound in Hz (default: 1000 - like a profanity censor bleep in TV).
* `duration` - duration of the beep sound in milliseconds (default: 500).

Tips:
* For "infinite" (maximum possible - 1,193 hours) duration, set the duration parameter to `-1`.
* **beep.exe** plays sound synchronously to provide an easy way to interrupt the sound.
* To interrupt the sound, kill the process or press `CTRL+C`.

Examples:
* `beep` - plays 1000 Hz beep for 500 ms.
* `beep 1000 1200` - plays 1000 Hz beep (TV "bleep") for 1200 ms. Now you can swear!

**log.c**
---
Usage: `log.exe filename message`

Creates a file in the temp directory (or appends to it if it exists) with the following content structure (timestamp will be current date/time):

`[2024-06-10 15:48:01] message_content`

Example:
* `log %TEMP%\foo.txt "action completed"`

:arrow_up: **quiet.c**
---
Runs a batch script without showing the console window.

Usage: `quiet.exe <path>` where `<path>` is the path to the `.bat` or `.cmd` file.

Example:
* `quiet.exe c:\autoexec.bat && calc` - executes c:\autoexec.bat without showing console window and, if no error occurs, launches the calculator.

Quirks and things to remember:
1) **Exit code** of the batch script will be forwarded.
2) User privileges are inherited by the script. If you run **quiet.exe** as admin, script will be ran as admin as well.
3) If the batch script opens a separate console window, it will still show up.
4) Use responsibly! You won't see any console output. The only way to interrupt script is to kill the task.

**recycler.c**
---
Moves files or directories to the Windows Recycle Bin. It produces no console output except on error. 

Unlike other similar tools, you don't need a `-f` parameter, it doesn't support dangerous wildcards, and most importantly, **it does not create a window**. This prevents focus-stealing during loops and makes execution significantly faster.

Usage: 
* `recycler <file_path>`

:arrow_up: **sleep.c**
---
Stops executing a synchronous script for a given time. No more "ping hacks" or millisecond-based confusion.

Usage: `sleep <TIME>[SUFFIX]` 
Where `<TIME>` is a number (integer or double) and `[SUFFIX]` is one of: `s` (seconds, default), `m` (minutes), `h` (hours), `d` (days).

Examples:
* `sleep 2` - sleep for 2 seconds (GNU style).
* `sleep 1.5m` - sleep for 1 minute and 30 seconds.

### *v0.0.1*
* Initial version.

### *Current: v0.2.0*
* :arrow_up: Completely rewritten.
* :arrow_up: New parameters and GNU sleep compatibility.
* :unamused: Lost backward compatibility with v0.0.1.
* :unamused: Removed useless debug messages.

:hatching_chick: **phppause.c** 
---
Usage: `phppause <SCRIPT.php> [arguments]`

A transparent wrapper for the PHP interpreter designed primarily for debugging. It automatically sets the console title to the script name, enables ANSI color support, and **keeps the window open** after the script finishes or crashes. 

This eliminates the need to manually add `pause` at the end of every script or create messy `.bat` wrappers. It displays the script path, the full command line used, the output, and the final exit code.

Example of a result when a script contains a parse error:
 ![Example of wrapper in action: displaying parse error, while pure php.exe would just quit](https://github.com/user-attachments/assets/f549e734-d079-4fb7-9723-7a106810b9e0)
* **Customization:** While designed for PHP (using `php` from PATH by default), the source code can be easily recompiled for other interpreters like Python.
* **PHP Path:** You can modify the `phpPath` variable in the source code (line 54) to point to a specific executable.

:hatching_chick: **quietps.c** 
---
Usage: `quietps.exe "<powershell_command>"`

Runs PowerShell commands invisibly (no window) and captures all output (stdout/stderr) to a log file. It automatically handles `ExecutionPolicy Bypass` and uses `-NoProfile` for faster execution.

* **Why not just `quiet powershell.exe`?** Standard window-hiding tools often fail to capture PowerShell output or might fail to execute scripts correctly without a pipe. **quietps.exe** uses dedicated pipe redirection to ensure PowerShell initializes and executes reliably in the background.
* **Logging:** By default, it logs to `%TEMP%\quietps.log.txt`. 
* **Exit Code:** The exit code from the PowerShell process is forwarded as the exit code of **quietps.exe**.

:hatching_chick: **pausex.c** 
---
Usage: `pausex`

A simple replacement for the system `pause` command that differentiates between a regular key press and **CTRL+C**, while preventing accidental skipping of steps.

Key differences from standard `pause`:
* **Distinct ^C handling**. Unlike the standard `pause`, which treats CTRL+C just like any other key or kills the batch flow, `pausex` specifically identifies the interrupt and returns **Exit Code 130**. This allows the caller to handle the termination gracefully.
* **Input debouncing**. If you're holding a key from a previous task, `pausex` waits for you to release it before listening for a new press. No more accidental fast-forwarding through tasks.
* **Ignores modifiers**. Pressing Shift, Ctrl, Alt, or the Windows key by itself won't finish the execution

**Python Example:**
```python
import subprocess, sys
res = subprocess.run(['pausex.exe']).returncode
if res == 130: sys.exit("Interrupted by user.")
```

**PHP Example:**
```php
exec('pausex', $out, $res);
if ($res === 130) die("Interrupted by user.");
```

Compiling
===
For **all**: `gcc %filename%.c -o %outname% -Os -s -ffunction-sections -fdata-sections -Wl,--gc-sections` (replace %% variables!)

Specific tools:
* **quietps**: `gcc quietps.c -o quietps.exe -mwindows -Os -s -ffunction-sections -fdata-sections -Wl,--gc-sections`
* **quiet**: `gcc quiet.c -o quiet.exe -mwindows -Os -s -ffunction-sections -fdata-sections -Wl,--gc-sections`
* **recycler**: `gcc recycler.c -o recycler -lole32 -lshell32 -municode`

**For phppause (with icon):**
1. `windres phppause.rc -o phppause_res.o`
2. `gcc phppause.c phppause_res.o -o phppause -Os -s -ffunction-sections -fdata-sections -Wl,--gc-sections -lshell32`

Support me
===
If you find these tools useful and want to support my work, feel free to buy me a coffee!

[!["Buy Me A Coffee"](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://www.buymeacoffee.com/krzysiunet)
