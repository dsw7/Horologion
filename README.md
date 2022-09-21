# Horologion
Check back later.
## Table of Contents
- [Setup](#setup)
  - [Step 1 - Run install script](#step-1---run-install-script)
  - [Step 2 - Check that binary is ready](#step-2---check-that-binary-is-ready)
  - [Step 3 - Set wake-sleep window](#step-3---set-wake-sleep-window)
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
## Teardown
