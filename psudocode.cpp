// === Pin Definitions ===
const int coolant_pump1        = 22;  // heater쪽 펌프
const int coolant_pump2        = 23;  // sea쪽 펌프
const int coolant_heater       = 24;
const int coolant_cbv1         = 25;  // heater쪽 밸브
const int coolant_cbv2         = 26;  // sea쪽 밸브
const int coolant_temperature  = A0;  // 온도 센서

const int hydrogen_valve1      = 28;
const int hydrogen_drain_valve = 29;

const int oxygen_compressor    = 30;
const int oxygen_drain_valve   = 31;

unsigned long ox_drain_time = 0;
bool ox_start_drain = true;

unsigned long hy_drain_time = 0;
bool hy_start_drain = true;

// === Setup ===
void setup() {
  waitForStart();
  
  pinMode(coolant_pump1, OUTPUT);
  pinMode(coolant_pump2, OUTPUT);
  pinMode(coolant_heater, OUTPUT);
  pinMode(coolant_cbv1, OUTPUT);
  pinMode(coolant_cbv2, OUTPUT);

  pinMode(hydrogen_valve1, OUTPUT);
  pinMode(hydrogen_drain_valve, OUTPUT);

  pinMode(oxygen_compressor, OUTPUT);
  pinMode(oxygen_drain_valve, OUTPUT);

  
  initialize_mode();
}

// === Main Loop ===
void loop() {
  // 온도 측정 (센서 특성에 따라 변환 필요)
  float temp = analogRead(coolant_temperature) * (5.0 / 1023.0) * 100.0;

  // 냉각 모드 전환
  if (temp < 70) {
    coolant_loop3();
  } else if (temp < 80) {
    coolant_loop2();
  } else {
    coolant_loop1();
  }

  // 180초에 10초씩 밸브
  if (millis() - ox_drain_time >= 180000) {
    if (ox_start_drain) {
      oxygen_solenoid_drain_On();
      ox_start_drain = false;
    if (millis() - ox_drain_time > 190000) {  
      oxygen_solenoid_drain_Off();
    }
  }

  if (millis() - hy_drain_time >= 60000) {
    if (hy_start_drain) {
      hydrogen_solenoid_drain_On();
      hy_start_drain = false;
    if (millis() - hy_drain_time > 63000) {  
      hydrogen_solenoid_drain_Off();
    }
  }

  delay(500);
}

void waitForStart() {
  Serial.println("If you want to start, please press 'S'!!");
  while (!systemStarted) {
    if (Serial.available()) {
      char command = Serial.read();
      if (command == 'S' || command == 's') {
        systemStarted = true;
        Serial.println("Start!");
      }
    }
    delay(100);
  }
}

        

// === Initialization ===
void initialize_mode() {
  hydrogen_on();
  oxygen_on();
  coolant_loop3(); // 기본 히터 모드
}

// === Coolant Control ===
void coolant_loop1() {
  digitalWrite(coolant_heater, LOW);
  digitalWrite(coolant_cbv1, LOW);
  digitalWrite(coolant_cbv2, HIGH);
  digitalWrite(coolant_pump1, LOW);
  digitalWrite(coolant_pump2, HIGH);
}

void coolant_loop2() {
  digitalWrite(coolant_heater, LOW);
  digitalWrite(coolant_cbv1, LOW);
  digitalWrite(coolant_cbv2, LOW);
  digitalWrite(coolant_pump1, HIGH);
  digitalWrite(coolant_pump2, LOW);
}

void coolant_loop3() {
  digitalWrite(coolant_heater, HIGH);
  digitalWrite(coolant_cbv1, HIGH);
  digitalWrite(coolant_cbv2, LOW);
  digitalWrite(coolant_pump1, HIGH);
  digitalWrite(coolant_pump2, LOW);
}

// === Subsystem Control ===
void hydrogen_on() {
  digitalWrite(hydrogen_valve1, HIGH);
}

void hydrogen_off() {
  digitalWrite(hydrogen_valve1, LOW);
}

void oxygen_on() {
  digitalWrite(oxygen_compressor, HIGH);
}

void oxygen_off() {
  digitalWrite(oxygen_compressor, LOW);
}

// === Drain Control ===
void oxygen_solenoid_drain_On() {
  digitalWrite(oxygen_drain_valve, HIGH);
  }
}
  
void oxygen_solenoid_drain_Off() {
  digitalWrite(oxygen_drain_valve, LOW);
  ox_drain_time = millis();
  ox_start_drain = true;
  }
}


void hydrogen_solenoid_drain_On() {
  digitalWrite(hydrogen_drain_valve, HIGH);
  }
}
  
void hydrogen_solenoid_drain_Off() {
  digitalWrite(hydrogen_drain_valve, LOW);
  hy_drain_time = millis();
  hy_start_drain = true;
  }
}
