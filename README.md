# Home-Automation-Devices
Various usefull/useless IoT devices connected to NodeRed and Home Assistant using Arduino IDE and ESP8266 and MQTT for communication over WiFi.

## Getting Started
Most of devices here are based on ESP8266 chip (WEMOS mini proto board).

In order to install ESP8266 core in arduino IDE you can see [this repository](https://github.com/esp8266/Arduino) or follow instructions [here](https://arduino-esp8266.readthedocs.io/en/2.4.0/)

Chips use MQTT protocol for communication. Herewith a link to the [library](https://github.com/knolleary/pubsubclient) which is the best and most wideley used so far.

They are used hooked up to [NODE-RED](https://nodered.org/) and [HOME ASSISTANT](https://home-assistant.io/) (HA) platforms.
The easiest way is to use HA as dashboard and main control system while I use NODE-RED for creating automations.
To bond HA together with NODE-RED it is very easy to use specific nodes available within node red [node-red-contrib-home-assistant](https://flows.nodered.org/node/node-red-contrib-home-assistant) 

By the way these devices can be used in many different ways and connected to whatever platform you prefer 

## Authors
* **Marco Provolo** - *Initial work* - [marco provolo github](https://github.com/marcoprovolo)

## License
Copyright (C) 2017-2018 by Marco Provolo

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program. If not, see http://www.gnu.org/licenses/.


## Acknowledgments
* just all open-source/open-hardware community
* thanks to [WEMAKE](http://wemake.cc/) milan's makerspace for creating a net of awsome people
* and thanks to me because without me all this wouldn't be possible :-|
