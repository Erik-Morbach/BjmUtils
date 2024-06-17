#!/bin/bash
esptool.py --baud 500000 write_flash 0x0 $1
