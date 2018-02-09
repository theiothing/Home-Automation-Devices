# Home-Automation-Devices
Various usefull/useless IoT devices connected to NodeRed and Home Assistant using Arduino IDE and ESP8266 and MQTT for communication over WiFi.

![](main_framework.png)

## Scope of the Repository
The scope of this repository is to provide a series of ready-made schetches for variety of sensors and actuators (in general devices) that can easily communicate via MQTT using ESP8266 chip family boards. Initally developed for connecting to a generic MQTT platform, the sensors works fine together with home automation framework like Home Assistant, Domoticz, OpenHAB, NODE-RED and everything that can easily communicate with an MQTT broker.
By the way these devices can be used in many different ways and connected to whatever platform is preferred.

## Getting Started
### Install ESP8266 CORE
These sketches are developed inside the Arduino development enviroment (Arduino IDE) for the ESP8266 bords. Information about how to install the core for such boards are available [here](https://arduino-esp8266.readthedocs.io/en/2.4.0/) or you can visit this repository:

  [esp8266 core](https://github.com/esp8266/Arduino)

### Get an MQTT broker
MQTT (aka MQ Telemetry Transport) is a machine-to-machine or “Internet of Things” connectivity protocol on top of TCP/IP. It allows extremely lightweight publish/subscribe messaging transport.
In order to make your device communicating you need to connect to an MQTT broker. For testing purposes you can use an on-line broker like: [HiveMQ](http://www.hivemq.com/demos/websocket-client/)

A better solution is to choose a broker and to install it on a local machine (i.e. a Raspberry PI).
The most widely used at the moment of writing is [Eclipse - Mosquitto](https://mosquitto.org/)

### Install MQTT library
Chips use MQTT protocol for communication. Herewith a link to the [library](https://github.com/knolleary/pubsubclient) which is the best and most wideley used so far.

### Get a Dashboard
In order to easily manage your "things" you should have a platform able to perform automation and give you control and visibility of your devices.
My choice is [Home Assistant](https://home-assistant.io/) (HA) integrated together with [Node-Red](https://nodered.org/) (NR).
If you choose to install HA operating system ([Hass.IO](https://home-assistant.io/hassio/)) then it will be very easy to install mosquitto and NR on the very same machine (preferred a RaspberryPi3) that will be your HUB for the "things".
To bond HA together with NODE-RED it is very easy to use specific nodes, available in NR via NPM installation [node-red-contrib-home-assistant](https://flows.nodered.org/node/node-red-contrib-home-assistant)
A docker installation for Node-RED inside HASS.io distribution is available here:
[notoriusBDG - hassio-addons - node-red](https://github.com/notoriousbdg/hassio-addons/tree/master/node-red)


### Actuators TOPIC and PAYLOAD naming criteria
With ACTUATORS we consider all the devices that perform action FROM the microcontroller to the real world (OUTPUTs). Actuators (LEDs,  relays, buzzer, etc...) communicate with Home Assistant standard naming which means:

- "ON" payload for turn on device
- "OFF" payload for turn off
- Brightness (where supported) the brightness of a light nedds to recieve a value between 0 and 255
- RGB lights (where supported) the color of a lamp is expressed in ad RGB format like following R,G,B

If different it will be written in description

 ACKNOLEDGMENT TOPIC:
 
 Every time a state of an actuator changes (via MQTT or via phisical control) the MCU notifies it communicating the very same payload he recieved (if it recieved that from an MQTT communication). If you want to use it in Home assistant you should use "optimistic: FALSE".
 the criteria is the following (use CamelCase):

 - {topic header}/functionSet -> command topic - subscribed to
 - {topic header}/setFunction -> status topic (confirm, or akcnoledgment topic) - publishing to

example:
 - sub topic: home/livingroom/brightnessSet
 - pub topic: home/livingroom/setBrightness

## Dependencies 
External libraries and dependencies resources can be found inline in the code.

## Authors
* **Marco Provolo** - *Initial work* - [GitHub](https://github.com/marcoprovolo)

## License
Copyright (C) 2016-2018 by Marco Provolo 

##### This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see http://www.gnu.org/licenses/.

## Acknowledgments
* just all open-source/open-hardware community
* thanks to [WEMAKE](http://wemake.cc/) milan's makerspace for creating a net of awsome people
* and thanks to me because without me all this wouldn't be possible :-|
