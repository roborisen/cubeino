#include <Servo.h>
#include "Cubeino.h"
#include <SoftwareSerial.h>

#define GCUBE_TOTAL 2  // 이 프로젝트에 사용할 전체 큐브의 숫자 
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
  int8_t temp[3];
  get_cube_acc_value(temp, 0, myCubeSerial); // Get x, y accelerometer value of the GCube 0

  Serial.print("x: ");
  Serial.print((int8_t)temp[1]);
  Serial.print("  y: ");
  Serial.print((int8_t)temp[2]);
  Serial.println("");

  // Shift image along the acc data with Sync. all connected GCubes  
  start_matrix_shift_image(0, GCUBE_TOTAL, roll_Image, temp[1], temp[2], myCubeSerial); 

}
