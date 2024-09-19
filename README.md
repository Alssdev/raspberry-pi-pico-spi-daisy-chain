# SPI Daisy-Chain with Raspberry Pi Pico
## Raspberry Pi Pico Scheme
<img width="870" alt="Screenshot 2024-09-18 at 6 34 41 PM" src="https://github.com/user-attachments/assets/68c267f5-275b-4317-9ede-f52025318042">

## Wiring
<img width="802" alt="Screenshot 2024-09-18 at 6 32 15 PM" src="https://github.com/user-attachments/assets/4d6ba18b-2249-4b42-9bfa-bfdc8dea9211">

## Compiling
To compile the code you can use the makefile included in this repo. Open a terminal in the project directory and run the following command:
```console
make
```
## Flash a Raspberry Pi Pico
You can flash your Pi Pico as the official [documentation](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf) shows. If you have
**picotool** installed, you can use the makefile included in this repo. To flash you pico using **picotool** you can run the following command:
```console
make flash
```
Remeber that the Pi Pico must by in flash mode, otherwise the above command will show an error like this:
```console
picotool load build/router.uf2
No accessible RP2040 devices in BOOTSEL mode were found.
make: *** [flash] Error 249
```
