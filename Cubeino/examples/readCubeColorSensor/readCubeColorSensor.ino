#include <Servo.h>
#include "Cubeino.h"
#include "dotMatrix.h"
#include <SoftwareSerial.h>

#define GCUBE_TOTAL 3  // PC 시리얼로 보여주며 큐브 1개를 더 사용해 도트매트릭스에 칼라키를 표시할 때 사용
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
}

void loop()
{
  byte sensor_data[3];
  byte analogPeriod = 0;
  byte bitFlag = 0;
  int sensor_result = 0;

  sensor_result = get_cube_ext_sensor_value(sensor_data, 1, GCUBE_EXT_SENSOR_COLOR_KEY, myCubeSerial); //1번 큐브의 외부포트에 꽂혀 있는 칼라센서 키값을 가져온다.
  Serial.print(" Color sensor KEY value:");
  Serial.print(sensor_data[2]);
  Serial.print(" ### ");
  
/*
  switch(sensor_data[2]){
    case 0:		send_matrix_still_image(2, letter_0, myCubeSerial);		break;
    case 1:		send_matrix_still_image(2, letter_1, myCubeSerial);		break;
    case 2:		send_matrix_still_image(2, letter_2, myCubeSerial);		break;
    case 3:		send_matrix_still_image(2, letter_3, myCubeSerial);		break;
    case 4:		send_matrix_still_image(2, letter_4, myCubeSerial);		break;
    case 5:		send_matrix_still_image(2, letter_5, myCubeSerial);		break;
    case 6:		send_matrix_still_image(2, letter_6, myCubeSerial);		break;
    case 7:		send_matrix_still_image(2, letter_7, myCubeSerial);		break;
    default:	send_matrix_still_image(2, full_Image, myCubeSerial);	break;
  }

*/
  sensor_result = get_cube_ext_sensor_value(sensor_data, 1, GCUBE_EXT_SENSOR_COLOR_VALUE, myCubeSerial); //1번 큐브의 외부포트에 꽂혀 있는 (선택된)센서 값을 가져온다.
  Serial.print(" Color sensor value:");
  Serial.print(" Red:");
  Serial.print(sensor_data[0]);
  Serial.print(" Green:");
  Serial.print(sensor_data[1]);
  Serial.print(" Blue:");
  Serial.print(sensor_data[2]);
  Serial.println(" ### ");
  
  delay(500); // +50msec(@sendData) = 550msec Color sensor reading period : 160msec
}
