
#include "Arduino.h"
#include "Servo.h"
#include "Cubeino.h"
#include "SoftwareSerial.h"

char old_s0=0;
char old_s1=0;
char old_s2=0;

void send_gcube(byte x0, byte x1, byte x2, byte x3, byte x4, byte x5, byte x6, byte x7, byte x8, byte x9, Stream &cubeSerial){
  cubeSerial.write(x0);
  cubeSerial.write(x1);
  cubeSerial.write(x2);
  cubeSerial.write(x3);
  cubeSerial.write(x4);
  cubeSerial.write(x5);
  cubeSerial.write(x6);
  cubeSerial.write(x7);
  cubeSerial.write(x8);
  cubeSerial.write(x9);
  delay(50);
}

//################## 변경 금지 #################//
void wait_for_first_cube_connected(Stream &cubeSerial){

  int rcvData[3];
  int rcvIndex = 0;

  while(1){
    if(cubeSerial.available()){
      rcvData[rcvIndex] = cubeSerial.read();

      if(rcvIndex>=2){
        if(rcvData[rcvIndex-2]==GCUBE_GET_BOARD_ID && rcvData[rcvIndex-1]==0x00 && rcvData[rcvIndex]==0x00){
          send_gcube(GCUBE_GET_BOARD_ID, get_iv(GCUBE_GET_BOARD_ID), 0,0,0, GCUBE_CUBEINO_BOARD_ID, 0,0,0,0, cubeSerial);
          break;
        }
      }
      if(rcvData[0]==GCUBE_GET_BOARD_ID) rcvIndex++;
    }
  }
}

void wait_for_hub_cube_connected(Stream &cubeSerial){
  int rcvData[3];
  int rcvIndex = 0;

  while(1){
    if(cubeSerial.available()){
      rcvData[rcvIndex] = cubeSerial.read();

      if(rcvIndex>=2){
        if(rcvData[rcvIndex-2]==GCUBE_GET_BOARD_ID && rcvData[rcvIndex-1]==0x00 && rcvData[rcvIndex]==0x00){
          send_gcube(GCUBE_GET_BOARD_ID, get_iv(GCUBE_GET_BOARD_ID), 0,0,0, GCUBE_HUB_BOARD_ID, 0,0,0,0, cubeSerial);
          break;
        }
      }
      if(rcvData[0]==GCUBE_GET_BOARD_ID) rcvIndex++;
    }
  }
}


void wait_for_all_cube_connected(byte GCUBE_TOTAL, Stream &cubeSerial){

  int rcvData[3];
  int rcvIndex = 0;
  byte cube_number = 0;
  unsigned int t_count=1;
  
  while(1){    
    //Send 1st request after 1 second
    if(t_count++%1000==0) send_gcube(GCUBE_GET_CUBE_NUMBER, get_iv(GCUBE_GET_CUBE_NUMBER),0,0,0,0,0,0,0,0, cubeSerial);
    delay(1);
    
    if(cubeSerial.available()){
          rcvData[rcvIndex] = cubeSerial.read();
          Serial.println(rcvData[rcvIndex]);
          if(rcvData[0]==GCUBE_GET_CUBE_NUMBER) rcvIndex++;
    }
  
    if(rcvIndex>=3){
      if(rcvData[0]==GCUBE_GET_CUBE_NUMBER && rcvData[1]==0x00){
        cube_number = rcvData[2]; // STAR큐브에 연결된 큐브 숫자 (STAR큐브포함) 저장
      }
      rcvIndex=0;
      if(cube_number>=GCUBE_TOTAL){
		t_count = 0;
		rcvIndex = 0;
		
		delay(1000);
		//모든 큐브 연결되었음을 GCube0에게 알려줌. GCube0는 연결된 모든 큐브들에게 getSensor 명령 전송
		send_gcube(GCUBE_SET_ALL_CUBES_CONNECTED, get_iv(GCUBE_SET_ALL_CUBES_CONNECTED), GCUBE_TOTAL,0,0,0,0,0,0,0, cubeSerial);
		delay(1000);
		
		break;
	  }
    }
  }
}
//################## 변경 금지 #################//


byte get_iv(byte a){
  return( (a/16) + (a & 0x0F)*16 );
}

void start_matrix_shift_image(byte cubeIndex, byte GCUBE_TOTAL, byte rollImage[], int8_t xp, int8_t yp, Stream &cubeSerial){
  // cubeIndex = 0 : apply all connected GCubes
  // cubeIndex > 0 : just apply only the GCube of cubeIndex

	byte roll[8]={};

	for (int i = 0; i < 8; i++) roll[i] = 0; // When maximum number of GCubes (8) are connected.

	//infinite loop for stop_rolling_matrix_image
	byte temp = GCUBE_TOTAL - 1; // the number of dot matrix plugged GCubes

	xp = xp * 4 / 90; // -4 ~ 4
	yp = yp * 4 / 90; // -4 ~ 4

  if(abs(xp)>4 || abs(yp)>4 ) return;

	if (cubeIndex > 0) {
		for (int i = 0; i < 8; i++) roll[i] = 0;
		for (int y = yp; y < 8; y++) {
			if (y >= 0) {
				if (xp >= 0)
					roll[y] = rollImage[y - yp] >> xp;
				else
					roll[y] = rollImage[y - yp] << (-1 * xp);
			}
		}

		send_matrix_still_image(cubeIndex, roll, cubeSerial);
	} else {
		for (int i = 0; i < 8; i++) roll[i] = 0;
		for (int y = yp; y < 8; y++) {
			if (y >= 0) {
				if (xp >= 0)
					roll[y] = rollImage[y - yp] >> xp;
				else
					roll[y] = rollImage[y - yp] << (-1 * xp);
			}
		}

		for (int c = 0; c < temp; c++) {
			send_matrix_still_image(c+1, roll, cubeSerial);
		}
	}
}


void get_cube_echo_value(uint8_t mdata[], Stream &cubeSerial){
  send_gcube(GCUBE_GET_CUBE_ECHO_BASE, get_iv(GCUBE_GET_CUBE_ECHO_BASE), 0,0,0,0,0,0,0,0, cubeSerial);
  
  for(int i=0; i<3; i++){
    if(cubeSerial.available()){
      mdata[i]=cubeSerial.read();
    }else mdata[i]=0;
  }
}


void get_cube_acc_value(int8_t mdata[], byte c_number, Stream &cubeSerial){
  send_gcube(GCUBE_GET_CUBE_GYRO_BASE+c_number, get_iv(GCUBE_GET_CUBE_GYRO_BASE+c_number), 0,0,0,0,0,0,0,0, cubeSerial);
  
  for(int i=0; i<3; i++){
    if(cubeSerial.available()){
      mdata[i]=cubeSerial.read();
    }else mdata[i]=0;
  }
}



void get_cube_proxi_value(uint8_t mdata[], byte c_number, Stream &cubeSerial){
  send_gcube(GCUBE_GET_CUBE_PROXI_BASE, get_iv(GCUBE_GET_CUBE_PROXI_BASE), c_number,0,0,0,0,0,0,0, cubeSerial);
  
  for(int i=0; i<3; i++){
    if(cubeSerial.available()){
      mdata[i]=cubeSerial.read();
    }else mdata[i]=0;
  }
}

void get_cube_button_value(uint8_t mdata[], byte c_number, Stream &cubeSerial){
  send_gcube(GCUBE_GET_CUBE_BUTTON_BASE, get_iv(GCUBE_GET_CUBE_BUTTON_BASE), c_number,0,0,0,0,0,0,0, cubeSerial);
  
  for(int i=0; i<3; i++){
    if(cubeSerial.available()){
      mdata[i]=cubeSerial.read();
    }else mdata[i]=0;
  }
}

uint16_t get_cube_ext_sensor_value(uint8_t mdata[], byte c_number, byte s_name, Stream &cubeSerial){

	if(s_name == GCUBE_EXT_SENSOR_COLOR_KEY){
		send_gcube(GCUBE_GET_CUBE_EXT_COLOR_KEY+c_number, get_iv(GCUBE_GET_CUBE_EXT_COLOR_KEY+c_number), 0,0,0,0,0,0,0,0, cubeSerial);
	}else if(s_name == GCUBE_EXT_SENSOR_COLOR_VALUE){
		send_gcube(GCUBE_GET_CUBE_EXT_COLOR_VALUE+c_number, get_iv(GCUBE_GET_CUBE_EXT_COLOR_VALUE+c_number), 0,0,0,0,0,0,0,0, cubeSerial);
	}else{
		send_gcube(GCUBE_GET_CUBE_EXT_ANALOG_2BYTE+c_number, get_iv(GCUBE_GET_CUBE_EXT_ANALOG_2BYTE+c_number), 0,0,0,0,0,0,0,0, cubeSerial);
	}
  
  for(int i=0; i<3; i++){
    if(cubeSerial.available()){
      mdata[i]=cubeSerial.read();
    }else mdata[i]=0;
  }
  
  switch(s_name){
	  case GCUBE_EXT_SENSOR_LIGHT:
	  case GCUBE_EXT_SENSOR_SOUND:
	  case GCUBE_EXT_SENSOR_VOLUME:
	  case GCUBE_EXT_SENSOR_MAGNETIC:
	  case GCUBE_EXT_SENSOR_ANALOG:
		uint16_t aaa=(uint16_t)mdata[1]*16;
		aaa+=(uint16_t)mdata[2];
		return aaa;
		break;

	  case GCUBE_EXT_SENSOR_ULTRASONIC:
		return mdata[1];
		break;

	  case GCUBE_EXT_SENSOR_TEMPRATURE:
		return mdata[1];
		break;

	  case GCUBE_EXT_SENSOR_COLOR_VALUE:
			return 0;
		break;
		
	  default :
			return 0;
		break;
  }
}

uint16_t get_cube_arduino_analog_value(byte c_number, byte a_port, Stream &cubeSerial){
  send_gcube(GCUBE_GET_CUBE_ARDUINO_ANALOG_BASE+c_number, get_iv(GCUBE_GET_CUBE_ARDUINO_ANALOG_BASE+c_number), a_port,0,0,0,0,0,0,0, cubeSerial);

  uint16_t mdata[3];  
  for(int i=0; i<3; i++){
    if(cubeSerial.available()){
      mdata[i]=cubeSerial.read();
    }else mdata[i]=0;
  }
  uint16_t aaa=(uint16_t)mdata[1]*16;
  aaa+=(uint16_t)mdata[2];
  return aaa;
}

void set_cube_arduino_analog_start(byte c_number, Stream &cubeSerial){
  send_gcube(GCUBE_SET_CUBE_ARDUINO_ANALOG_START, get_iv(GCUBE_SET_CUBE_ARDUINO_ANALOG_START), c_number,0,0,0,0,0,0,0, cubeSerial);
  send_gcube(GCUBE_SET_CUBE_ARDUINO_ANALOG_START, get_iv(GCUBE_SET_CUBE_ARDUINO_ANALOG_START), c_number,0,0,0,0,0,0,0, cubeSerial);
}

void get_req_command(uint8_t mdata[], Stream &cubeSerial){
  for(int i=0; i<3; i++){
    if(cubeSerial.available()){
      mdata[i]=cubeSerial.read();
    }else mdata[i]=0;
  }
}

void report_analog_data(Stream &cubeSerial){
	// bitFlag : B00xxxxxx A3~A0 A3,A2: 8bit, A1,A0:10bit
	uint16_t a[4]={};
	
	for(int i=0;i<4;i++){
		a[i] = analogRead(i);
	}
	a[3]=a[3]/4; //1024->256
	a[2]=a[2]/4; //1024->256
	a[1]=a[1]/4; //1024->256
	byte a0_hi=(byte)a[0]/256;
	byte a0_lo=(byte)a[0]%256;
	
	send_gcube(GCUBE_REQ_ANALOG_READ, get_iv(GCUBE_REQ_ANALOG_READ), (byte)a[3],(byte)a[2],(byte)a[1],a0_hi,a0_lo,0,0,0, cubeSerial);
}


void set_cube_arduino_digital_value(byte c_number,byte port_hi, byte port_lo, Stream &cubeSerial){
  send_gcube(GCUBE_SET_CUBE_ARDUINO_DIGITAL_CONTROL, get_iv(GCUBE_SET_CUBE_ARDUINO_DIGITAL_CONTROL), c_number,port_hi, port_lo,0,0,0,0,0, cubeSerial);
}


void set_digital_port(byte vh, byte vl){
	// bit Flag : b4~b13 => D4~D13
	for(int i=4; i<14; i++){
		pinMode(i, OUTPUT);
		if(i<8) digitalWrite(i, vl & (byte)pow(2,i));
		else  digitalWrite(i, vh & (byte)pow(2,i-8));
	}
}

void send_a_cube_speed(byte cube_number, char cube_speed, Stream &cubeSerial){
  send_gcube(GCUBE_SET_A_MOTOR_SPEED, get_iv(GCUBE_SET_A_MOTOR_SPEED), cube_number, cube_speed,0,0,0,0,0,0,cubeSerial);
}

//void send_multi_cube_speed(byte GCUBE_TOTAL, char s7, char s6, char s5, char s4, char s3, char s2, char s1, char s0, Stream &cubeSerial){
void send_multi_cube_speed(byte GCUBE_TOTAL, char s0, char s1, char s2, char s3, char s4, char s5, char s6, char s7, Stream &cubeSerial){
  GCUBE_TOTAL=GCUBE_TOTAL-2;
	send_gcube(GCUBE_SET_MULTI_MOTOR_SPEED+GCUBE_TOTAL, get_iv(GCUBE_SET_MULTI_MOTOR_SPEED+GCUBE_TOTAL), s7, s6, s5, s4, s3, s2, s1, s0,cubeSerial);
/*  
  if(GCUBE_TOTAL > 1 ){ //Total Cube is more than four
	send_gcube(GCUBE_SET_MULTI_MOTOR_SPEED+GCUBE_TOTAL, get_iv(GCUBE_SET_MULTI_MOTOR_SPEED+GCUBE_TOTAL), s7, s6, s5, s4, s3, s2, s1, s0,cubeSerial);
  }else{
	if( abs(s2-old_s2)+abs(s1-old_s1)+abs(s0-old_s0) != 0){ // send command when the speed is changed
		send_gcube(GCUBE_SET_MULTI_MOTOR_SPEED+GCUBE_TOTAL, get_iv(GCUBE_SET_MULTI_MOTOR_SPEED+GCUBE_TOTAL), s7, s6, s5, s4, s3, s2, s1, s0,cubeSerial);
		old_s0 = s0;
		old_s1 = s1;
		old_s2 = s2;
	}
  }
*/  
}

void stop_all_cube_motor(byte GCUBE_TOTAL, Stream &cubeSerial){
  send_multi_cube_speed(GCUBE_TOTAL, 0,0,0,0,0,0,0,0, cubeSerial);
}

void send_a_cube_angle(byte cube_number, int cube_angle, Stream &cubeSerial){
  cube_angle = 100*cube_angle/18;
  send_gcube(GCUBE_SET_A_MOTOR_ANGLE, get_iv(GCUBE_SET_A_MOTOR_ANGLE), cube_number, (cube_angle>>8)&0xFF,cube_angle&0xFF,0,0,0,0,0,cubeSerial);
}

//void send_multi_cube_angle(byte cube_range, int s3, int s2, int s1, int s0, Stream &cubeSerial){
void send_multi_cube_angle(byte cube_range, int s0, int s1, int s2, int s3, Stream &cubeSerial){
  cube_range=cube_range-2;
  s3 = 100*s3/18;
  s2 = 100*s2/18;
  s1 = 100*s1/18;
  s0 = 100*s0/18;
  send_gcube(GCUBE_SET_MULTI_MOTOR_ANGLE+cube_range, get_iv(GCUBE_SET_MULTI_MOTOR_ANGLE+cube_range), (s3>>8)&0xFF, s3&0xFF, (s2>>8)&0xFF, s2&0xFF, (s1>>8)&0xFF, s1&0xFF, (s0>>8)&0xFF, s0&0xFF,cubeSerial);
}

void send_a_servo_angle(byte cube_number, byte cube_angle, Servo &svo, Stream &cubeSerial){
  if(cube_number ==0){
  svo.write(cube_angle);
  }else{
  send_gcube(GCUBE_SET_A_SERVO_ANGLE, get_iv(GCUBE_SET_A_SERVO_ANGLE), cube_number, cube_angle,0,0,0,0,0,0,cubeSerial);
  }
}

//void send_multi_servo_angle(byte GCUBE_TOTAL, byte s7, byte s6, byte s5, byte s4, byte s3, byte s2, byte s1, byte s0, Servo &svo, Stream &cubeSerial){
void send_multi_servo_angle(byte GCUBE_TOTAL, byte s0, byte s1, byte s2, byte s3, byte s4, byte s5, byte s6, byte s7, Servo &svo, Stream &cubeSerial){
  GCUBE_TOTAL=GCUBE_TOTAL-2;
  svo.write(s0);
  send_gcube(GCUBE_SET_MULTI_SERVO_ANGLE+GCUBE_TOTAL, get_iv(GCUBE_SET_MULTI_SERVO_ANGLE+GCUBE_TOTAL), s7, s6, s5, s4, s3, s2, s1, s0,cubeSerial);
}

void send_matrix_still_image(byte cube_index, byte m[], Stream &cubeSerial){
  cube_index = cube_index-1;
  send_gcube(GCUBE_SET_DOTMATRIX_IMAGE_DATA+cube_index, get_iv(GCUBE_SET_DOTMATRIX_IMAGE_DATA+cube_index), m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7],cubeSerial);
}

void set_matrix_blink_rate(byte cube_index, byte blinkrate, Stream &cubeSerial){
  send_gcube(GCUBE_SET_DOTMATRIX_BLINK_RATE, get_iv(GCUBE_SET_DOTMATRIX_BLINK_RATE), cube_index, blinkrate, 0, 0, 0, 0, 0, 0,cubeSerial);	
}

void set_matrix_pixel_data(byte cube_index, byte x, byte y, byte onoff, Stream &cubeSerial){
  send_gcube(GCUBE_SET_DOTMATRIX_PIXEL_DATA, get_iv(GCUBE_SET_DOTMATRIX_PIXEL_DATA), cube_index, x, y, onoff, 0, 0, 0, 0,cubeSerial);	
}

void set_matrix_string_data(byte cube_index, byte s0, byte s1, byte s2, byte s3, byte s4, byte s5, byte s6, byte s7, Stream &cubeSerial){
	if(cube_index>0) cube_index = cube_index-1;
  send_gcube(GCUBE_SET_DOTMATRIX_STRING_DATA+cube_index, get_iv(GCUBE_SET_DOTMATRIX_STRING_DATA+cube_index), s0, s1, s2, s3, s4, s5, s6, s7,cubeSerial);
}

void start_matrix_roll_image(byte GCUBE_TOTAL, byte m[], int duration, Stream &cubeSerial){
  GCUBE_TOTAL = GCUBE_TOTAL -1 ; // the number of Cube which has dotMatrix
  byte roll[GCUBE_TOTAL*8];
  byte roll_on[2]; // two matrix sector which should be refreshed
  byte current_roll_index = 0;
  byte total_line = GCUBE_TOTAL * 8;
  int d_index = 0;
  bool roll_flag = true;
 
  while(roll_flag){
  for(int i=0; i<total_line; i++) roll[i]=0;
  
  for(int s=current_roll_index; s<current_roll_index+8; s++){
       roll[s%total_line] = m[s-current_roll_index];
  }

  if(current_roll_index%8==0){
	  roll_on[0]= (byte)(current_roll_index/8);
	  roll_on[1]= (byte)(current_roll_index/8-1);
	  if(roll_on[1]<0) roll_on[1]=GCUBE_TOTAL-1;
  }else{
	  roll_on[0]= (byte)(current_roll_index/8);
	  roll_on[1]= (byte)(current_roll_index/8+1);
	  if(roll_on[1]>=GCUBE_TOTAL) roll_on[1]=0;	  
  }

  current_roll_index ++;
  current_roll_index = current_roll_index % total_line;

  for (int i = 0; i < GCUBE_TOTAL; i++){
	if(roll_on[0] == i || roll_on[1] == i){ // Refresh the matrix display only for the matrix the image is passing on
      send_gcube(GCUBE_SET_DOTMATRIX_IMAGE_DATA+i, get_iv(GCUBE_SET_DOTMATRIX_IMAGE_DATA+i), roll[8 * i + 0], roll[8 * i + 1], roll[8 * i + 2], roll[8 * i + 3], roll[8 * i + 4], roll[8 * i + 5], roll[8 * i + 6], roll[8 * i + 7], cubeSerial);
      d_index ++;
	}
  }
  if(d_index >= duration*20) roll_flag = false; // 20*50msec = 1sec
  }
}


void move_pingpong_robot(byte gear_flag, int distance, Stream &cubeSerial){ // distance : cm unit
	if(gear_flag > 0){ //Geared-wheel type : AutoCar, BattleBot, AntBot
		int length = distance * 44;
		length = length/10;
		unsigned int d_time = abs(distance) * 120;
		if(gear_flag==1) send_multi_cube_angle(2, -1*length, length, 0, 0, cubeSerial);
		else send_multi_cube_angle(3, 0, -1*length, length, 0, cubeSerial);
		delay(d_time);
	}else{ //Not geared-wheel type : Drawing bot
		int length = distance * 176;
		length = length/10;
		unsigned int d_time = abs(distance) * 480;
		send_multi_cube_angle(3, 0, -1*length, length, 0, cubeSerial);
		delay(d_time);
	}
}

void rotate_pingpong_robot(byte gear_flag, int angle, Stream &cubeSerial){ // distance : degree unit
	if(gear_flag > 0){ //Geared-wheel type : AutoCar, BattleBot, AntBot
		int rotation = angle * 41;
		rotation = rotation/100;
		unsigned int d_time = abs(angle) * 11;
		if(gear_flag==1) send_multi_cube_angle(2, -1*rotation, -1*rotation, 0, 0, cubeSerial);
		else send_multi_cube_angle(3, 0, -1*rotation, -1*rotation, 0, cubeSerial);
		delay(d_time);
	}else{ //Not geared-wheel type : Drawing bot
		int rotation = angle * 135;
		rotation = rotation/100;
		unsigned int d_time = abs(angle) * 44;
		send_multi_cube_angle(3, 0, -1*rotation, -1*rotation, 0, cubeSerial);
		delay(d_time);
	}
}

void pen_up(Stream &cubeSerial){
	send_a_cube_angle(0, 90, cubeSerial);
	unsigned int d_time = 90 * 40;
	delay(d_time);
}

void pen_down(Stream &cubeSerial){
	send_a_cube_angle(0, -90, cubeSerial);
	unsigned int d_time = 90 * 40;
	delay(d_time);
}
	
void lever_up(Stream &cubeSerial){
	send_a_cube_angle(0, 90, cubeSerial);
	unsigned int d_time = 90 * 40;
	delay(d_time);
}
	
void lever_down(Stream &cubeSerial){
	send_a_cube_angle(0, -90, cubeSerial);
	unsigned int d_time = 90 * 40;
	delay(d_time);
}
	
void gripper_open(Stream &cubeSerial){
	send_a_cube_angle(0, 180, cubeSerial);
	unsigned int d_time = 180 * 40;
	delay(d_time);
}
	
void gripper_close(Stream &cubeSerial){
	send_a_cube_angle(0, -180, cubeSerial);
	unsigned int d_time = 180 * 40;
	delay(d_time);
}
	


