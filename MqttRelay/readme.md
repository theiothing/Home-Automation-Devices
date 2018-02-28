# MQTT Relay:

![relay wiring](relayschema.png)


## HOME ASSISTANT integration

If headerTopic is changed
```
"{headertopic}/setPower"
"{headertopic}/powerSet" 
```
i.e. headerTopic = "downstairs/livingroom/sofalight/" your power topics will be:
```
downstairs/livingroom/sofalight/setPower
downstairs/livingroom/sofalight/powerSet
```
If the relay controls a socket or a generic load
```
switch:
  platform: mqtt
  name: "my_first_controlled_device"
  command_topic: "myESP/setPower" 
  state_topic: "myESP/powerSet" 
  payload_on: "ON"
  payload_off: "OFF"
  optimistic: false
  qos: 1
  retain: true
```
If the relay controls a light then:
```
light:
  platform: mqtt
  name: "my_first_controlled_relay"
  command_topic: "myESP/setPower" 
  state_topic: "myESP/powerSet" 
  payload_on: "ON"
  payload_off: "OFF"
  optimistic: false
  qos: 1
  retain: true
  ```
  ## NODE RED integration
  The following nodes create a switch able to control the relay with NODE_RED dashboard
  
  ![nodered relay](nodered%20switch.png)
  
        [{"id":"e50ee7eb.b059f8","type":"mqtt out","z":"6f71c430.146c64","name":"command topic","topic":"","qos":"","retain":"","broker":"","x":620,"y":520,"wires":[]},{"id":"5ecbc4ac.3db62c","type":"mqtt in","z":"6f71c430.146c64","name":"state topic","topic":"myESP/powerSet","qos":"1","broker":"","x":232,"y":521,"wires":[["1e91c6c7.dd6be9"]]},{"id":"1e91c6c7.dd6be9","type":"ui_switch","z":"6f71c430.146c64","name":"","label":"MQTT RELAY","group":"f689e960.40baa8","order":0,"width":0,"height":0,"passthru":false,"decouple":"true","topic":"","style":"","onvalue":"ON","onvalueType":"str","onicon":"","oncolor":"","offvalue":"OFF","offvalueType":"str","officon":"","offcolor":"","x":413.8299102783203,"y":520.5555725097656,"wires":[["e50ee7eb.b059f8"]]},{"id":"f689e960.40baa8","type":"ui_group","z":"","name":"Default","tab":"","disp":true,"width":"6"}]
