#define PHOTOCOUPLER_PIN 2  // 포토커플러 출력 신호를 받을 핀 (디지털 핀 2번)
#define LED_PIN 13          // 아두이노 내장 LED 핀 (일반적으로 핀 13)
#define MOTOR_PWM_PIN 9     // DC 모터 제어를 위한 PWM 핀
#define MOTOR_DIR_PIN 8     // DC 모터 방향 제어를 위한 핀

volatile bool systemActive = true;  // 시스템 상태를 저장하는 변수 (모터와 LED 상태)
int pwmValue = 50;  // 고정된 PWM 값 (DC 모터 속도 제어, 0 ~ 255)

// 인터럽트 핸들러 함수: 포토커플러 신호가 HIGH일 때 호출
void handleInterrupt() {
  systemActive = !systemActive;  // 시스템 상태 반전
  if (systemActive) {
    digitalWrite(LED_PIN, HIGH);       // LED 켜기
    analogWrite(MOTOR_PWM_PIN, pwmValue);  // 모터 재시작 (PWM 신호 송출)
    digitalWrite(MOTOR_DIR_PIN, HIGH);  // 모터 방향 설정
    Serial.println("시스템 재시작: LED 켜짐, 모터 동작 중");
  } else {
    digitalWrite(LED_PIN, LOW);        // LED 끄기
    analogWrite(MOTOR_PWM_PIN, 0);     // 모터 정지
    Serial.println("시스템 중지: LED 꺼짐, 모터 정지");
  }
}

void setup() {
  pinMode(PHOTOCOUPLER_PIN, INPUT_PULLUP);  // 포토커플러 출력 신호 핀 (풀업 저항 사용)
  pinMode(LED_PIN, OUTPUT);                 // 내장 LED 핀을 출력으로 설정
  pinMode(MOTOR_PWM_PIN, OUTPUT);           // 모터 제어를 위한 PWM 핀 설정
  pinMode(MOTOR_DIR_PIN, OUTPUT);           // 모터 방향 제어 핀 설정

  // 포토커플러에서 신호가 HIGH(즉, 신호가 들어오는 순간)일 때 인터럽트 발생
  attachInterrupt(digitalPinToInterrupt(PHOTOCOUPLER_PIN), handleInterrupt, HIGH);

  // 시리얼 모니터를 위한 초기화 (디버깅용)
  Serial.begin(115200);
  Serial.println("시스템 대기 중...");

  // 시스템 초기 상태: 모터 동작 및 LED 켜기
  digitalWrite(LED_PIN, HIGH);         // LED 켜기
  analogWrite(MOTOR_PWM_PIN, pwmValue);  // 모터 구동
  digitalWrite(MOTOR_DIR_PIN, HIGH);   // 모터 방향 설정
}

void loop() {
  // loop는 비어있어도 됩니다. 모든 작업은 인터럽트에서 처리됨.
}
