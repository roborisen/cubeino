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

  Serial.println("All cubes are connected");
}


void loop()
{
    if(Serial.available()){
      char c = Serial.read();
      
      if(c == '0'){
        send_a_cube_speed(0, 100, myCubeSerial);
      }else if(c == '1'){
        send_a_cube_speed(1, 100, myCubeSerial);
      }else if(c == '2'){
        send_a_cube_speed(0, 50, myCubeSerial);
      }else if(c == '3'){
        send_a_cube_speed(1, 50, myCubeSerial);
      }else if(c == '4'){
        stop_all_cube_motor(GCUBE_TOTAL, myCubeSerial);
      }else if(c == '5'){
        send_multi_cube_speed(GCUBE_TOTAL, 100,100,0,0,0,0,0,0, myCubeSerial); //G0:100, G1:100, G2....:0
      }
	}
}