#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include "Servo.h"
#include "Cubeino.h"

#define GCUBE_TOTAL 2  // 이 프로젝트에 사용할 전체 큐브의 숫자 

SoftwareSerial myCubeSerial (2, 3); // 2:RX, 3:TX for uART 보드 제조사에 맞게 핀번호 변경하여 사용

HUSKYLENS huskylens;

//HUSKYLENS green line >> SDA; blue line >> SCL
int ID1 = 1;


void setup() {
    Serial.begin(115200);
	myCubeSerial.begin(115200); // G큐브에 연결하여 사용하는 UART 통신

	Serial.println("Start Huskylens Line Tracking");
	
    // Wait for GCube connection
	wait_for_first_cube_connected(myCubeSerial); // G큐브가 켜질 때 까지 기다리기
	wait_for_all_cube_connected(GCUBE_TOTAL, myCubeSerial); // 이 프로젝트에 필요한 큐브들이 모두 연결될 때까지 기다리기

    Wire.begin();
    while (!huskylens.begin(Wire))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protol Type>>I2C)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }
    huskylens.writeAlgorithm(ALGORITHM_LINE_TRACKING); //Switch the algorithm to line tracking.
}

int left = 0, right = 0;

void loop() {
    int32_t error; 
    if (!huskylens.request()) {Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));left = 0; right = 0;}
    else if(!huskylens.isLearned()) {Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));left = 0; right = 0;}
    else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
    else
    {
        HUSKYLENSResult result = huskylens.read();
        // Calculate the error:
        error = (int32_t)result.xTarget - (int32_t)160;

//        left = 40 - error * 0.06;
//        right = 40 + error * 0.06;
        left = 30 - error * 0.08;
        right = 30 + error * 0.08;
//        left = 20 - error * 0.1;
//        right = 20 + error * 0.1;

        if(left<0) left=10; else if(left>100) left=100;
        if(right<0) right=10; else if(right>100) right=100;
    }

    Serial.println(String()+left+","+right);
	
	send_multi_cube_speed(GCUBE_TOTAL, -1*right,left,0,0,0,0,0,0, myCubeSerial); // G0:-1*right, G1:left, G2.....:0

}

