# TFmini-ESPHome
Custom Sensor Component for ESPHome TFmini Plus LiDAR

#**************** UART ********************
uart:
  tx_pin: 1 #D8
  rx_pin: 3 #D7
  baud_rate: 115200
  id: uart_bus
  debug:
    direction: BOTH
    dummy_receiver: false
    after:
      delimiter: "\n"
    sequence:
     - lambda: UARTDebug::log_string(direction, bytes);
      
#******************** Sensor *********************
sensor:
  - platform: custom
#    id: distance
    lambda: |-
      auto distance_sensor = new TFminiSensor(1000, id(uart_bus));
      App.register_component(distance_sensor);
      return {distance_sensor};
    sensors:
      id: distance_value
      name: "Distance sensor"
      unit_of_measurement: "cm"
#      accuracy_decimals: 2
      filters:
        - median:
