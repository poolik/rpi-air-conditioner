rpi-air-conditioner
===================

Companion app for my home automation project https://github.com/poolik/ingliste based on https://github.com/dnezic/rpi-air-conditioner

I've taken the base code, refactored, fixed a few bugs and changed some parts of the protocol that
by my reverse engineering proved to be different.

The unit I'm using this project with is MSZ-FH35VE
Remote type is SG13A 2184

#### Installation:
```
make
sudo make install
```

#### Usage:
```
sudo mitsu <temp> <on> <operation>
        <temp> = 16, ... 31
        <on> = 1=ON | 0=OFF
        <operation> = 0=HEATING | 1=COOLING
```

LED diode of IR transmitter is connected to GPIO 3 according to wiringPi library.
```
+----------+-Rev2-+------+--------+------+-------+
| wiringPi | GPIO | Phys | Name   | Mode | Value |
+----------+------+------+--------+------+-------+
|      3   |  22  |  15  | GPIO 3 | OUT  | Low   |
+----------+------+------+--------+------+-------+
```


