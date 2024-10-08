#include <Servo.h>
#include "Cubeino.h"
#include <SoftwareSerial.h>

#define GCUBE_TOTAL 2  // 이 프로젝트에 사용할 전체 큐브의 숫자 
SoftwareSerial myCubeSerial (2, 3); // 2:RX, 3:TX for uART 보드 제조사에 맞게 핀번호 변경하여 사용

unsigned int l_count=0;
unsigned int left=0;
unsigned int right=0;

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
      // send_a_Cube_Speed(0, 100, myCubeSerial);

    left= analogRead(A2);
	right= analogRead(A3);
	  
	Serial.print("LEFT: ");
    Serial.print(left);
	Serial.print("  RIGHT: ");
    Serial.println(right);

    // nominal speed 30 with left sensor:line:right sensor
    if(left < 100 & right < 100) send_multi_cube_speed(GCUBE_TOTAL, -30, 30, 0, 0, 0, 0, 0, 0, myCubeSerial);
    else if(left > 100) send_multi_cube_speed(GCUBE_TOTAL, -22, 38, 0, 0, 0, 0, 0, 0, myCubeSerial);
    else if(right > 100) send_multi_cube_speed(GCUBE_TOTAL, -38, 22, 0, 0, 0, 0, 0, 0, myCubeSerial);
    else send_multi_cube_speed(GCUBE_TOTAL, 0,0,0,0,0,0,0,0, myCubeSerial);

}
