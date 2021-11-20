EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Sign Buddy"
Date "2021-11-15"
Rev "v01"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 "Author: Stephen Scott"
$EndDescr
$Comp
L MCU_ST_STM32L0:STM32L053C8Tx U?
U 1 1 619401EF
P 8950 3300
F 0 "U?" H 8950 5081 50  0000 C CNN
F 1 "STM32L053C8Tx" H 8950 4990 50  0000 C CNN
F 2 "Package_QFP:LQFP-48_7x7mm_P0.5mm" H 8450 1800 50  0001 R CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00105960.pdf" H 8950 3300 50  0001 C CNN
	1    8950 3300
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_ST_STDC14 J?
U 1 1 6194300C
P 1750 6100
F 0 "J?" H 1307 6146 50  0000 R CNN
F 1 "Conn_ST_STDC14" H 1307 6055 50  0000 R CNN
F 2 "" H 1750 6100 50  0001 C CNN
F 3 "https://www.st.com/content/ccc/resource/technical/document/user_manual/group1/99/49/91/b6/b2/3a/46/e5/DM00526767/files/DM00526767.pdf/jcr:content/translations/en.DM00526767.pdf" V 1400 4850 50  0001 C CNN
	1    1750 6100
	1    0    0    -1  
$EndComp
$Comp
L Sensor_Motion:BNO055 U?
U 1 1 6198335B
P 5550 2350
F 0 "U?" H 5550 3231 50  0000 C CNN
F 1 "BNO055" H 5550 3140 50  0000 C CNN
F 2 "Package_LGA:LGA-28_5.2x3.8mm_P0.5mm" H 5800 1700 50  0001 L CNN
F 3 "https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST_BNO055_DS000_14.pdf" H 5550 2550 50  0001 C CNN
	1    5550 2350
	1    0    0    -1  
$EndComp
$Comp
L MCU_Nordic:nRF51x22-QFxx U?
U 1 1 61984589
P 1750 2600
F 0 "U?" H 1750 711 50  0000 C CNN
F 1 "nRF51x22-QFxx" H 1750 620 50  0000 C CNN
F 2 "Package_DFN_QFN:QFN-48-1EP_6x6mm_P0.4mm_EP4.6x4.6mm" H 1750 2600 50  0001 C CNN
F 3 "http://infocenter.nordicsemi.com/pdf/nRF51822_PS_v3.3.pdf" H 1350 2500 50  0001 C CNN
	1    1750 2600
	1    0    0    -1  
$EndComp
$EndSCHEMATC
