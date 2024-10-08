#include <Servo.h>
#include "Cubeino.h"
#include "dotMatrix.h"
#include <SoftwareSerial.h>

#define GCUBE_TOTAL 3  // 이 프로젝트에 사용할 전체 큐브의 숫자 
SoftwareSerial myCubeSerial (2, 3); // 2:RX, 3:TX for uART 보드 제조사에 맞게 핀번호 변경하여 사용

unsigned int l_count=0;

void setup()
{
  Serial.begin(115200); // PC에 연결하여 모니터링에 사용하는 UART 통신
  myCubeSerial.begin(115200); // G큐브에 연결하여 사용하는 UART 통신

  Serial.println("Start");
  
  wait_for_first_cube_connected(myCubeSerial); // G큐브가 켜질 때 까지 기다리기
  wait_for_all_cube_connected(GCUBE_TOTAL, myCubeSerial); // 이 프로젝트에 필요한 큐브들이 모두 연결될 때까지 기다리기

  Serial.println("Sensor Hub start");

  delay(2000); //Wait for starting motion
}

void loop()
{
  byte sensor_data[3];
  byte analogPeriod = 0;
  byte bitFlag = 0;
  int sensor_result = 0;

  sensor_result = get_cube_ext_sensor_value(sensor_data, 2, GCUBE_EXT_SENSOR_COLOR_KEY, myCubeSerial); //1번 큐브의 외부포트에 꽂혀 있는 칼라센서 키값을 가져온다.
  Serial.print(" Color sensor KEY value:");
  Serial.print(sensor_data[2]);
  Serial.println(" ### ");

  switch(sensor_data[2]){
    case 0://Red -> 직진 10cm
      move_pingpong_robot(PINGPONG_AUTOCAR, 10, myCubeSerial); // 10 cm
      break;
    case 1://Green -> 좌회전 90
      rotate_pingpong_robot(PINGPONG_AUTOCAR, -90, myCubeSerial);
      break;
    case 2://Blue -> 우회전 90
      rotate_pingpong_robot(PINGPONG_AUTOCAR, 90, myCubeSerial);
      break;
    case 3://Yellow -> 후진 -10cm
      move_pingpong_robot(PINGPONG_AUTOCAR, -10, myCubeSerial); // 10 cm
      break;
    default:	break;
  }
/*
  sensor_result = get_cube_ext_sensor_value(sensor_data, 1, GCUBE_EXT_SENSOR_COLOR_VALUE, myCubeSerial); //1번 큐브의 외부포트에 꽂혀 있는 (선택된)센서 값을 가져온다.
  Serial.print(" Color sensor value:");
  Serial.print(" Red:");
  Serial.print(sensor_data[0]);
  Serial.print(" Green:");
  Serial.print(sensor_data[1]);
  Serial.print(" Blue:");
  Serial.print(sensor_data[2]);
  Serial.println(" ### ");
*/
  delay(1000); // +50msec(@sendData) = 100msec 
}
