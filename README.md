# winOS

My attempt to replicate a vintage Windows-like OS following the Bare Bones OSDev wiki.

### Installation & Structure
Tested on QEMU and uses GRUB as boot.
We locate prebuilt binaries for i686-elf-gcc and use wget for installation.

## Building & Running winOS
Run the build command:
```bash
./build.sh
```
This will build libc, create libk.a, compile the kernel, link everything together and create the bootable kernel binary. 

We can boot directly into the kernel using:
```bash
qemu-system-i386 -kernel sysroot/boot/winOS.kernel
```
or we can boot more realistically using GRUB:
```bash
# Create bootable ISO
mkdir -p isodir/boot/grub
cp sysroot/boot/myos.kernel isodir/boot/

cat > isodir/boot/grub/grub.cfg << 'EOF'
menuentry "winOS" {
    multiboot /boot/winOS.kernel
}
EOF

grub-mkrescue -o myos.iso isodir
qemu-system-i386 -cdrom winOS.iso
```