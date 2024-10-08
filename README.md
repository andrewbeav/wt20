# Walkie Talkie 2.0

## What is it?
The goal of this project is to create customized toys for my nieces and nephews which they can use to send short-range voice messages to each other, with optional audio effects!

## Design
Overall design is ever changing, but currently the plan is:
- ESP32C6 MCU
- ESP IDF
- ESPNOW protocol
  - however, high level software should be designed so it is protocol agnostic!
- WM8960 Audo Codec

## Design Principles
1. When reasonable, all developer-written code (i.e. not FreeRTOS or IDF code) shall be unit tested off target
2. Except in cases of a clear and **necessary** performance gain, all code should be as modular as possible

## Project Status
- Currently on stage 1. Current plan is:
1. Proof of concept with dev boards
  - POC should be able to send voice message from one system to another, with optional audio effects. UI does not need to be in place (UART commands from PC in place of buttons/screens is okay)
2. UI Design
3. Hardware design
4. Final software design
5. Enclosure
