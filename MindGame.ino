#define BUTTON_RED 2
#define BUTTON_GREEN 3
#define BUTTON_YELLOW 4
#define LED_RED 5
#define LED_GREEN 6
#define LED_YELLOW 7
#define BUZZER 9

int buttonRedValue = 0;
int buttonGreenValue = 0;
int buttonYellowValue = 0;

char mission[] = "RRYNNNNNNNNN";
int answerStep = 0;
char currentAnswer = 'I';

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(BUTTON_RED, INPUT);
  pinMode(BUTTON_GREEN, INPUT);
  pinMode(BUTTON_YELLOW, INPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
}

void buzz(int freq) {
  tone(BUZZER, freq, 200);
}

int getSizeOfMission() {
  int sizeOfMission = 0;
  while (mission[sizeOfMission] != 'N' && sizeOfMission < sizeof(mission) - 1) {
    Serial.println(mission[sizeOfMission]);
    sizeOfMission++;
  }
  Serial.println(sizeOfMission);
  return sizeOfMission;
}

void runMission() {
  Serial.println(sizeof(mission));
  delay(500);

  for (int i = 0; i < sizeof(mission) - 1; i++) {
    Serial.println(mission[i]);
    char color = mission[i];
    switch (color) {
      case 'R':
        buzz(1000);
        digitalWrite(LED_RED, HIGH);
        delay(400);
        digitalWrite(LED_RED, LOW);
        delay(400);
        break;
      case 'Y':
        buzz(2000);
        digitalWrite(LED_YELLOW, HIGH);
        delay(400);
        digitalWrite(LED_YELLOW, LOW);
        delay(400);
        break;
      case 'G':
        buzz(3000);
        digitalWrite(LED_GREEN, HIGH);
        delay(400);
        digitalWrite(LED_GREEN, LOW);
        delay(400);
        break;
    }
  }
}

void welcomeLights() {
  digitalWrite(LED_RED, HIGH);
  delay(50);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  delay(50);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, HIGH);
  delay(50);
  digitalWrite(LED_YELLOW, LOW);
  delay(50);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_GREEN, LOW);
  delay(50);
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, HIGH);
    delay(400);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    delay(200);
  }
}

void resetTheGame() {
  welcomeLights();
  answerStep = 0;
  runMission();
}

void wrongBuzzer() {
  tone(BUZZER, 100);
  delay(300);
  noTone(BUZZER);
  delay(300);
  tone(BUZZER, 100);
  delay(300);
  noTone(BUZZER);
}

void correctBuzzer() {
  tone(BUZZER, 5000);
  delay(200);
  noTone(BUZZER);
  delay(100);
  tone(BUZZER, 7000);
  delay(200);
  noTone(BUZZER);
  delay(100);
  tone(BUZZER, 5000);
  delay(200);
  noTone(BUZZER);
}

void wrongAnswer() {
  wrongBuzzer();
  resetTheGame();
}

void correctAnswer() {
  correctBuzzer();
  resetTheGame();
}

void loop() {
  if (millis() < 200) {
    // program stated
    welcomeLights();
    runMission();
  }

  // turn off all lights
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);


  // put your main code here, to run repeatedly:
  buttonRedValue = digitalRead(BUTTON_RED);
  buttonGreenValue = digitalRead(BUTTON_GREEN);
  buttonYellowValue = digitalRead(BUTTON_YELLOW);

  if (buttonRedValue == 1) {
    digitalWrite(LED_RED, HIGH);
    buzz(1000);
    currentAnswer = 'R';
  } else if (buttonGreenValue == 1) {
    digitalWrite(LED_GREEN, HIGH);
    buzz(2000);
    currentAnswer = 'G';
  } else if (buttonYellowValue == 1) {
    digitalWrite(LED_YELLOW, HIGH);
    buzz(3000);
    currentAnswer = 'Y';
  } else {
    currentAnswer = 'I';
  }

  if (currentAnswer == mission[answerStep]) {
    Serial.println("Correct Answer");
    Serial.println(answerStep);
    Serial.println(sizeof(mission));
    if (sizeof(mission) == answerStep + 2 || mission[answerStep + 1] == 'N') {
      Serial.println("You have done it");
      correctAnswer();
    } else {
      answerStep++;
    }
  } else if (currentAnswer != 'I') {
    Serial.println("Wrong Answer");
    wrongAnswer();
    resetTheGame();
  }


  Serial.println(currentAnswer);
  delay(200);
}
