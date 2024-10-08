#include <Servo.h>
#include "Cubeino.h"
#include <SoftwareSerial.h>

#define GCUBE_TOTAL 2  // 이 프로젝트에 사용할 전체 큐브의 숫자 
//#define GCUBE_TOTAL 3  // 이 프로젝트에 사용할 전체 큐브의 숫자 

SoftwareSerial myCubeSerial (2, 3); // 2:RX, 3:TX for uART 보드 제조사에 맞게 핀번호 변경하여 사용

unsigned int l_count=0;
bool first_trigger = false;
bool second_trigger = false;


void setup()
{
  Serial.begin(115200); // PC에 연결하여 모니터링에 사용하는 UART 통신
  myCubeSerial.begin(115200); // G큐브에 연결하여 사용하는 UART 통신

  Serial.println("Start");
  
  wait_for_first_cube_connected(myCubeSerial); // G큐브가 켜질 때 까지 기다리기
  wait_for_all_cube_connected(GCUBE_TOTAL, myCubeSerial); // 이 프로젝트에 필요한 큐브들이 모두 연결될 때까지 기다리기

  Serial.println("All cubes are connected");

  delay(2000); //Wait for starting motion
}


void loop()
{
  byte first_data[3], second_data[3];
  get_cube_proxi_value(first_data, 0, myCubeSerial); //0번 큐브의 근접센서 값을 가져온다.

  if(first_data[2]>180){
    first_trigger = true;
  }else{
    first_trigger = false;
  }

  if(first_trigger){
    first_trigger=false;
    move_pingpong_robot(PINGPONG_AUTOCAR, 10, myCubeSerial); // 10 cm
    rotate_pingpong_robot(PINGPONG_AUTOCAR, 90, myCubeSerial); // 90 degree (+:right turn)
    move_pingpong_robot(PINGPONG_AUTOCAR, 10, myCubeSerial); // 10 cm
    rotate_pingpong_robot(PINGPONG_AUTOCAR, 90, myCubeSerial); // 90 degree (+:right turn)
    move_pingpong_robot(PINGPONG_AUTOCAR, 10, myCubeSerial); // 10 cm
    rotate_pingpong_robot(PINGPONG_AUTOCAR, 90, myCubeSerial); // 90 degree (+:right turn)
  }
}