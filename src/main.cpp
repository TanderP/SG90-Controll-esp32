#include <Servo.h>
#include <TFT_eSPI.h>
#define button1 25
#define button2 32
#define button3 33
#define button4 26

const int servoPin1 = 27;

double speed1 = 10;

uint8_t pos1 = 0;
int delay1 = 100;



Servo myservo = Servo();

TFT_eSPI tft= TFT_eSPI();

TFT_eSprite sprite = TFT_eSprite(&tft);

void Speed(void *pvParameters); 
void Position(void *pvParameters);
void servo (void *pvParameters);
void Display(void *pvParameters);

void setup() {
  Serial.begin(115200);
  pinMode(button1, INPUT_PULLDOWN);
  pinMode(button2, INPUT_PULLDOWN);
  pinMode(button3, INPUT_PULLDOWN);
  pinMode(button4, INPUT_PULLDOWN);

  tft.init();
  tft.setRotation(2);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);

  xTaskCreate(Speed, "Input", 10000, NULL, 1, NULL);
  xTaskCreate(Position, "Position", 10000, NULL, 1, NULL);
  xTaskCreate(servo, "servo", 10000, NULL, 1, NULL);

  xTaskCreate(Display, "Display", 10000, NULL, 1, NULL);

}
void servo (void *pvParameters){
  while(1){
myservo.write(servoPin1, pos1);
     delay(6);
  }
}
void Speed(void *pvParameters){
  while(1){
    if(digitalRead(button3) == HIGH){
      speed1 = speed1 + 1;
      if (speed1 > 180){
        speed1 = 180;
      }// limit up
      delay(100);
    }
    if(digitalRead(button4) == HIGH){
      speed1 = speed1 - 1;
      if (speed1 <= 0){
        speed1 = 0;
      } //limit bot
      delay(100);
    }
    if (speed1 <5){
      delay1 = 30;
    } 
    else if (speed1 >=5 && speed1 <11){
      delay1 = 35;
    }
    else if(speed1>10 && speed1<20){
      delay1 = 50;
      }
    else {delay1 = 55;}

    delay(100);
  }
}

void Position(void *pvParameters){
  while(1){
      if(digitalRead(button1) == HIGH){
      pos1 = pos1 + speed1;
      if (pos1 >= 180){
        pos1 = 0; // if its hit 180 more then its reset to 0
      }
      delay(25);
  }
  if(digitalRead(button2) == HIGH){
      pos1 = pos1 - speed1;
      if (pos1 <= 0){
        pos1 = 180;
        
      } // if its hit 0 then its reset to 180
      delay(25);
  }

  delay(delay1);
}
}

void Display(void *pvParameters){
  while(1){
    // show on display
    sprite.setColorDepth(8);
    sprite.createSprite(240, 135);
    sprite.setTextSize(2);
    sprite.setTextColor(TFT_WHITE, TFT_BLACK);
    sprite.setCursor(50, 20);
    sprite.println("Speed");
    sprite.setCursor(50, 50);
    sprite.println(speed1);
    sprite.setCursor(50, 70);
    sprite.println("Position");
    sprite.setCursor(50, 90);
    sprite.println(pos1);
    sprite.pushSprite(50, 0);
    sprite.deleteSprite();
    delay(100);
  }
}
void loop() {
  // serial print
  Serial.println("LOOP");
  Serial.printf("Speed= %.1f \n",speed1);
  Serial.printf("Position= %d \n",pos1);
  Serial.printf("Delay= %d \n",delay1);
  delay(500);

}