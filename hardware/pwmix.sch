EESchema Schematic File Version 2
LIBS:power
LIBS:device
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
LIBS:ppmix-cache
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
L ATTINY85-S IC1
U 1 1 57B84D30
P 4850 1250
F 0 "IC1" H 3700 1650 50  0000 C CNN
F 1 "ATTINY85-S" H 5850 850 50  0000 C CNN
F 2 "SMD_Packages:SOIC-8-N" H 5800 1250 50  0000 C CIN
F 3 "" H 4850 1250 50  0000 C CNN
	1    4850 1250
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 57B84D8F
P 6300 1250
F 0 "C1" H 6325 1350 50  0000 L CNN
F 1 "1uF" H 6325 1150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 6338 1100 50  0001 C CNN
F 3 "" H 6300 1250 50  0000 C CNN
	1    6300 1250
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 57B84EC2
P 3250 1400
F 0 "R5" V 3330 1400 50  0000 C CNN
F 1 "1K" V 3250 1400 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3180 1400 50  0001 C CNN
F 3 "" H 3250 1400 50  0000 C CNN
	1    3250 1400
	0    1    1    0   
$EndComp
$Comp
L R R6
U 1 1 57B84F27
P 3250 1500
F 0 "R6" V 3330 1500 50  0000 C CNN
F 1 "10K" V 3250 1500 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3180 1500 50  0001 C CNN
F 3 "" H 3250 1500 50  0000 C CNN
	1    3250 1500
	0    1    1    0   
$EndComp
$Comp
L R R4
U 1 1 57B84F9A
P 3250 1300
F 0 "R4" V 3330 1300 50  0000 C CNN
F 1 "1K" V 3250 1300 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3180 1300 50  0001 C CNN
F 3 "" H 3250 1300 50  0000 C CNN
	1    3250 1300
	0    1    1    0   
$EndComp
$Comp
L R R3
U 1 1 57B84FD1
P 3250 1200
F 0 "R3" V 3330 1200 50  0000 C CNN
F 1 "1K" V 3250 1200 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3180 1200 50  0001 C CNN
F 3 "" H 3250 1200 50  0000 C CNN
	1    3250 1200
	0    1    1    0   
$EndComp
Wire Wire Line
	6200 1000 6300 1000
Wire Wire Line
	6300 900  6300 1100
Wire Wire Line
	6300 1400 6300 1600
Wire Wire Line
	6300 1500 6200 1500
$Comp
L GND #PWR01
U 1 1 57B852C4
P 6300 1600
F 0 "#PWR01" H 6300 1350 50  0001 C CNN
F 1 "GND" H 6300 1450 50  0000 C CNN
F 2 "" H 6300 1600 50  0000 C CNN
F 3 "" H 6300 1600 50  0000 C CNN
	1    6300 1600
	1    0    0    -1  
$EndComp
Connection ~ 6300 1500
Text GLabel 3000 1300 0    60   Input ~ 0
IN1
Text GLabel 3000 1400 0    60   Input ~ 0
IN2
Text GLabel 6300 900  1    60   Input ~ 0
VCC
Connection ~ 6300 1000
Text GLabel 3000 1500 0    60   Input ~ 0
VCC
Wire Wire Line
	3000 1500 3100 1500
Wire Wire Line
	3400 1500 3500 1500
Wire Wire Line
	3400 1400 3500 1400
Wire Wire Line
	3400 1300 3500 1300
Wire Wire Line
	3100 1400 3000 1400
Wire Wire Line
	3000 1300 3100 1300
Text GLabel 3000 1200 0    60   Input ~ 0
OUT2
Text GLabel 3000 1100 0    60   Input ~ 0
OUT1
$Comp
L R R2
U 1 1 57B85F0E
P 3250 1100
F 0 "R2" V 3330 1100 50  0000 C CNN
F 1 "1K" V 3250 1100 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3180 1100 50  0001 C CNN
F 3 "" H 3250 1100 50  0000 C CNN
	1    3250 1100
	0    1    1    0   
$EndComp
$Comp
L R R1
U 1 1 57B85F4D
P 3250 1000
F 0 "R1" V 3330 1000 50  0000 C CNN
F 1 "330" V 3250 1000 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3180 1000 50  0001 C CNN
F 3 "" H 3250 1000 50  0000 C CNN
	1    3250 1000
	0    1    1    0   
$EndComp
$Comp
L LED D1
U 1 1 57B85F88
P 2600 1300
F 0 "D1" H 2600 1400 50  0000 C CNN
F 1 "LED" H 2600 1200 50  0000 C CNN
F 2 "LEDs:LED_0805" H 2600 1300 50  0001 C CNN
F 3 "" H 2600 1300 50  0000 C CNN
	1    2600 1300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2600 1100 2600 1000
Wire Wire Line
	2600 1000 3100 1000
Wire Wire Line
	3000 1100 3100 1100
Wire Wire Line
	3000 1200 3100 1200
Wire Wire Line
	3400 1000 3500 1000
Wire Wire Line
	3400 1100 3500 1100
Wire Wire Line
	3400 1200 3500 1200
$Comp
L GND #PWR02
U 1 1 57B86177
P 2600 1600
F 0 "#PWR02" H 2600 1350 50  0001 C CNN
F 1 "GND" H 2600 1450 50  0000 C CNN
F 2 "" H 2600 1600 50  0000 C CNN
F 3 "" H 2600 1600 50  0000 C CNN
	1    2600 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 1500 2600 1600
$Comp
L CONN_01X03 P1
U 1 1 57B862DB
P 1000 1050
F 0 "P1" H 1000 1250 50  0000 C CNN
F 1 "CONN_01X03" V 1100 1050 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x03" H 1000 1050 50  0001 C CNN
F 3 "" H 1000 1050 50  0000 C CNN
	1    1000 1050
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X03 P2
U 1 1 57B86398
P 1000 1500
F 0 "P2" H 1000 1700 50  0000 C CNN
F 1 "CONN_01X03" V 1100 1500 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x03" H 1000 1500 50  0001 C CNN
F 3 "" H 1000 1500 50  0000 C CNN
	1    1000 1500
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X03 P3
U 1 1 57B8679F
P 1750 1050
F 0 "P3" H 1750 1250 50  0000 C CNN
F 1 "CONN_01X03" V 1850 1050 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x03" H 1750 1050 50  0001 C CNN
F 3 "" H 1750 1050 50  0000 C CNN
	1    1750 1050
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X03 P4
U 1 1 57B8680C
P 1750 1500
F 0 "P4" H 1750 1700 50  0000 C CNN
F 1 "CONN_01X03" V 1850 1500 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x03" H 1750 1500 50  0001 C CNN
F 3 "" H 1750 1500 50  0000 C CNN
	1    1750 1500
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR03
U 1 1 57B8694F
P 1300 1250
F 0 "#PWR03" H 1300 1000 50  0001 C CNN
F 1 "GND" H 1300 1100 50  0000 C CNN
F 2 "" H 1300 1250 50  0000 C CNN
F 3 "" H 1300 1250 50  0000 C CNN
	1    1300 1250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 57B8697D
P 1300 1700
F 0 "#PWR04" H 1300 1450 50  0001 C CNN
F 1 "GND" H 1300 1550 50  0000 C CNN
F 2 "" H 1300 1700 50  0000 C CNN
F 3 "" H 1300 1700 50  0000 C CNN
	1    1300 1700
	1    0    0    -1  
$EndComp
Text GLabel 1300 1500 2    60   Input ~ 0
VCC
Wire Wire Line
	1200 1600 1300 1600
Wire Wire Line
	1300 1600 1300 1700
Wire Wire Line
	1200 1500 1300 1500
Wire Wire Line
	1200 1150 1300 1150
Wire Wire Line
	1300 1150 1300 1250
Text GLabel 1300 1050 2    60   Input ~ 0
VCC
Wire Wire Line
	1200 1050 1300 1050
Text GLabel 1300 1400 2    60   Input ~ 0
IN1
Text GLabel 1300 950  2    60   Input ~ 0
IN2
Wire Wire Line
	1200 950  1300 950 
Wire Wire Line
	1200 1400 1300 1400
$Comp
L GND #PWR05
U 1 1 57B8711E
P 2050 1700
F 0 "#PWR05" H 2050 1450 50  0001 C CNN
F 1 "GND" H 2050 1550 50  0000 C CNN
F 2 "" H 2050 1700 50  0000 C CNN
F 3 "" H 2050 1700 50  0000 C CNN
	1    2050 1700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 57B8714C
P 2050 1250
F 0 "#PWR06" H 2050 1000 50  0001 C CNN
F 1 "GND" H 2050 1100 50  0000 C CNN
F 2 "" H 2050 1250 50  0000 C CNN
F 3 "" H 2050 1250 50  0000 C CNN
	1    2050 1250
	1    0    0    -1  
$EndComp
Text GLabel 2050 1500 2    60   Input ~ 0
VCC
Text GLabel 2050 1050 2    60   Input ~ 0
VCC
Text GLabel 2050 1400 2    60   Input ~ 0
OUT1
Text GLabel 2050 950  2    60   Input ~ 0
OUT2
Wire Wire Line
	1950 950  2050 950 
Wire Wire Line
	1950 1050 2050 1050
Wire Wire Line
	1950 1150 2050 1150
Wire Wire Line
	2050 1150 2050 1250
Wire Wire Line
	1950 1400 2050 1400
Wire Wire Line
	1950 1500 2050 1500
Wire Wire Line
	1950 1600 2050 1600
Wire Wire Line
	2050 1600 2050 1700
$EndSCHEMATC
