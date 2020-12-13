## HOWTO:
This document describes how to configure and use the periperals of your board from Linux.

**Configure/Use the Leds**:
Configure the Leds GPIOs as outputs:
````
$echo 508 > /sys/class/gpio/export
$echo out > /sys/class/gpio/gpiochip508/direction
````
Set the Leds value:
````
$echo 0 > /sys/class/gpio/gpio508/value
$echo 1 > /sys/class/gpio/gpio508/value
````

**Configure/Use the PWM RGB Led**:
````
$ cd /sys/class/pwm/pwmchip0
$ echo 0 > export
$ cd pwm0
$ echo 100 > period
$ echo 50 > duty_cycle
$ echo 1 > enable
````

This should turn configure the Led with 50% PWM that you can adjust by changing `duty_cycle` value from `0` to the configured `period`.

**Configure/Use Ethernet**:
Verify that the `eth0` ethernet device is present:
`$ ifconfig -a`:
````
eth0      Link encap:Ethernet  HWaddr C6:6A:FB:04:6A:B9
          BROADCAST MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)

lo        Link encap:Local Loopback
          inet addr:127.0.0.1  Mask:255.0.0.0
          inet6 addr: ::1/128 Scope:Host
          UP LOOPBACK RUNNING  MTU:65536  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)

sit0      Link encap:IPv6-in-IPv4
          NOARP  MTU:1480  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)
````
Configure it:
`$ ifconfig eth0 192.168.1.50`

Verify that you can ping another machine on your network:
`$ ping 192.168.1.100`:
````
PING 192.168.1.100 (192.168.1.100): 56 data bytes
64 bytes from 192.168.1.100: seq=0 ttl=64 time=19.839 ms
64 bytes from 192.168.1.100: seq=1 ttl=64 time=4.585 ms
64 bytes from 192.168.1.100: seq=2 ttl=64 time=8.510 ms
64 bytes from 192.168.1.100: seq=3 ttl=64 time=12.522 ms
^C
--- 192.168.1.100 ping statistics ---
4 packets transmitted, 4 packets received, 0% packet loss
round-trip min/avg/max = 4.585/11.364/19.839 ms
````

**Configure/Use the SPI Flash:**
There should be a `/dev/mtd0` that you can read from/write to directly from bash, i.e.,:
```
$ cat /dev/mtd0
```
Or even better, to see the data clearly:

```
$ dd if=/dev/mtd0 count=6 bs=1 status=none | hexdump
```

Before writing you should erase the flash first. This requires `BR2_PACKAGE_MTD` and `BR2_PACKAGE_MTD_JFFS_UTILS` to be enabled in the buildroot config.

```
$ flash_erase /dev/mtd0 0 1
$ echo -ne "\x01\x01" > /dev/mtd0
```

**Configure/Use the SDCard:**
TODO

To put files (such as programs) on the board, put the files you want in the directory you want them in at
linux-on-litex-vexriscv/buildroot/board/litex_vexriscv/rootfs_overlay
Then go to the buildroot directory and run
make BR2_EXTERNAL=../linux-on-litex-vexriscv/buildroot/ litex_vexriscv_defconfig && make
Then copy "Image" and "rootfs.cpio" from buildroot/output/images to the SD card that plugs into the  board. Remember to umount the sd card. If things aren't working properly, try using "make.py --board=orangecrab --load" to reload the FPGA image, then try lxterm'ing in again.

To recompile the FPGA image, clone the https://github.com/SpinalHDL/VexRiscv.git repository, enter the root directory, and run 'sbt "runMain vexriscv.GenCoreDefault --externalInterruptArray=true --csrPluginConfig=linux-minimal"' to generate a file named "VexRiscv.v". Once this file is created, rename it to VexRiscv.v and VexRiscv.yaml to VexRiscv_Linux.v and VexRiscv_Linux.yaml, then copy these two files into pythondata-cpu-vexriscv/pythondata_cpu_vexriscv/verilog/.
Finally, go into linux-on-litex-vexriscv directory and run "./make.py --board=orangecrab --build" to build the new VexRiscv FPGA bitstream, then "./make.py --board=orangecrab --load" while connected to the board over USB (board must be in flash mode, do so by unplugging, holding button down, and plugging in while the button is held down) to load the FPGA bitstream onto the board.

To compile C programs for use on the board, run the riscv64-unknown-elf-gcc with the -march=rv32im -mabi=ilp32 flags enabled. The march flag only enables the RISCV I and M instructions, more letters need to be added here if you want support for more instruction features. Then, use the above information to copy the resultant file over to the orangecrab board.
