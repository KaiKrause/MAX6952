#include <MAX6952.h>


MAX6952 max6952 = MAX6952(23,18,5,8);

void setup() {
  // put your setup code here, to run once:

  max6952.shutdown(0);
  max6952.setIntensity(15);
  

}

void loop() {
  // put your main code here, to run repeatedly:

  max6952.setText("MAX6952",LEFT);
  delay(1000);
  max6952.setText("MAX6952",CENTER);
  delay(1000);
  max6952.setText("MAX6952",RIGHT);
  delay(1000);

  max6952.setTextMarquee("MAX6952",200,CLASSIC,LEFT_TO_RIGHT);
  delay(1000);
  max6952.setTextMarquee("MAX6952",200,CLASSIC,RIGHT_TO_LEFT);
  delay(1000);

  max6952.setTextMarquee("MAX6952",200,BOUNCE,LEFT_TO_RIGHT);
  delay(1000);
  max6952.setTextMarquee("MAX6952",200,BOUNCE,RIGHT_TO_LEFT);
  delay(1000);

  

}
