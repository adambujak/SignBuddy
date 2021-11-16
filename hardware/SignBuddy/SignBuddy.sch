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
L Connector:USB_B_Micro J?
U 1 1 61941FFB
P 1600 2550
F 0 "J?" H 1657 3017 50  0000 C CNN
F 1 "USB_B_Micro" H 1657 2926 50  0000 C CNN
F 2 "" H 1750 2500 50  0001 C CNN
F 3 "~" H 1750 2500 50  0001 C CNN
	1    1600 2550
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_ST_STDC14 J?
U 1 1 6194300C
P 1600 4350
F 0 "J?" H 1157 4396 50  0000 R CNN
F 1 "Conn_ST_STDC14" H 1157 4305 50  0000 R CNN
F 2 "" H 1600 4350 50  0001 C CNN
F 3 "https://www.st.com/content/ccc/resource/technical/document/user_manual/group1/99/49/91/b6/b2/3a/46/e5/DM00526767/files/DM00526767.pdf/jcr:content/translations/en.DM00526767.pdf" V 1250 3100 50  0001 C CNN
	1    1600 4350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
