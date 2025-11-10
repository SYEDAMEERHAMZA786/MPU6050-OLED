#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <U8g2lib.h>

// U8g2 constructor for SSD1306 128x64 I2C OLED
// Use U8G2_SSD1306_128X64_NONAME_F_HW_I2C for hardware I2C
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// MPU6050 object
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  delay(100);

  // Initialize I2C for ESP32 (SDA = 21, SCL = 22)
  Wire.begin(21, 22);
  Wire.setClock(100000); // Lower I2C clock for stability

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while (1);
  }

  Serial.println("MPU6050 initialized");

  // Initialize OLED
  u8g2.begin();
  u8g2.enableUTF8Print(); // Enable UTF-8 support
}

void loop() {
  // Get new sensor events with the readings
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Print to Serial Monitor
  Serial.print("Accel X: "); Serial.print(a.acceleration.x);
  Serial.print(" Y: "); Serial.print(a.acceleration.y);
  Serial.print(" Z: "); Serial.println(a.acceleration.z);

  Serial.print("Gyro  X: "); Serial.print(g.gyro.x);
  Serial.print(" Y: "); Serial.print(g.gyro.y);
  Serial.print(" Z: "); Serial.println(g.gyro.z);
  Serial.println("---------------------------------");

  // Display on OLED
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr); // Set the font

  // Accelerometer Data
  u8g2.setCursor(0, 10);
  u8g2.print("Accel:");
  u8g2.setCursor(0, 22);
  u8g2.printf("X: %.1f Y: %.1f", a.acceleration.x, a.acceleration.y);
  u8g2.setCursor(0, 32);
  u8g2.printf("Z: %.1f", a.acceleration.z);

  // Gyroscope Data
  u8g2.setCursor(0, 44);
  u8g2.print("Gyro:");
  u8g2.setCursor(0, 56);
  u8g2.printf("X: %.1f Y: %.1f", g.gyro.x, g.gyro.y);
  // Corrected final cursor position (was 64, which might be off-screen)
  u8g2.setCursor(0, 62); 
  u8g2.printf("Z: %.1f", g.gyro.z);

  u8g2.sendBuffer(); // Send the complete buffer to the display

  delay(200); // Adjust refresh rate
}