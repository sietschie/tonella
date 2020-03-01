---
title: Figurines
---

# Figurines

! This section shall describe how to print a figure, write the rfid chip and create the sd card contents.

## Programming the chip

Process for writing ids to the nfc tags:

* unscrew box
* connect via usb to the arduino
* manually change value set to buffer[0] to the id that shall be written
* flash programm to arduino
* put token on rfid reader to write id
* flash tonella software back to arduino
