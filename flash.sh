#!/usr/bin/kermit

# Inspired from http://blackfin.uclinux.org/doku.php?id=bootloaders:u-boot:serial_port_loading_files
echo "Prepared to boot new kernel, finger Crossed!"

input 60 "Hit any key to stop autoboot"
# If your board wants you to press a different key to get to
# U-Boot, edit this line.
output " "
input 5 "U-boot>"
# Here, 0x800000 is the memory address into which the kernel
# should be loaded.
lineout "loadb 0x8000"
# This should be the absolute path to your kernel uImage file.
send kernel.img
input 5 "U-boot>"
#lineout "bootm 0x8000"
lineout "go 0x8000"

# This command drops you into a console where you can interact
# with the kernel.
connect
