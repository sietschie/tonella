all:
	arduino-cli compile  -b arduino:avr:nano .

upload:
	arduino-cli upload -p /dev/ttyUSB0 -b arduino:avr:nano:cpu=atmega328old -t -v .

setup:
	arduino-cli core install arduino:avr
	arduino-cli lib install DFRobotDFPlayerMini "Adafruit NeoPixel" MFRC522


