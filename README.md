# Horologion
Check back later.

```math
\sqrt{3}
```

## Table of Contents
- [Setup](#setup)
  - [Step 1 - Run install script](#step-1---run-install-script)
  - [Step 2 - Check that binary is ready](#step-2---check-that-binary-is-ready)
  - [Step 3 - Set wake-sleep window](#step-3---set-wake-sleep-window)
  - [Step 4 - Select the suspend type](#step-4---select-the-suspend-type)
  - [Step 5 - Choose commands to run](#step-5---choose-commands-to-run)
  - [Step 6 - Enable the trigger](#step-6---enable-the-trigger)
- [Teardown](#teardown)

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
### Step 2 - Check that binary is ready
Run the following:
```
horolog --help
```
Which should print a help menu.
### Step 3 - Set wake-sleep window
Open the software's configuration file:
```
sudo vi /etc/horolog.ini
```
And edit the following fields:
```ini
# ----------------------------------------
# When the machine should wake up
# ----------------------------------------

# Specify hour [0-23]
time-wake-hour = 8

# Specify minute [0-59]
time-wake-minute = 0

# ----------------------------------------
# When the machine should sleep
# ----------------------------------------

# Specify hour [0-23]
time-sleep-hour = 8

# Specify minute [0-59]
time-sleep-minute = 5
```
The above defaults will wake the machine at 8:00 AM and suspend the system at 8:05 AM. All times are
interpreted in the system's time zone setting.
### Step 4 - Select the suspend type
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
### Step 5 - Choose commands to run
Locate the targets section in `horolog.ini` and choose some targets:
```ini
# Specify targets following target_1 ... target_n

target_1 = sleep 1
target_2 = sleep 2
```
The above two commands will be run 1 minute after the scheduled wake time (i.e. 8:01 AM). Both commands will
be spawned as subprocesses.
### Step 6 - Enable the trigger
To enable the system, run:
```
sudo horolog enable
```
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
