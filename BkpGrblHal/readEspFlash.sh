#!/bin/bash
esptool.py --baud 500000 read_flash 0x0 0x400000 $1
