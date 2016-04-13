# ESP-Relay
Controlling relays with NodeMCU V1.0 (ESP8266-12E). Contains fritzing layout and Arduino IDE Code.

This PCB was planed for Sainsmart 4-Channel 5V Relay Module. It is alss possible to measure the current if ACS712 is connected to the prepared header.

## Schematic
![Schematic](https://cloud.githubusercontent.com/assets/4531344/14311570/649bde0a-fbe8-11e5-957a-ffbac4b89519.png)

## ACS712 Module
![ACS712](https://cloud.githubusercontent.com/assets/4531344/14313402/c4f3081a-fbf1-11e5-8ec6-98914111f634.jpg)

## SainSmart Relay
![Relay](https://cloud.githubusercontent.com/assets/4531344/14313482/33cf5554-fbf2-11e5-9ed3-54b02dc066e5.jpg)

## PCB Top
![Schematic](https://cloud.githubusercontent.com/assets/4531344/14311613/79bdba7e-fbe8-11e5-85e0-911eeb732946.png)

## PCB Bottom
![Schematic](https://cloud.githubusercontent.com/assets/4531344/14311601/70c4a734-fbe8-11e5-9041-8921783b6a1e.png)

Assembly List

| Label | Part Type | Properties
| ------| ----------| ----------
| ACS | Generic male header - 3 pins | pin spacing 0.1in (2.54mm); hole size 1.0mm,0.508mm; pins 3; form ♂ (male); row single; package THT
| Header1 | Generic male header - 15 | pins pin spacing 0.1in (2.54mm); hole size 1.0mm,0.508mm; pins 15; form ♂ (male); row single; package THT
| Header2 | Generic male header - 15 | pins pin spacing 0.1in (2.54mm); hole size 1.0mm,0.508mm; pins 15; form ♂ (male); row single; package THT
| NodeMCU V1.0 | NodeMCU V1.0 | chip ESP8266; variant NodeMCUV1
| OUT | Generic male header - 6 pins | pin spacing 0.1in (2.54mm); hole size 1.0mm,0.508mm; pins 6; form ♂ (male); row single; package THT
| PS  | Generic male header - 5 pins | pin spacing 0.1in (2.54mm); hole size 1.0mm,0.508mm; pins 5; form ♂ (male); row single; package THT
| Q1  | NPN-Transistor | type NPN (EBC); package TO92 [THT]
| Q2  | NPN-Transistor | type NPN (EBC); package TO92 [THT]
| Q3  | NPN-Transistor | type NPN (EBC); package TO92 [THT]
| Q4  | NPN-Transistor | type NPN (EBC); package TO92 [THT]
| R1  | 2.2kΩ Resistor | resistance 2.2kΩ; tolerance ±5%; package 2010 [SMD]
| R2  | 2.2kΩ Resistor | resistance 2.2kΩ; tolerance ±5%; package 2010 [SMD]
| R3  | 2.2kΩ Resistor | resistance 2.2kΩ; tolerance ±5%; package 2010 [SMD]
| R4  | 2.2kΩ Resistor | resistance 2.2kΩ; tolerance ±5%; package 2010 [SMD]
| R5  | Trimmer Potentiometer | track Linear; type Trimmer Potentiometer; size Trimmer - 12mm; package THT; maximum resistance 10kΩ


Shopping List

| Amount | Part Type | Properties
| ------ | --------- | ----------
| 1 | Generic male header - 3 pins |  pin spacing 0.1in (2.54mm); hole size 1.0mm,0.508mm; pins 3; form ♂ (male); row single; package THT
| 2 | Generic male header - 15 pins | pin spacing 0.1in (2.54mm); hole size 1.0mm,0.508mm; pins 15; form ♂ (male); row single; package THT
| 1 | NodeMCU V1.0  | chip ESP8266; variant NodeMCUV1
| 1 | Generic male header - 6 pins | pin spacing 0.1in (2.54mm); hole size 1.0mm,0.508mm; pins 6; form ♂ (male); row single; package THT
| 1 | Generic male header - 5 pins | pin spacing 0.1in (2.54mm); hole size 1.0mm,0.508mm; pins 5; form ♂ (male); row single; package THT
| 4 | NPN-Transistor | type NPN (EBC); package TO92 [THT]
| 4 | 2.2kΩ Resistor | resistance 2.2kΩ; tolerance ±5%; package 2010 [SMD]
| 1 | Trimmer Potentiometer | track Linear; type Trimmer Potentiometer; size Trimmer - 12mm; package THT; maximum resistance 10kΩ
