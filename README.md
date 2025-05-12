# Horologion
An easy-to-use program for waking a Linux machine at a specific time of day, running some commands, then
suspending the machine some time later. The program achieves this by dynamically modifying the host's real
time clock (RTC).

> Why not just use `systemd-sleep` or perhaps some `cron` / `rtcwake` combination?

These solutions are recommended. However, I wanted a more user-friendly "self-contained" solution.
## Table of Contents
- [How it works](#how-it-works)
- [Setup](#setup)
  - [Step 1 - Run install script](#step-1---run-install-script)
  - [Step 2 - Set custom configurations](#step-2---set-custom-configurations)
  - [Step 3 - Start service](#step-3---start-service)
  - [Step 4 - Suspend the system](#step-4---suspend-the-system)
- [Teardown](#teardown)
- [Logging](#logging)
- [Disabling the software](#disabling-the-software)
- [Visualizing what happens per cycle](#visualizing-what-happens-per-cycle)

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
```bash
chmod +x install
```
Then run the script:
```bash
./install
```
The following menu will appear:
```
Select setup type:
[1] -> Install product
[2] -> Uninstall product
>
```
Type 1 then press enter. The install script will prompt for the user's password for `sudo`.
### Step 2 - Set custom configurations
Open the software's configuration file:
```bash
vi $HOME/.horolog/horolog.toml
```
#### Set critical times
Start by setting the critical times. The critical times must adhere to the conditions set out in the
[How it works](#how-it-works) section. A mapping follows:

| Variable | Field in `toml` file |
| ---- | ---- |
| $t_w$ | `[times.wake] -> hour + minute` |
| $t_c$ | `[times.cmd] -> hour + minute` |
| $t_s$ | `[times.sleep] -> hour + minute` |

```toml
[times.wake]
# When the machine should wake up

# Specify hour [0-23]
hour = 8

# Specify minute [0-59]
minute = 0

[times.cmd]
# When the machine should run commands

# Specify hour [0-23]
hour = 8

# Specify minute [0-59]
minute = 1

[times.sleep]
# When the machine should sleep

# Specify hour [0-23]
hour = 8

# Specify minute [0-59]
minute = 2
```
The above defaults will wake the machine at 8:00 AM, run all commands at 8:01 AM, and suspend the system at
8:02 AM. All times are interpreted in the system's time zone setting.

#### Set suspend types
The suspend type will dictate the overall power savings when the machine is suspended.
```toml
# Select one of:

# none    -> Do nothing (useful for troubleshooting)
# standby -> Power-On Suspend    ACPI state S1
# mem     -> Suspend-to-RAM      ACPI state S3
# disk    -> Suspend-to-Disk     ACPI state S4
# ...
suspend-type = "mem"
```
ACPI state S4 (`disk`) provides the greatest power savings at the expense of the longest wake latency.

#### Choose commands to run
Locate the targets section in `horolog.toml` and choose some targets:
```toml
targets = [
  "sleep 1",
  "sleep 2",
]
```
The above two commands will be run at time $t_c$ and will run concurrently.

### Step 3 - Start service
Start the service:
```bash
sudo systemctl start horolog
```
Then check the service's status:
```bash
sudo systemctl status horolog
```

### Step 4 - Suspend the system
When work on the system is complete, suspend the system using `systemd`:
```bash
sudo systemctl suspend
```
The system should then wake at the next scheduled time $t_w$ and respect the configurations specified in
`horolog.toml`. **NOTE:** A power down (i.e. via `shutdown`) will result in a system that does not wake at the
next time $t_w$.

## Teardown
To uninstall the product, make the install script executable:
```
chmod +x install
```
Then run the script:
```
./install
```
The following menu will appear:
```
Select setup type:
[1] -> Install product
[2] -> Uninstall product
>
```
Type 2 then press enter. The install script will uninstall all relevant files and the `horolog` service. Note
that this will require the user's `sudo` password.

## Logging
Logging is handled by `journald`. The view the logs produced by `horolog`, run:
```bash
sudo journalctl -u horolog -f
```

## Disabling the software
To disable the software, simply stop the `horolog` service:
```bash
sudo systemctl stop horolog
```
This will send a SIGTERM to the `horolog` process which will reset the `sysfs` wakealarm file. Inspecting the
logs using:
```bash
sudo journalctl -u horolog -f
```
Should reveal:
```bash
systemd[1]: Stopping Horolog Job Scheduling Software...
horolog[21086]: YYYY-MM-DDTHH:mm:ss 21086 I Received signal 15
horolog[21086]: YYYY-MM-DDTHH:mm:ss 21086 I Ending loop
horolog[21086]: YYYY-MM-DDTHH:mm:ss 21086 I Unsetting alarm
horolog[21086]: YYYY-MM-DDTHH:mm:ss 21086 I Will write "0" to /sys/class/rtc/rtc0/wakealarm
```
To prevent the software from restarting when the machine is rebooted, run:
```bash
sudo systemctl disable horolog
```

## Visualizing what happens per cycle
To visualize when the next cycle will take place and what will happen during the cycle, run:
```bash
horolog plan
```
This command will read `horolog.toml` and return a "schematic" representation:
```
[Plan]:

 ┌─{t_w}: Sat Sep DD 08:00:00 YYYY
 │
 │
 ├─{t_c}: Sat Sep DD 08:01:00 YYYY -> [Commands]
 │
 │
 └─{t_s}: Sat Sep DD 08:02:00 YYYY

[Commands]: ─┬─ sleep 1
             ├─ sleep 2
             └─ sleep 3
```
The following mapping applies here:
| Branch from `plan` command | Variable |
| ---- | ---- |
| `{t_w}` | $t_w$ |
| `{t_c}` | $t_c$ |
| `{t_s}` | $t_s$ |

This output can be interpreted as: "At time `t_c`, the commands under the `[Commands]` tree will be executed."
