#include "esphome.h"
using namespace esphome;
 
int uart_value[9]; //save data measured by LiDAR
float distance;
const int HEADER=0x59; //frame header of data package
  
class TFminiSensor : public PollingComponent, public sensor::Sensor, public UARTDevice{
 public:
    TFminiSensor(uint32_t update_interval, UARTComponent *parent) : PollingComponent(update_interval), UARTDevice(parent) {}

    float get_setup_priority() const override { return esphome::setup_priority::AFTER_CONNECTION; }
	
	void setup() override{};

    void update() override {
     if( available() > 0 ) {    
		 if(read() == HEADER){;//Read while start packet
		  uart_value[0] = HEADER;
		  if (read() == HEADER) uart_value[1] = HEADER; //assess data package frame header 0x59
		  for (int i = 2; i < 9; i++) { //save data in array
			uart_value[i] = read();
		  }
	//	  flush();

		 int check = uart_value[0] + uart_value[1] + uart_value[2] + uart_value[3] + uart_value[4] + uart_value[5] + uart_value[6] + uart_value[7];
		 if (uart_value[8] == (check & 0xff)){ //verify the received data as per protocol
		 distance = uart_value[2] + uart_value[3] * 256; //calculate distance value
		 int strength = uart_value[4] + uart_value[5] * 256; //calculate signal strength value
		 float temprature = uart_value[6] + uart_value[7] *256;//calculate chip temprature
		 temprature = temprature/8 - 256;


//	  if (distance > 800) distance =0;
		  publish_state(distance);  //in meters
		};
		};
	 };
};
};
