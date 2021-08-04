# UART_SDK
SDK for UART network with small MCUs and Linux/FreeBSD access points connected to cloud services

## UART Application System Overview

### Hardware Overview
IoT Gateway
    * intel64 PC
    * Linux or BSD
    * FTDI USB/UART converter
    * RS-485 driver
Edge Device
    * MCU
    * UART
    * RS-485 driver

### Software Overview
* Each device has a unique address
* Each packet has a recipient address and a sender address
* Each packet has a CRC.

## General Application Concept
### IoT Gateway
It is a usual PC running a distro of Linux or BSD; i.e. posix like OS.
Their CPU architecture should be intel64.

### Bus mutex
It controls that the RS-485 local bus must be occupied at the most two devices.
Usually an IoT gateway supports the function. The bus mutex always has address 0.

### IoT Edge Device
It is a MCU board equipped with sensors, actuators, heaters, etc.
All the devices must watch packets sent to the bus mutex to avoid
to send a packet disturging conversation between other devices.

## Usecases
### (Initiate - reply) synchrnonous operation
<br>

### (Initiate - trigger - SRQ - command - reply) operation
<br>

### (Initiate - repeat (sleep - SRQ - command - reply)) operation
<br>

## packet types
### reserved address
There are several reserved addresses.
address | definition
-------|-------
0x00 | 1-by-1 communication (only two paticipant in a network)
0xff | broadcast (all participants receives the packet)
<br>

### payloadless packet
asd[1] | asd[0]
-------|-------
0x80 (NOP: no operation) | none
0x81 (ACK: acknowledge) | result code
0x82 (BRQ : bus request, broadcast) | none
0x83 (BLK : bus locked, broadcast) | none
0x84 (BRL : bus release, broadcast) | none
0x85 (BULK : bus unlocked, broadcast) | none
0x86 (END: end loop) | none
0x87 (TRG: trigger) | device specific trigger number
0x88 (SRQ: service request) | device specific service request number
0x89 (GETD: get data) | none
0x8A (GETI: get item number) | device specific item number
0x8B (GETT: get device type ID) | device type ID
0x8C (GDT: general 1-byte data) | any value
0x8D (SETN) | set device number
0x8E (SETA) | set device address
0x8F (PING: Ping) | none
0x90 (QRY: device query) | query type [device address | device type | device number ]
<br>

### payload packet
location | definition
--|--
payload[0] | block ID
payload[1] | in-block sequence number
payload[2,3] | payload type
payload[4].. | payload data
payload[last -1, last] | CRC
<br>

## Usecases
### Plug & Play
1 A device is connected to bus
2 A device broadcast PING packet
3 The gateway sends ACK to the device
4 The device sends SRQ with asd[0]=Initialization request
5 The gateway sends QRY device type
6 The device reply device type
7 The gateway sends QRY device number
8 The device reply device number
9 The gateway sends properties to the device
10 The device reply ACK with result code [success | failure]

What device do:  
- PING  
- SRQ  
<br>
What device reply:  
- QRY device type
- QRY device number
- ACK to setup properties
