/**
 * A basic example of using the EncoderButton library with ESP32.
 */

//#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <EncoderButton.h>

/**
 * Instatiate an EncoderButton.
 * For ESP32, you can use any GPIO pin for hardware interrupts (within the ESP32 pin restrictions).
 * Probably better to pick a more meaningful name than 'eb1'...
 * Encoder+button:
 * EncoderButton(byte encoderPin1, byte encoderPin2, byte switchPin);
 * Encoder only:
 * EncoderButton(byte encoderPin1, byte encoderPin2);
 * Button only:
 * EncoderButton(byte switchPin);
 */

EncoderButton eb1(33, 35, 26);

int myValue = 0;
bool accel = false;

/**
 * A function to handle the 'clicked' event
 * Can be called anything but requires EncoderButton& 
 * as its only parameter.
 * I tend to prefix with 'on' and suffix with the 
 * event type.
 */
void onEb1Clicked(EncoderButton& eb) {
  Serial.print("eb1 Clicked - Count: ");
  Serial.println(eb.clickCount());
   eb1.resetPosition(0);
}

/**
 * A function to handle the 'double clicked' event
 * as its only parameter.
 */
void onEb1DblClick(EncoderButton& eb) {
  Serial.print("eb1 Double - Count: ");
  Serial.println(eb.clickCount());
  eb1.resetPosition(myValue);
}
/**
 * A function to handle the 'long press' event
 * This enables acceleration in this example.
 */
void onEb1LongPress(EncoderButton& eb) {
  Serial.print("eb1 Long - Count: ");
  Serial.println(eb.clickCount());
  accel = !accel;
}

/**
 * A function to handle the 'encoder' event
 */
void onEb1Encoder(EncoderButton& eb) {
  digitalWrite(LED_BUILTIN, HIGH);
  if (!accel){
    myValue += eb.increment();
    Serial.print("eb1 incremented by: ");
    Serial.println(eb.increment());
    Serial.print("eb1 position is: ");
    Serial.print(eb.position());    
    Serial.print(" myValue is now: ");
    Serial.println(myValue);
  }
  else {
    myValue += eb.increment()*abs(eb.increment());
    Serial.print("eb1 incremented by: ");
    Serial.print(eb.increment());
    Serial.print(" accelerated to: ");
    Serial.print(eb.increment()*abs(eb.increment()));
    Serial.print(" myValue is now: ");
    Serial.println(myValue);
  }
  delay(50); // This is here to allow you to see the LED blink - remove if not using the LED. 
  digitalWrite(LED_BUILTIN, LOW);
}

/**
 * Specifiy ISR to handle the 'encoder' event
 * This is required 1 per encoder
 */

void IRAM_ATTR readEncoderISR() {
  eb1.EncoderISR();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);
  Serial.println("EncoderButton ESP32 Example");

  /**
   * Call Begin and Setup encoder to attach 
   * the interrupts
   */
  eb1.Encoderbegin();
  eb1.Encodersetup(readEncoderISR);

  //Link the event(s) to your function(s)
  eb1.setClickHandler(onEb1Clicked);
  eb1.setLongPressHandler(onEb1LongPress);
  eb1.setDoubleClickHandler(onEb1DblClick);
  eb1.setEncoderHandler(onEb1Encoder);
  //Set the rate limit on your encoder.
  eb1.setRateLimit(200);
  //If set to 200 milliseconds, the increment() will be > 1 if the encoder rotation clicks more than 5 time/second

  pinMode(LED_BUILTIN, OUTPUT);
}
void loop() {
  // put your main code here, to run repeatedly:
  // You must call update() for every defined EncoderButton.
  // This will update the state of the encoder & button and 
  // fire the appropriat events.
  eb1.update();
}