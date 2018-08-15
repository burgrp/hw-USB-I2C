EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:device.farm
LIBS:MCU_ST_STM32F0
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L I2C-6P X1
U 1 1 5B72E107
P 3200 3800
F 0 "X1" H 3200 4300 60  0000 C CNN
F 1 "I2C-6P" H 3200 3450 60  0000 C CNN
F 2 "device.farm:Micro-Match-FOB-6" H 3350 3950 60  0001 C CNN
F 3 "" H 3350 3950 60  0001 C CNN
	1    3200 3800
	1    0    0    -1  
$EndComp
$Comp
L I2C X2
U 1 1 5B72E168
P 3200 4850
F 0 "X2" H 3200 5200 60  0000 C CNN
F 1 "I2C" H 3200 4500 60  0000 C CNN
F 2 "device.farm:Micro-Match-FOB-4" H 3350 5000 60  0001 C CNN
F 3 "" H 3350 5000 60  0001 C CNN
	1    3200 4850
	1    0    0    -1  
$EndComp
$Comp
L USB_OTG J1
U 1 1 5B72E2E3
P 3250 2500
F 0 "J1" H 3050 2950 50  0000 L CNN
F 1 "USB_OTG" H 3050 2850 50  0000 L CNN
F 2 "device.farm:conn_usb_B_micro_smd-2" H 3400 2450 50  0001 C CNN
F 3 "" H 3400 2450 50  0001 C CNN
	1    3250 2500
	1    0    0    -1  
$EndComp
$Comp
L SPX3819 U1
U 1 1 5B72E336
P 5000 2350
F 0 "U1" H 5000 2800 60  0000 C CNN
F 1 "SPX3819" H 5000 2700 60  0000 C CNN
F 2 "TO_SOT_Packages_SMD:TSOT-23-5" H 5000 2350 60  0001 C CNN
F 3 "" H 5000 2350 60  0001 C CNN
	1    5000 2350
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 5B72E37D
P 7350 3400
F 0 "C3" H 7375 3500 50  0000 L CNN
F 1 "100n" H 7375 3300 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 7388 3250 50  0001 C CNN
F 3 "" H 7350 3400 50  0001 C CNN
	1    7350 3400
	1    0    0    -1  
$EndComp
$Comp
L SWD X3
U 1 1 5B72E3F4
P 3200 5700
F 0 "X3" H 3200 5950 60  0000 C CNN
F 1 "SWD" H 3200 5450 60  0000 C CNN
F 2 "device.farm:SWD" H 3200 5700 60  0001 C CNN
F 3 "" H 3200 5700 60  0000 C CNN
	1    3200 5700
	1    0    0    -1  
$EndComp
Text Label 3550 5600 0    60   ~ 0
SWDIO
Text Label 3550 5800 0    60   ~ 0
SWCLK
$Comp
L +5V #PWR01
U 1 1 5B72E51C
P 3550 2300
F 0 "#PWR01" H 3550 2150 50  0001 C CNN
F 1 "+5V" H 3550 2440 50  0000 C CNN
F 2 "" H 3550 2300 50  0001 C CNN
F 3 "" H 3550 2300 50  0001 C CNN
	1    3550 2300
	0    1    1    0   
$EndComp
$Comp
L +5V #PWR02
U 1 1 5B72E53E
P 4450 2200
F 0 "#PWR02" H 4450 2050 50  0001 C CNN
F 1 "+5V" H 4450 2340 50  0000 C CNN
F 2 "" H 4450 2200 50  0001 C CNN
F 3 "" H 4450 2200 50  0001 C CNN
	1    4450 2200
	0    -1   -1   0   
$EndComp
$Comp
L +3V3 #PWR03
U 1 1 5B72E589
P 5750 2200
F 0 "#PWR03" H 5750 2050 50  0001 C CNN
F 1 "+3V3" H 5750 2340 50  0000 C CNN
F 2 "" H 5750 2200 50  0001 C CNN
F 3 "" H 5750 2200 50  0001 C CNN
	1    5750 2200
	0    1    1    0   
$EndComp
$Comp
L +3V3 #PWR04
U 1 1 5B72E5E5
P 3550 3600
F 0 "#PWR04" H 3550 3450 50  0001 C CNN
F 1 "+3V3" V 3550 3800 50  0000 C CNN
F 2 "" H 3550 3600 50  0001 C CNN
F 3 "" H 3550 3600 50  0001 C CNN
	1    3550 3600
	0    1    1    0   
$EndComp
$Comp
L +3V3 #PWR05
U 1 1 5B72E605
P 3550 4650
F 0 "#PWR05" H 3550 4500 50  0001 C CNN
F 1 "+3V3" V 3550 4850 50  0000 C CNN
F 2 "" H 3550 4650 50  0001 C CNN
F 3 "" H 3550 4650 50  0001 C CNN
	1    3550 4650
	0    1    1    0   
$EndComp
$Comp
L +5V #PWR06
U 1 1 5B72E625
P 3550 3500
F 0 "#PWR06" H 3550 3350 50  0001 C CNN
F 1 "+5V" V 3550 3700 50  0000 C CNN
F 2 "" H 3550 3500 50  0001 C CNN
F 3 "" H 3550 3500 50  0001 C CNN
	1    3550 3500
	0    1    1    0   
$EndComp
$Comp
L GND #PWR07
U 1 1 5B72E640
P 3500 2900
F 0 "#PWR07" H 3500 2650 50  0001 C CNN
F 1 "GND" V 3500 2700 50  0000 C CNN
F 2 "" H 3500 2900 50  0001 C CNN
F 3 "" H 3500 2900 50  0001 C CNN
	1    3500 2900
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR08
U 1 1 5B72E662
P 6000 5400
F 0 "#PWR08" H 6000 5150 50  0001 C CNN
F 1 "GND" V 6000 5200 50  0000 C CNN
F 2 "" H 6000 5400 50  0001 C CNN
F 3 "" H 6000 5400 50  0001 C CNN
	1    6000 5400
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR09
U 1 1 5B72E6D6
P 5900 3500
F 0 "#PWR09" H 5900 3350 50  0001 C CNN
F 1 "+3V3" V 5900 3700 50  0000 C CNN
F 2 "" H 5900 3500 50  0001 C CNN
F 3 "" H 5900 3500 50  0001 C CNN
	1    5900 3500
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR010
U 1 1 5B72E6F8
P 6000 3500
F 0 "#PWR010" H 6000 3350 50  0001 C CNN
F 1 "+3V3" V 6000 3700 50  0000 C CNN
F 2 "" H 6000 3500 50  0001 C CNN
F 3 "" H 6000 3500 50  0001 C CNN
	1    6000 3500
	1    0    0    -1  
$EndComp
Text Label 3550 3750 0    60   ~ 0
SDA
Text Label 3550 3850 0    60   ~ 0
SCL
$Comp
L R R4
U 1 1 5B72E79A
P 7050 4700
F 0 "R4" V 7000 4850 50  0000 C CNN
F 1 "4k7" V 7050 4700 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 6980 4700 50  0001 C CNN
F 3 "" H 7050 4700 50  0001 C CNN
	1    7050 4700
	0    1    1    0   
$EndComp
$Comp
L R R3
U 1 1 5B72E818
P 7050 4600
F 0 "R3" V 7000 4750 50  0000 C CNN
F 1 "4k7" V 7050 4600 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 6980 4600 50  0001 C CNN
F 3 "" H 7050 4600 50  0001 C CNN
	1    7050 4600
	0    1    1    0   
$EndComp
$Comp
L STM32F042K4Tx U2
U 1 1 5B72EAB3
P 6000 4400
F 0 "U2" H 5600 5250 50  0000 L CNN
F 1 "STM32F042K4Tx" H 6200 5250 50  0000 L CNN
F 2 "Housings_QFP:LQFP-32_7x7mm_Pitch0.8mm" H 5600 3500 50  0001 R CNN
F 3 "" H 6000 4400 50  0001 C CNN
	1    6000 4400
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR011
U 1 1 5B72ECE9
P 6100 3500
F 0 "#PWR011" H 6100 3350 50  0001 C CNN
F 1 "+3V3" V 6100 3700 50  0000 C CNN
F 2 "" H 6100 3500 50  0001 C CNN
F 3 "" H 6100 3500 50  0001 C CNN
	1    6100 3500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 5B72ED6A
P 5900 5400
F 0 "#PWR012" H 5900 5150 50  0001 C CNN
F 1 "GND" V 5900 5200 50  0000 C CNN
F 2 "" H 5900 5400 50  0001 C CNN
F 3 "" H 5900 5400 50  0001 C CNN
	1    5900 5400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR013
U 1 1 5B72EE97
P 7600 3650
F 0 "#PWR013" H 7600 3400 50  0001 C CNN
F 1 "GND" H 7600 3500 50  0000 C CNN
F 2 "" H 7600 3650 50  0001 C CNN
F 3 "" H 7600 3650 50  0001 C CNN
	1    7600 3650
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR014
U 1 1 5B72EEBD
P 7600 3150
F 0 "#PWR014" H 7600 3000 50  0001 C CNN
F 1 "+3V3" H 7600 3290 50  0000 C CNN
F 2 "" H 7600 3150 50  0001 C CNN
F 3 "" H 7600 3150 50  0001 C CNN
	1    7600 3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 5B72EEE3
P 3550 5700
F 0 "#PWR015" H 3550 5450 50  0001 C CNN
F 1 "GND" V 3600 5500 50  0000 C CNN
F 2 "" H 3550 5700 50  0001 C CNN
F 3 "" H 3550 5700 50  0001 C CNN
	1    3550 5700
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR016
U 1 1 5B72EFD5
P 5000 2800
F 0 "#PWR016" H 5000 2550 50  0001 C CNN
F 1 "GND" H 5000 2650 50  0000 C CNN
F 2 "" H 5000 2800 50  0001 C CNN
F 3 "" H 5000 2800 50  0001 C CNN
	1    5000 2800
	1    0    0    -1  
$EndComp
Text Label 3550 2500 0    60   ~ 0
USB_DP
Text Label 3550 2600 0    60   ~ 0
USB_DM
Text Label 3550 3400 0    60   ~ 0
ALERT
$Comp
L GND #PWR017
U 1 1 5B72F169
P 3550 4000
F 0 "#PWR017" H 3550 3750 50  0001 C CNN
F 1 "GND" V 3550 3800 50  0000 C CNN
F 2 "" H 3550 4000 50  0001 C CNN
F 3 "" H 3550 4000 50  0001 C CNN
	1    3550 4000
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR018
U 1 1 5B72F18F
P 3550 5050
F 0 "#PWR018" H 3550 4800 50  0001 C CNN
F 1 "GND" V 3550 4850 50  0000 C CNN
F 2 "" H 3550 5050 50  0001 C CNN
F 3 "" H 3550 5050 50  0001 C CNN
	1    3550 5050
	0    -1   -1   0   
$EndComp
Text Label 3550 4800 0    60   ~ 0
SDA
Text Label 3550 4900 0    60   ~ 0
SCL
$Comp
L GND #PWR019
U 1 1 5B730927
P 5500 5200
F 0 "#PWR019" H 5500 4950 50  0001 C CNN
F 1 "GND" V 5500 5000 50  0000 C CNN
F 2 "" H 5500 5200 50  0001 C CNN
F 3 "" H 5500 5200 50  0001 C CNN
	1    5500 5200
	0    1    1    0   
$EndComp
$Comp
L C C1
U 1 1 5B730B31
P 5350 3700
F 0 "C1" H 5375 3800 50  0000 L CNN
F 1 "100n" H 5375 3600 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 5388 3550 50  0001 C CNN
F 3 "" H 5350 3700 50  0001 C CNN
	1    5350 3700
	0    1    1    0   
$EndComp
$Comp
L GND #PWR020
U 1 1 5B730BDB
P 5200 3700
F 0 "#PWR020" H 5200 3450 50  0001 C CNN
F 1 "GND" H 5200 3550 50  0000 C CNN
F 2 "" H 5200 3700 50  0001 C CNN
F 3 "" H 5200 3700 50  0001 C CNN
	1    5200 3700
	0    1    1    0   
$EndComp
$Comp
L LED D1
U 1 1 5B730BFC
P 4750 5100
F 0 "D1" H 4750 5200 50  0000 C CNN
F 1 "LED" H 4750 5000 50  0000 C CNN
F 2 "LEDs:LED_0805" H 4750 5100 50  0001 C CNN
F 3 "" H 4750 5100 50  0001 C CNN
	1    4750 5100
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 5B730C88
P 5050 5100
F 0 "R1" V 5130 5100 50  0000 C CNN
F 1 "270R" V 5050 5100 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 4980 5100 50  0001 C CNN
F 3 "" H 5050 5100 50  0001 C CNN
	1    5050 5100
	0    1    1    0   
$EndComp
$Comp
L GND #PWR021
U 1 1 5B730E45
P 4600 5100
F 0 "#PWR021" H 4600 4850 50  0001 C CNN
F 1 "GND" V 4600 4900 50  0000 C CNN
F 2 "" H 4600 5100 50  0001 C CNN
F 3 "" H 4600 5100 50  0001 C CNN
	1    4600 5100
	0    1    1    0   
$EndComp
$Comp
L +3V3 #PWR022
U 1 1 5B730FF7
P 7200 4600
F 0 "#PWR022" H 7200 4450 50  0001 C CNN
F 1 "+3V3" V 7200 4800 50  0000 C CNN
F 2 "" H 7200 4600 50  0001 C CNN
F 3 "" H 7200 4600 50  0001 C CNN
	1    7200 4600
	0    1    1    0   
$EndComp
$Comp
L +3V3 #PWR023
U 1 1 5B731026
P 7200 4700
F 0 "#PWR023" H 7200 4550 50  0001 C CNN
F 1 "+3V3" V 7200 4900 50  0000 C CNN
F 2 "" H 7200 4700 50  0001 C CNN
F 3 "" H 7200 4700 50  0001 C CNN
	1    7200 4700
	0    1    1    0   
$EndComp
Text Label 6500 4800 0    60   ~ 0
USB_DM
Text Label 6500 4900 0    60   ~ 0
USB_DP
Text Label 6500 4700 0    60   ~ 0
SDA
Text Label 6500 4600 0    60   ~ 0
SCL
Text Label 6500 4500 0    60   ~ 0
ALERT
$Comp
L R R2
U 1 1 5B73138D
P 7050 4500
F 0 "R2" V 7000 4650 50  0000 C CNN
F 1 "4k7" V 7050 4500 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 6980 4500 50  0001 C CNN
F 3 "" H 7050 4500 50  0001 C CNN
	1    7050 4500
	0    1    1    0   
$EndComp
$Comp
L +3V3 #PWR024
U 1 1 5B731447
P 7200 4500
F 0 "#PWR024" H 7200 4350 50  0001 C CNN
F 1 "+3V3" V 7200 4700 50  0000 C CNN
F 2 "" H 7200 4500 50  0001 C CNN
F 3 "" H 7200 4500 50  0001 C CNN
	1    7200 4500
	0    1    1    0   
$EndComp
$Comp
L C C2
U 1 1 5B731D86
P 5600 2450
F 0 "C2" H 5625 2550 50  0000 L CNN
F 1 "1u" H 5625 2350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 5638 2300 50  0001 C CNN
F 3 "" H 5600 2450 50  0001 C CNN
	1    5600 2450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR025
U 1 1 5B731EA2
P 5600 2600
F 0 "#PWR025" H 5600 2350 50  0001 C CNN
F 1 "GND" H 5600 2450 50  0000 C CNN
F 2 "" H 5600 2600 50  0001 C CNN
F 3 "" H 5600 2600 50  0001 C CNN
	1    5600 2600
	1    0    0    -1  
$EndComp
NoConn ~ 5450 2500
NoConn ~ 5500 4200
NoConn ~ 5500 4300
NoConn ~ 5500 4500
NoConn ~ 5500 4600
NoConn ~ 5500 4700
NoConn ~ 5500 4800
NoConn ~ 5500 4900
NoConn ~ 5500 5000
NoConn ~ 6500 3700
NoConn ~ 6500 3800
NoConn ~ 6500 3900
NoConn ~ 6500 4000
NoConn ~ 6500 4100
NoConn ~ 6500 4200
NoConn ~ 6500 4300
NoConn ~ 6500 4400
NoConn ~ 6500 5200
$Comp
L PWR_FLAG #FLG026
U 1 1 5B732D47
P 6900 2300
F 0 "#FLG026" H 6900 2375 50  0001 C CNN
F 1 "PWR_FLAG" V 6900 2600 50  0000 C CNN
F 2 "" H 6900 2300 50  0001 C CNN
F 3 "" H 6900 2300 50  0001 C CNN
	1    6900 2300
	0    1    1    0   
$EndComp
$Comp
L PWR_FLAG #FLG027
U 1 1 5B732D77
P 6900 2400
F 0 "#FLG027" H 6900 2475 50  0001 C CNN
F 1 "PWR_FLAG" V 6900 2700 50  0000 C CNN
F 2 "" H 6900 2400 50  0001 C CNN
F 3 "" H 6900 2400 50  0001 C CNN
	1    6900 2400
	0    1    1    0   
$EndComp
$Comp
L +3V3 #PWR028
U 1 1 5B733016
P 6900 2400
F 0 "#PWR028" H 6900 2250 50  0001 C CNN
F 1 "+3V3" V 6900 2600 50  0000 C CNN
F 2 "" H 6900 2400 50  0001 C CNN
F 3 "" H 6900 2400 50  0001 C CNN
	1    6900 2400
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR029
U 1 1 5B733046
P 6900 2300
F 0 "#PWR029" H 6900 2150 50  0001 C CNN
F 1 "+5V" V 6900 2500 50  0000 C CNN
F 2 "" H 6900 2300 50  0001 C CNN
F 3 "" H 6900 2300 50  0001 C CNN
	1    6900 2300
	0    -1   -1   0   
$EndComp
NoConn ~ 3550 2700
Text Label 6500 5000 0    60   ~ 0
SWDIO
Text Label 6500 5100 0    60   ~ 0
SWCLK
$Comp
L GND #PWR030
U 1 1 5B732FE6
P 6900 2500
F 0 "#PWR030" H 6900 2250 50  0001 C CNN
F 1 "GND" V 6900 2300 50  0000 C CNN
F 2 "" H 6900 2500 50  0001 C CNN
F 3 "" H 6900 2500 50  0001 C CNN
	1    6900 2500
	0    1    1    0   
$EndComp
$Comp
L PWR_FLAG #FLG031
U 1 1 5B732DA7
P 6900 2500
F 0 "#FLG031" H 6900 2575 50  0001 C CNN
F 1 "PWR_FLAG" V 6900 2800 50  0000 C CNN
F 2 "" H 6900 2500 50  0001 C CNN
F 3 "" H 6900 2500 50  0001 C CNN
	1    6900 2500
	0    1    1    0   
$EndComp
$Comp
L C C4
U 1 1 5B73420C
P 7600 3400
F 0 "C4" H 7625 3500 50  0000 L CNN
F 1 "100n" H 7625 3300 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 7638 3250 50  0001 C CNN
F 3 "" H 7600 3400 50  0001 C CNN
	1    7600 3400
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 5B734255
P 7850 3400
F 0 "C5" H 7875 3500 50  0000 L CNN
F 1 "100n" H 7875 3300 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 7888 3250 50  0001 C CNN
F 3 "" H 7850 3400 50  0001 C CNN
	1    7850 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 5100 5500 5100
Wire Wire Line
	6500 4600 6900 4600
Wire Wire Line
	6500 4700 6900 4700
Wire Wire Line
	6500 4500 6900 4500
Wire Wire Line
	5450 2200 5750 2200
Wire Wire Line
	5600 2300 5600 2200
Connection ~ 5600 2200
Wire Wire Line
	4450 2200 4550 2200
Wire Wire Line
	4500 2200 4500 2500
Wire Wire Line
	4500 2500 4550 2500
Connection ~ 4500 2200
Wire Wire Line
	3150 2900 3500 2900
Connection ~ 3250 2900
Wire Wire Line
	7350 3550 7350 3600
Wire Wire Line
	7350 3600 7850 3600
Wire Wire Line
	7850 3600 7850 3550
Wire Wire Line
	7600 3550 7600 3650
Connection ~ 7600 3600
Wire Wire Line
	7600 3150 7600 3250
Wire Wire Line
	7350 3250 7350 3200
Wire Wire Line
	7350 3200 7850 3200
Connection ~ 7600 3200
Wire Wire Line
	7850 3200 7850 3250
$Comp
L Conn_01x06 J2
U 1 1 5B734769
P 3200 6650
F 0 "J2" H 3200 6950 50  0000 C CNN
F 1 "Conn_01x06" H 3200 6250 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06_Pitch2.54mm" H 3200 6650 50  0001 C CNN
F 3 "" H 3200 6650 50  0001 C CNN
	1    3200 6650
	-1   0    0    1   
$EndComp
$Comp
L +3V3 #PWR032
U 1 1 5B734A70
P 3400 6550
F 0 "#PWR032" H 3400 6400 50  0001 C CNN
F 1 "+3V3" V 3400 6750 50  0000 C CNN
F 2 "" H 3400 6550 50  0001 C CNN
F 3 "" H 3400 6550 50  0001 C CNN
	1    3400 6550
	0    1    1    0   
$EndComp
$Comp
L +5V #PWR033
U 1 1 5B734C73
P 3400 6450
F 0 "#PWR033" H 3400 6300 50  0001 C CNN
F 1 "+5V" V 3400 6650 50  0000 C CNN
F 2 "" H 3400 6450 50  0001 C CNN
F 3 "" H 3400 6450 50  0001 C CNN
	1    3400 6450
	0    1    1    0   
$EndComp
Text Label 3400 6350 0    60   ~ 0
ALERT
Text Label 3400 6650 0    60   ~ 0
SDA
Text Label 3400 6750 0    60   ~ 0
SCL
$Comp
L GND #PWR034
U 1 1 5B734EB5
P 3400 6850
F 0 "#PWR034" H 3400 6600 50  0001 C CNN
F 1 "GND" V 3400 6650 50  0000 C CNN
F 2 "" H 3400 6850 50  0001 C CNN
F 3 "" H 3400 6850 50  0001 C CNN
	1    3400 6850
	0    -1   -1   0   
$EndComp
$EndSCHEMATC