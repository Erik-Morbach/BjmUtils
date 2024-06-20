#!/bin/bash
sudo apt -y update
sudo apt -y upgrade
python3 -m pip install gpiozero matplotlib mttkinter smbus2 --break
sudo apt-get -y install python3-pil.imagetk libminizip1 libegl1-mesa xbase-clients
home=$HOME
cd $home
git clone https://github.com/Erik-Morbach/bCNC.git
cd Desktop
cd $home

mkdir utils
echo 'from gpiozero import LED
import time
bootPin = 4
resetPin = 17
rst = LED(resetPin)
bot = LED(bootPin)
bot.on()
time.sleep(1)
rst.off()
time.sleep(1)
rst.on()
' > $home/utils/resetEsp.py

echo "@reboot python3 $home/utils/resetEsp.py" > tmpFile
crontab -l -u $USER | cat - tmpFile | crontab -u $USER -
rm tmpFile

cd $home
echo "Z+ 114 Right
Z- 113 Left
X- 111 Up
X+ 116 Down
B+ 112 Prior
B- 117 Next" > bCNC/jogConf.txt

cp bCNC/cnc.desktop Desktop/BjmCncInterface.desktop

echo "
enable_uart=1
init_uart_baud=500000
max_usb_current=1
hdmi_force_hotplug=1
config_hdmi_boost=7
hdmi_group=2
hdmi_mode=87
hdmi_cvt=1024 600 60 6 0 0 0" | sudo tee -a /boot/firmware/config.txt

