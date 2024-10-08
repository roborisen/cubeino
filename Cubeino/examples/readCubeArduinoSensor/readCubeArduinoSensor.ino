#include <Servo.h>
#include "Cubeino.h"
#include <SoftwareSerial.h>

#define GCUBE_TOTAL 2  // 이 프로젝트에 사용할 전체 큐브의 숫자 
SoftwareSerial myCubeSerial (2, 3); // 2:RX, 3:TX for uART 보드 제조사에 맞게 핀번호 변경하여 사용

unsigned int l_count=0;

void setup()
{
  Serial.begin(115200); // PC에 연결하여 모니터링에 사용하는 UART 통신
  myCubeSerial.begin(115200); // G큐브에 연결하여 사용하는 UART 통신

  Serial.println("Start");
  
  wait_for_first_cube_connected(myCubeSerial); // G큐브가 켜질 때 까지 기다리기
  wait_for_all_cube_connected(GCUBE_TOTAL, myCubeSerial); // 이 프로젝트에 필요한 큐브들이 모두 연결될 때까지 기다리기

  Serial.println("Arduino sensor reading start");
}

void loop()
{
  int sensor_value;
  uint16_t iflag = 0;

  sensor_value = get_cube_arduino_analog_value(1, 0, myCubeSerial); //1번 큐브에 꽂혀 있는 아두이노의 아날로그 포트 0번의 값을 읽어 온다.
  Serial.print(" Analog value: ");
  Serial.println(sensor_value);
 
  // 40*50 = 2000ms (2초) 주기로 1번 큐브에 꽂혀 있는 아두이노의 디지털 포트 전체를 끄고 켠다.
  if (iflag%40==0)
	  set_cube_arduino_digital_value(1, 0xFF, 0xFF, myCubeSerial); // 1번 큐브에 꽂혀 있는 아두이노의 디지털 포트의 값을 모두 1로 세팅한다.
  else if(iflag%40==20)
	  set_cube_arduino_digital_value(1, 0x00, 0x00, myCubeSerial); // 1번 큐브에 꽂혀 있는 아두이노의 디지털 포트의 값을 모두 0로 세팅한다.
  
  iflag++;
  
  delay(50); // +50msec(@sendData) = 100msec
}
