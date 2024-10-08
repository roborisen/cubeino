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

  Serial.println("Sensor Hub start");

}

void loop()
{
  byte sensor_data[3];
  int sensor_result = 0;

  // 함수에서 선택한 큐브에 해당 샌서가 미리 꽂혀 있어야 함.
  sensor_result = get_cube_ext_sensor_value(sensor_data, 1, GCUBE_EXT_SENSOR_LIGHT, myCubeSerial); //1번 큐브의 외부포트에 꽂혀 있는 (선택된)센서 값을 가져온다.
  Serial.print(" Light sensor value:");
  Serial.print(sensor_result);
  Serial.print(" ### ");
  
  sensor_result = get_cube_ext_sensor_value(sensor_data, 1, GCUBE_EXT_SENSOR_SOUND, myCubeSerial); //1번 큐브의 외부포트에 꽂혀 있는 (선택된)센서 값을 가져온다.
  Serial.print(" Light sensor value:");
  Serial.print(sensor_result);
  Serial.print(" ### ");

  sensor_result = get_cube_ext_sensor_value(sensor_data, 1, GCUBE_EXT_SENSOR_VOLUME, myCubeSerial); //1번 큐브의 외부포트에 꽂혀 있는 (선택된)센서 값을 가져온다.
  Serial.print(" Volume sensor value:");
  Serial.print(sensor_result);
  Serial.print(" ### ");

  sensor_result = get_cube_ext_sensor_value(sensor_data, 1, GCUBE_EXT_SENSOR_MAGNETIC, myCubeSerial); //1번 큐브의 외부포트에 꽂혀 있는 (선택된)센서 값을 가져온다.
  Serial.print(" Magnetic sensor value:");
  Serial.print(sensor_result);
  Serial.print(" ### ");

  sensor_result = get_cube_ext_sensor_value(sensor_data, 1, GCUBE_EXT_SENSOR_ULTRASONIC, myCubeSerial); //1번 큐브의 외부포트에 꽂혀 있는 (선택된)센서 값을 가져온다.
  Serial.print(" Ultrasonic sensor value:");
  Serial.print(sensor_result);
  Serial.print(" ### ");

  sensor_result = get_cube_ext_sensor_value(sensor_data, 1, GCUBE_EXT_SENSOR_TEMPRATURE, myCubeSerial); //1번 큐브의 외부포트에 꽂혀 있는 (선택된)센서 값을 가져온다.
  Serial.print(" Temperature sensor value:");
  Serial.print(sensor_data[1]);
  Serial.print(".");
  Serial.print(sensor_data[2]);
  Serial.print("degree ");
  Serial.print(" ### ");

  sensor_result = get_cube_ext_sensor_value(sensor_data, 1, GCUBE_EXT_SENSOR_COLOR_KEY, myCubeSerial); //1번 큐브의 외부포트에 꽂혀 있는 칼라센서 키값을 가져온다.
  Serial.print(" Color sensor KEY value:");
  Serial.print(sensor_data[2]);
  Serial.print(" ### ");

  sensor_result = get_cube_ext_sensor_value(sensor_data, 1, GCUBE_EXT_SENSOR_COLOR_VALUE, myCubeSerial); //1번 큐브의 외부포트에 꽂혀 있는 (선택된)센서 값을 가져온다.
  Serial.print(" Color sensor value:");
  Serial.print(" Red:");
  Serial.print(sensor_data[0]);
  Serial.print(" Green:");
  Serial.print(sensor_data[1]);
  Serial.print(" Blue:");
  Serial.print(sensor_data[2]);
  Serial.print(" ### ");

  sensor_result = get_cube_ext_sensor_value(sensor_data, 1, GCUBE_EXT_SENSOR_ANALOG, myCubeSerial); //1번 큐브의 외부포트에 꽂혀 있는 (선택된)센서 값을 가져온다.
  Serial.print(" 8bit:");
  Serial.print(sensor_data[1]);
  Serial.print(" 12bit:");
  Serial.print(sensor_result);
  Serial.println(" ### ");

  delay(50); // +50msec(@sendData) = 100msec 
}
