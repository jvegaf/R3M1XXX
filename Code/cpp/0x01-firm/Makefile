# Makefile for PlatformIO project

# Default target
all:
	pio run

# Upload firmware
upload:
	pio run -t upload

# Clean build files
clean:
	pio run -t clean

# Monitor serial output
monitor:
	pio device monitor

# Run tests
test:
	pio test

# Update platformio packages
update:
	pio update

# Build and upload
deploy: all upload

.PHONY: all upload clean monitor test update deploy
