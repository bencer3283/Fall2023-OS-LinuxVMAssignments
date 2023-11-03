This tutorial is based on the one from https://www.eecs.umich.edu/courses/eecs570/discussions/w22/pthreads.html

This code has been tested with the TinyCore VM
By default the VM is not booted with a multicore CPU, for adding cores use the -smp option.

For example:
`qemu-system-x86_64 -m 2048 -vga virtio -usb -device usb-tablet -drive file=os_2023.qcow2 -smp 4`