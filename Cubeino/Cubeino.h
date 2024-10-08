
#ifndef Cubeino_H
#define Cubeino_H


#define GCUBE_GET_BOARD_ID 0x10 // 보드의 ID 값 확인 요청
#define GCUBE_CUBEINO_BOARD_ID 0x70 // 제조사 및 모델마다 고유 아이디 부여함, Cubeino : 0x70
#define GCUBE_HUB_BOARD_ID 0x71 // 큐브에 꽂힌 아두이노(큐브이노)가 Sensor Hub 역할을 하는 경우 보드 ID : 0x71
#define GCUBE_GET_CUBE_NUMBER 0x21 // 연결된 큐브 개수 요청
#define GCUBE_SET_ALL_CUBES_CONNECTED 0x2A // 모든 큐브 연결됨


#define GCUBE_REQ_ANALOG_READ 0x11
#define GCUBE_REQ_DIGITAL_SET 0x1A // 아두이노의 디지털 포트 명령 수신

//GCube motor & servo & dot-matrix control
#define GCUBE_SET_A_MOTOR_SPEED 0x30
#define GCUBE_SET_MULTI_MOTOR_SPEED 0x31
#define GCUBE_SET_A_MOTOR_ANGLE 0x38
#define GCUBE_SET_MULTI_MOTOR_ANGLE 0x39
#define GCUBE_SET_A_SERVO_ANGLE 0x40
#define GCUBE_SET_MULTI_SERVO_ANGLE 0x41
#define GCUBE_SET_DOTMATRIX_IMAGE_DATA 0x51
#define GCUBE_SET_DOTMATRIX_BLINK_RATE 0x5A
#define GCUBE_SET_DOTMATRIX_PIXEL_DATA 0x60
#define GCUBE_SET_DOTMATRIX_STRING_DATA 0x61

//Get GCube sensors
#define GCUBE_GET_CUBE_GYRO_BASE 0xA0 // 큐브의 기울기값(자이로센서) 요청
#define GCUBE_GET_CUBE_PROXI_BASE 0xAA // 큐브의 근접센서 요청
#define GCUBE_GET_CUBE_BUTTON_BASE 0xAB // 큐브의 버튼 상태 요청
#define GCUBE_GET_CUBE_EXT_ANALOG_2BYTE 0xB0 // 큐브의 확장포트 2바이트형 센서 값
#define GCUBE_GET_CUBE_ARDUINO_ANALOG_BASE 0xD0 // 큐브-아두이노의 아날로그값 요청
#define GCUBE_SET_CUBE_ARDUINO_ANALOG_START 0xD8 // 특정 큐브-아두이노의 아날로그 신호 보고 명령 전송
#define GCUBE_SET_CUBE_ARDUINO_DIGITAL_CONTROL 0xD9 // 특정 큐브-아두이노의 디지털 포트 제어 명령 전송

#define GCUBE_GET_CUBE_EXT_COLOR_KEY 0xE0 // 큐브의 확장포트 칼라센서 칼라키 값
#define GCUBE_GET_CUBE_EXT_COLOR_VALUE 0xE8 // 큐브의 확장포트 칼라센서 RGB 값 

#define GCUBE_GET_CUBE_ECHO_BASE 0xFF // 단순 Echo 메시지 받는 기능

//GCube external sensor name
#define GCUBE_EXT_SENSOR_LIGHT 0xC1
#define GCUBE_EXT_SENSOR_SOUND 0xC2
#define GCUBE_EXT_SENSOR_VOLUME 0xC3
#define GCUBE_EXT_SENSOR_MAGNETIC 0xC4
#define GCUBE_EXT_SENSOR_ULTRASONIC 0xC5
#define GCUBE_EXT_SENSOR_TEMPRATURE 0xC6
#define GCUBE_EXT_SENSOR_COLOR_KEY 0xC7
#define GCUBE_EXT_SENSOR_COLOR_VALUE 0xC8
#define GCUBE_EXT_SENSOR_ANALOG 0xF7


//Control PingPong robot model
#define PINGPONG_ANTBOT 2 // Gear가 장착된 자동차형 핑퐁로봇 모델
#define PINGPONG_BATTLEBOT 2 // Gear가 장착된 자동차형 핑퐁로봇 모델
#define PINGPONG_AUTOCAR 1 // Gear가 장착된 자동차형 핑퐁로봇 모델
#define PINGPONG_DRAWINGBOT 0 // Gear가 장착되지 않은 자동차형 핑퐁로봇 모델

//Common util
void send_gcube(byte x0, byte x1, byte x2, byte x3, byte x4, byte x5, byte x6, byte x7, byte x8, byte x9, Stream &cubeSerial);
byte get_iv(byte a);

void get_cube_echo_value(uint8_t mdata[], Stream &cubeSerial);

//Get GCube sensor value 
void get_cube_acc_value(int8_t mdata[], byte c_number, Stream &cubeSerial);
void get_cube_proxi_value(uint8_t mdata[], byte c_number, Stream &cubeSerial);
void get_cube_button_value(uint8_t mdata[], byte c_number, Stream &cubeSerial);
uint16_t get_cube_ext_sensor_value(uint8_t mdata[], byte c_number, byte s_name, Stream &cubeSerial);

//Get analog value or set digital port of GCube-attached Arduino 
void set_cube_arduino_analog_start(byte c_number, Stream &cubeSerial);
uint16_t get_cube_arduino_analog_value(byte c_number, byte a_port, Stream &cubeSerial);
void set_cube_arduino_digital_value(byte c_number,byte port_hi, byte port_lo, Stream &cubeSerial);

//Interface between Arduino and the connected GCube
void wait_for_first_cube_connected(Stream &cubeSerial);
void wait_for_hub_cube_connected(Stream &cubeSerial);
void wait_for_all_cube_connected(byte GCUBE_TOTAL, Stream &cubeSerial);
void get_req_command(uint8_t mdata[], Stream &cubeSerial);
void report_analog_data(Stream &cubeSerial);
void set_digital_port(byte vh, byte vl);

//Control GCube's motor and attached servo-motor or matrix
void send_a_cube_speed(byte cube_number, char cube_speed, Stream &cubeSerial);
//void send_multi_cube_speed(byte GCUBE_TOTAL, char s7, char s6, char s5, char s4, char s3, char s2, char s1, char s0, Stream &cubeSerial);
void send_multi_cube_speed(byte GCUBE_TOTAL, char s0, char s1, char s2, char s3, char s4, char s5, char s6, char s7, Stream &cubeSerial);
void stop_all_cube_motor(byte GCUBE_TOTAL, Stream &cubeSerial);
void send_a_cube_angle(byte cube_number, int cube_angle, Stream &cubeSerial);
//void send_multi_cube_angle(byte cube_range, int s3, int s2, int s1, int s0, Stream &cubeSerial);
void send_multi_cube_angle(byte cube_range, int s0, int s1, int s2, int s3, Stream &cubeSerial);
void send_a_servo_angle(byte cube_number, byte cube_angle, Servo &svo, Stream &cubeSerial);
//void send_multi_servo_angle(byte GCUBE_TOTAL, byte s7, byte s6, byte s5, byte s4, byte s3, byte s2, byte s1, byte s0, Servo &svo, Stream &cubeSerial);
void send_multi_servo_angle(byte GCUBE_TOTAL, byte s0, byte s1, byte s2, byte s3, byte s4, byte s5, byte s6, byte s7, Servo &svo, Stream &cubeSerial);
void send_matrix_still_image(byte cube_index, byte m[], Stream &cubeSerial);
void start_matrix_roll_image(byte GCUBE_TOTAL, byte m[], int duration, Stream &cubeSerial);
void start_matrix_shift_image(byte cubeIndex, byte GCUBE_TOTAL, byte rollImage[], int8_t xp, int8_t yp, Stream &cubeSerial);
void set_matrix_blink_rate(byte cube_index, byte blinkrate, Stream &cubeSerial);
void set_matrix_pixel_data(byte cube_index, byte x, byte y, byte onoff, Stream &cubeSerial);
void set_matrix_string_data(byte cube_index, byte s0, byte s1, byte s2, byte s3, byte s4, byte s5, byte s6, byte s7, Stream &cubeSerial);

//Control PingPong robot model with Arduino
void move_pingpong_robot(byte gear_flag, int distance, Stream &cubeSerial);
void rotate_pingpong_robot(byte gear_flag, int angle, Stream &cubeSerial);
void pen_up(Stream &cubeSerial);
void pen_down(Stream &cubeSerial);
void lever_up(Stream &cubeSerial);
void lever_down(Stream &cubeSerial);
void gripper_open(Stream &cubeSerial);
void gripper_close(Stream &cubeSerial);



#endif
