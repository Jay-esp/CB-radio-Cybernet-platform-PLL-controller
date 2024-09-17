# CB-radio-Cybernet-platform-PLL-controller

This design allows a CB radio with 40 channels to be fitted with a rotary encoder and a 0.96" OLED display to browse through the channels, in channel mode or frequency mode, or scan the channels. It adds extra channels below and above the regular channels.
It is designed for typical 70'-80's Cybernet chassis radios, in my case a Super Panther.

Although the radio has 40 channels, the PLL used, the famous PLL02A, can handle 512 channels but the actual range of the radio itself is limited by the range of it's VCO (Voltage Controlled Oscillator). My usable range with the controller is -40 to +90 or 130 channels.
In the glory years of CB many had their 40 radio channels extended by adding switches for so called high, super-high, low and super-low channels.
In that period i was active in the CB scene and did a lot of modifications for fellow CBers.

Around 1981 i designed a scanner add-on for CB, i sold the design to a small CB shop and they built and sold them, it was very basic but gave the user 127 channels and the scanning feature.
I did do a new design a few years later with more features such as fast and slow forward / reverse buttons but that was for personal use. By the late 80's CB disappeared and i sold my radios.
For a while i wanted a cheap project radio to again add a scanner but with modern technology, and then a friend gave me this beat up Super Panther.

Disclaimer: additional to the standard license, no commercial usage is allowed, otherwise feel free to use the design and code and build your own version.
Provided as-is, no warranty or liability, use at your own risk. There is not really a risk voiding the warranty on your radio since this is intended for radios of the 70s and 80's. You might want to check if modifying the radio is within the legal bounds of your country's laws on CB radio. A decent level of knowledge on electronics and/or CB radios is assumed, do not attempt to build this if you do not have the proper skills otherwise you will get stuck soldering the port expander or even worse, blow up your radio. You can ask me for info but i can't provide details for use in other CB radios, I have no commercial interests, I do not sell kits.

That's the background for the design, it uses an ESP32-S3 mini zero, one of the smallest computer boards but very powerful.
Programming is done on the Arduino IDE platform, i use version 1.xx because it can work in portable mode, this allows the project to be "frozen" with the current drivers because drivers are updated all the time and breaking changes happen, i recommend using portable mode.

Source code is included.
Many font files are packed with it, not all are used but allow customization.
A lot is documented in the code comments.
An array is used to translate channels and frequencies, if you look at a frequency table for a CB radio you will notice a few deliberate gaps (Alpha channels) and some jumps (mixed up in frequency) and in case of negative channels, it's all reversed. I followed the sequences from a Ham International Jumbo 3.

The schematic and gerber files are included.
We are connecting 9 pins to the PLL for frequency selection plus a lock detect signal, further a connection to the squelch circuit.
The ESP does not have enough pins exposed therefor we use a port expander MCP23017 which is connected via the I2C interface bus.
The PLL of the radio works on 5V levels, the ESP32 on 3.3V, the port expander runs on 5V so it can connect directly to the PLL. The I2C port has level shifters on the SDA and SCL lines.
Since the ESP is in a socket we use the space below it to place components.
A transistor is placed in parallel to the transistor in the squelch circuit and has an open collector configuration to 3.3V so it can be coupled directly to the ESP.
The power supply to the OLED display is filtered by an CRC network.

The controller needs a 5V supply, it's not on the board, i used a 7805 regulator with decoupling caps, although not very efficient current is low and the noise level is lower than a buck converter. Make sure to insulate it from the chassis because that's a different ground.

Soldering the MCP is not for beginners, use plenty of flux and a special tip (hollowed out tip like Plato's SMD flow tip or Pace wave or mini wave tip).

Programming instructions are included, it will guide you through the installation of the development environment and to program the ESP32-S3 with the source code.

Mounting instructions are included for one type of Cybernet chassis, a very typical and widely used chassis.

A basic user guide is included.

I put a movie about this on Youtube: https://youtu.be/u1AFa6-rFkU

Also check out my other repository with the FM modulator/demodulator add-on.

