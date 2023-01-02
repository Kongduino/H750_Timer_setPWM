#define pin PA10 // connect an LED on PA10 + GND

HardwareTimer MyTimE3(TIM6);
uint32_t lastTime;
void OnTimer1Interrupt() {
  digitalWrite(PE3, HIGH);
  Serial.print("Time elapsed: ");
  Serial.println(millis() - lastTime);
  lastTime = millis();
  delay(500);
  digitalWrite(PE3, LOW);
}

void setup() {
  SysClkFullSpeed();
  // SysClkHalfSpeed();
  pinMode(PE3, OUTPUT);
  // Automatically retrieve TIM instance and channel associated to pin
  // This is used to be compatible with all STM32 series automatically.
  TIM_TypeDef *Instance = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(pin), PinMap_PWM);
  uint32_t channel = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(pin), PinMap_PWM));
  // Instantiate HardwareTimer object. Thanks to 'new' instantiation, HardwareTimer is not destructed when setup() function is finished.
  HardwareTimer *MyTim = new HardwareTimer(Instance);
  // Configure and start PWM
  MyTim->setPWM(channel, pin, 1, 50); // 1 Hertz, 50% dutycycle
  // MyTim->setPWM(channel, pin, 1, 50, myCallback); // Add a callback

  MyTimE3.setPrescaleFactor(36818);
  MyTimE3.setOverflow(65536); // 480000000/7234/32768 = 2 Hz. 1 second at full speed, 2 s at half speed
  // 32768 / 65536 = 8.9 s
  // 36818 / 65536 = 10 s
  MyTimE3.attachInterrupt(OnTimer1Interrupt);
  MyTimE3.refresh(); // Make register changes take effect
  MyTimE3.resume(); // Start
  lastTime = millis();
}

void loop() {
  /* Nothing to do all is done by hardware. Even no interrupt required. */
}
