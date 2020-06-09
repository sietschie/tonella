all:
	cp version.tpp version.cpp
	sed -i s/BUILD_DATE/"`date`"/ version.cpp
	sed -i s/BUILD_VERSION/`git describe --tag`/ version.cpp
	arduino-cli compile  -b arduino:avr:nano .

upload:
	 arduino-cli upload -p /dev/ttyUSB0 -b arduino:avr:nano:cpu=atmega328old -t -v .
