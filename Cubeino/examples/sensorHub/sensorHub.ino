#include <Servo.h>
#include "Cubeino.h"
#include <SoftwareSerial.h>

SoftwareSerial myCubeSerial (2, 3); // 2:RX, 3:TX for uART 보드 제조사에 맞게 핀번호 변경하여 사용

unsigned int l_count=0;
byte analogPeriod = 0;

void setup()
{
  Serial.begin(115200); // PC에 연결하여 모니터링에 사용하는 UART 통신
  myCubeSerial.begin(115200); // G큐브에 연결하여 사용하는 UART 통신

  Serial.println("Sensor Hub start");
  
  wait_for_hub_cube_connected(myCubeSerial); 
  
  Serial.println("Sensor Hub Cube connected");
}


void loop()
{
	byte cmd_data[3];

	get_req_command(cmd_data, myCubeSerial);
	switch(cmd_data[0]){
		case GCUBE_REQ_DIGITAL_SET: 
			set_digital_port(cmd_data[1], cmd_data[2]);
			break;
		default: 
			break;
	}
		
	report_analog_data(myCubeSerial);
	delay(50); // +50msec(@sendData) = 100msec (default sensor report period of Cubeino -> GCube)
}