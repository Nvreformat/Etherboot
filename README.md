# Etherboot

Etherboot is an (experimental) bootloader that uses the ethernet port on the RPi B and above for uploading custom kernels at runtime. It uses the USPi library for the USB driver and some environment components.

It consists on 2 parts. One is the kernel which is executed on the Raspberry Pi, and the other is the Linux program which runs on your machine.
Once both parties are up and running, the program will upload your kernel to the RPi. Note that this project is still experimental and not all kernels might work. For example, it can't boot the Linux kernel.

See below for info about using this

Configuring and building
------------------------

Open the file etherbootdefs.h located in the root folder of the project. There you have to provide your computer's and your RPi's MAC addresses.
You can know your MAC address by typing ifconfig on a terminal and looking for a field called "HWaddr" in the network interface you want to use.
You can also change the address where your kernel will be placed in the memory.

Then open the Makefile located in kernel/ and change:
>RASPPI

With your RPi version, and

>PREFIX

With the prefix of your toolchain

Afther this, just run the makeall script in the root folder to compile both the kernel and the program.

If everything goes ok, you'll se two files called "kernel.img" and "etherboot" in the root directory.

Usage
------------------------

Turn on your Raspberry Pi with the Etherboot kernel (And wait until the ethernet leds turn on)

Run the following command as root (it needs root access because of the use of raw sockets):

`etherboot /path/to/your/kernel`

And the RPi will shortly load your kernel.

Note that the order you do these steps doesn't matter. You could, for example, run the command before turning the RPi on and it will just wait till it is on. 

Notes
-----
>*The maximum kernel size is approximately 5.5 MB

>*This has only been tested on the B+ model. So if anyone has got another model with ethernet port, please inform me if there are any issues with it.

>*Also, I only tested this on a wired connection. If you plan to use this over a wireless connection I can't guarantee you that it will work, but I believe it should. Also you'd need to change the interface name in etherbootdefs.h
