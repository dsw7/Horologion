# Horologion
An easy-to-use program for waking a Linux machine at a specific time of day, running some commands, then
suspending the machine some time later. The program achieves this by dynamically modifying the host's real
time clock (RTC).
> Why not just use `systemd`?

I like `systemd`, but I felt `systemd` was a bit bloated for this task.
## Table of Contents
- [How it works](#how-it-works)
- [Setup](#setup)
  - [Step 1 - Run install script](#step-1---run-install-script)
  - [Step 2 - Set custom configurations](#step-2---set-custom-configurations)
- [Teardown](#teardown)
## How it works
To start, let's provide some definitions:
| Variable | Description |
| ---- | ---- |
| $t_w$ | System wake up time |
| $t_c$ | Command run time |
| $t_s$ | System suspend time |
| $u$ | Difference between $t_w$ and $t_c$ |
| $v$ | Difference between $t_c$ and $t_s$ |

The following conditions must hold:

$$
t_w < t_c < t_s
$$

And,

$$
t_c - t_w \geq u
$$

And,

$$
t_s - t_c \geq v
$$

The system also assumes that the interval bounded by $t_w$ and $t_s$ is bounded within the same calendar day.
Given the above conditions are met, the program will follow:

| Day | Time   | Action |
| --- | ------ | ------ |
| $1$ | $t_{w1}$ | System wakes up |
| $1$ | $t_{c1}$ | `horolog` process runs user defined commands |
| $1$ | $t_{c1}$ | `horolog` process sets alarm for $t_{w1} + 86400\ s$, or $t_{w2}$ |
| $1$ | $t_{s1}$ | `horolog` process sends ACPI suspend signal to machine |
| $2$ | $t_{w2}$ | System wakes up |
| $2$ | $t_{c2}$ | `horolog` process runs user defined commands |
| $2$ | $t_{c2}$ | `horolog` process sets alarm for $t_{w2} + 86400\ s$, or $t_{w3}$ |
| $2$ | $t_{s2}$ | `horolog` process sends ACPI suspend signal to machine |
| ... | ... | ... |
| $n$ | $t_{wn}$ | System wakes up |
| $n$ | $t_{cn}$ | `horolog` process runs user defined commands |
| $n$ | $t_{cn}$ | `horolog` process sets alarm for $t_{wn} + 86400\ s$, or $t_{wn + 1}$ |
| $n$ | $t_{sn}$ | `horolog` process sends ACPI suspend signal to machine |

## Setup
### Step 1 - Run install script
Make the script executable:
```
chmod +x Horologion/install.sh
```
Then run the script with elevated privileges:
```
sudo ./Horologion/install.sh
```
The following menu will appear:
```
Select setup type:
[1] -> Install product
[2] -> Uninstall product
>
```
Type 1 then press enter. The install script will compile a binary and position all requisite files.
### Step 2 - Set custom configurations
Open the software's configuration file:
```
sudo vi /etc/horolog.ini
```
#### Set critical times
Start by setting the critical times. The critical times must adhere to the conditions set out in the [Setup](#setup)
section. A mapping follows:

| Variable | Field in `ini` file |
| ---- | ---- |
| $t_w$ | `time-wake-{hour,minute}` |
| $t_c$ | `time-cmd-{hour,minute}` |
| $t_s$ | `time-sleep-{hour,minute}` |

```ini
# ----------------------------------------
# When the machine should wake up
# ----------------------------------------

# Specify hour [0-23]
time-wake-hour = 8

# Specify minute [0-59]
time-wake-minute = 0

# ----------------------------------------
# When the machine should run commands
# ----------------------------------------

# Specify hour [0-23]
time-cmd-hour = 8

# Specify minute [0-59]
time-cmd-minute = 1

# ----------------------------------------
# When the machine should sleep
# ----------------------------------------

# Specify hour [0-23]
time-sleep-hour = 8

# Specify minute [0-59]
time-sleep-minute = 2
```
The above defaults will wake the machine at 8:00 AM, run all commands at 8:01 AM, and suspend the system at
8:02 AM. All times are interpreted in the system's time zone setting.

#### Set suspend types
The suspend type will dictate the overall power savings when the machine is suspended.
```ini
# Select one of:

# none    -> Do nothing (useful for troubleshooting)
# freeze  -> Suspend-to-Idle     ACPI state S0
# standby -> Power-On Suspend    ACPI state S1
# mem     -> Suspend-to-RAM      ACPI state S3
# disk    -> Suspend-to-Disk     ACPI state S4
...
suspend-type = mem
```
ACPI state S4 (`disk`) provides the greatest power savings at the expense of the longest wake latency.

#### Choose commands to run
Locate the targets section in `horolog.ini` and choose some targets:
```ini
# Specify targets following target_1 ... target_n

target_1 = sleep 1
target_2 = sleep 2
```
The above two commands will be run at time $t_c$ and will run concurrently.

## Teardown
To uninstall the product, make the script executable:
```
chmod +x Horologion/install.sh
```
Then run the script with elevated privileges:
```
sudo ./Horologion/install.sh
```
The following menu will appear:
```
Select setup type:
[1] -> Install product
[2] -> Uninstall product
>
```
Type 2 then press enter. The install script will uninstall all relevant files.
