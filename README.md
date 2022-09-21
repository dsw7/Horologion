# Horologion
Check back later.
## Table of Contents
- [Setup](#setup)
  - [Step 1 - Run install script](#step-1---run-install-script)
  - [Step 2 - Check that binary is ready](#step-2---check-that-binary-is-ready)
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
## Teardown
