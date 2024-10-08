#include <Servo.h>
#include "Cubeino.h"
#include <SoftwareSerial.h>

#define GCUBE_TOTAL 2 // 이 프로젝트에 사용할 전체 큐브의 숫자 
#define SERVO_PIN 4 // 서보모터를 연결한 포트
SoftwareSerial myCubeSerial (2, 3); // 2:RX, 3:TX for uART 보드 제조사에 맞게 핀번호 변경하여 사용
Servo myServo;

byte servo_angle =0;

void setup()
{
  Serial.begin(115200); // PC에 연결하여 모니터링에 사용하는 UART 통신
  myCubeSerial.begin(115200); // G큐브에 연결하여 사용하는 UART 통신
  myServo.attach(SERVO_PIN);

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
        send_a_servo_angle(0, servo_angle, myServo, myCubeSerial);
      }else if(c == '1'){
        send_a_servo_angle(1, servo_angle, myServo, myCubeSerial);
      }else if(c == '2'){
        send_multi_servo_angle(GCUBE_TOTAL, servo_angle,servo_angle,0,0,0,0,0,0, myServo, myCubeSerial);
      }else if(c == '3'){
        send_multi_servo_angle(GCUBE_TOTAL, 90,90,0,0,0,0,0,0, myServo, myCubeSerial);
      }else if(c == '4'){
      }
	}

	servo_angle ++;
	servo_angle=servo_angle%180;
}