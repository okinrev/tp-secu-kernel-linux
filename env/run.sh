#!/bin/sh
qemu-system-x86_64 \
    -m 128M \
    -cpu kvm64 \
    -kernel bzImage \
    -initrd initramfs.cpio.gz \
    -snapshot \
    -nographic \
    -monitor /dev/null \
    -no-reboot \
    -append "console=ttyS0 nosmep nosmap nopti nokaslr quiet panic=1" \
    -s
