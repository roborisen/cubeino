#include <Servo.h>
#include "Cubeino.h"
#include "dotMatrix.h"
#include <SoftwareSerial.h>

#define GCUBE_TOTAL 3  // 이 프로젝트에 사용할 전체 큐브의 숫자 
SoftwareSerial myCubeSerial (2, 3); // 2:RX, 3:TX for uART 보드 제조사에 맞게 핀번호 변경하여 사용

unsigned int l_count=0;
unsigned int roll_index=0;

byte roll_Image[] = {
  B00000000,
  B00000000,
  B00011000,
  B00111100,
  B00111100,
  B00011000,
  B00000000,
  B00000000  
};


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
  send_matrix_still_image(1, full_Image, myCubeSerial);
  delay(50);
  send_matrix_still_image(2, full_Image, myCubeSerial);
  delay(1000);
  send_matrix_still_image(1, null_Image, myCubeSerial);
  send_matrix_still_image(2, null_Image, myCubeSerial);
  delay(500);

  byte duration = 5; // 롤링시간(초) 계속하려면 0
  start_matrix_roll_image(GCUBE_TOTAL, roll_Image, duration, myCubeSerial);
}