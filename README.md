**Custom firmware for rain and light sensor of M32T/M36T platform**
> Codename *"Golden Rain"*

!!!ALARMA!!!  
**BEFORE ANY MANIPULATIONS WITH YOUR RLS MAKE SURE THAT YOU HAVE SAVED IT FACTORY FIRMWARE (BYTES 0x000003FE-0x000003FF CONTAINS INDIVIDUAL CALIBRATION VALUES AND MUST BE COPIED TO MAIN.C SCFTRIM AND SCTRIM)**

 - Toolchain: IAR ARM 9.30
 - IDE: Visual Studio Code with IAR extension
 - Debugger: J-Link via SWD
 - Code: Govnocode with pieces of disassembled factory firmware
 - LIN Stack: 4.6.6 (C) Freescale Semiconductor, Inc. 2008-2016
 - LIN DB: Reverse engineered from RLS and BCM firmwares.
