#include <PS2X_lib.h>
#include <Wire.h> 
#include <Adafruit_PWMServoDriver.h> 

#define PS2_DAT 12 // MISO
#define PS2_CMD 13 // MOSI
#define PS2_SEL 15 // SS
#define PS2_CLK 14 // SLK

#define pressures false
#define rumble false

PS2X ps2x;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int LX, LY, RX, RY;

void setup() {
  Serial.begin(115200);
  pwm.begin(); 
  pwm.setOscillatorFrequency(27000000); 
  pwm.setPWMFreq(50);
  Wire.setClock(400000); 
  
  int error = -1;
  for (int i = 0; i < 10; i++) {
    delay(1000); 
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
    if (error == 0) break; 
  }

  if (error == 0) {
    Serial.println("Thành công");
  } else {
    Serial.println("Thất bại");
  }
}

void loop() {
  ps2x.read_gamepad(false, false); //đọc trạng thái các nút điều khiển trên bộ Controller PS2

  // Điều khiển servo mở/đóng cửa sau
  if (ps2x.Button(PSB_PAD_UP)) pwm.setPWM(2, 0, 400);
  if (ps2x.Button(PSB_PAD_DOWN)) pwm.setPWM(2, 0, 0);

  // Điều khiển servo nâng/hạ ống shooter
  if (ps2x.Button(PSB_PAD_LEFT)) pwm.setPWM(3, 0, 400);
  if (ps2x.Button(PSB_PAD_RIGHT)) pwm.setPWM(3, 0, 0);

  // Bật/tắt động cơ thu bóng
  if (ps2x.Button(PSB_L1)) {
    pwm.setPWM(10, 0, 4096);
    pwm.setPWM(11, 0, 0);
  }
  if (ps2x.Button(PSB_L2)) {
    pwm.setPWM(10, 0, 0);
    pwm.setPWM(11, 0, 0);
  }

  // Bật/tắt động cơ shooter
  if (ps2x.Button(PSB_R1)) {
    pwm.setPWM(12, 0, 4096);
    pwm.setPWM(13, 0, 0);
  }
  if (ps2x.Button(PSB_R2)) {
    pwm.setPWM(12, 0, 0);
    pwm.setPWM(13, 0, 0);
  }

  // Điều khiển ROBOT với Joystick  
  // Quy đổi giá trị của joystick sang xung PWM
  int speed = 0;
  LX = ps2x.Analog(PSS_LX);
  LY = ps2x.Analog(PSS_LY);
  RX = ps2x.Analog(PSS_RX);
  RY = ps2x.Analog(PSS_RY);

  // joystick trái để tiến/lùi
  if (LY < 128) {
    speed = map(LX, 128, 0, 0, 4096);
    pwm.setPWM(8, 0, 0);
    pwm.setPWM(9, 0, speed);
    pwm.setPWM(14, 0, 0);
    pwm.setPWM(15, 0, speed);
  } else if (LY > 128) {
    speed = map(LY, 128, 255, 0, 4096);
    pwm.setPWM(8, 0, speed);
    pwm.setPWM(9, 0, 0);
    pwm.setPWM(14, 0, speed);
    pwm.setPWM(15, 0, 0);
  } else {
    pwm.setPWM(8, 0, 0);
    pwm.setPWM(9, 0, 0);
    pwm.setPWM(14, 0, 0);
    pwm.setPWM(15, 0, 0);
  }

  // joystick phải để chuyển hướng rẽ trái/phải
  if (RX < 128) {
    speed = map(RX, 128, 0, 0, 4096);
    pwm.setPWM(8, 0, 0);
    pwm.setPWM(9, 0, 0);
    pwm.setPWM(14, 0, 0);
    pwm.setPWM(15, 0, speed);
  } else if (RX > 128) {
    speed = map(RX, 128, 255, 0, 4096);
    pwm.setPWM(8, 0, speed);
    pwm.setPWM(9, 0, 0);
    pwm.setPWM(14, 0, speed);
    pwm.setPWM(15, 0, 0);
  } else {
    pwm.setPWM(8, 0, 0);
    pwm.setPWM(9, 0, 0);
    pwm.setPWM(14, 0, 0);
    pwm.setPWM(15, 0, 0);
  }
}
