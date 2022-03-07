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

char mission[] = "RRYNNNNNNNNNNNNN";
int answerStep = 0;
char currentAnswer = 'I';
int level = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  pinMode(BUTTON_RED, INPUT);
  pinMode(BUTTON_GREEN, INPUT);
  pinMode(BUTTON_YELLOW, INPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  regenerateMission();
}

void buzz(int freq) {
  tone(BUZZER, freq, 200);
}

void regenerateMission() {
  for (int i = 0; i < sizeof(mission) - 1 && i < level + 2; i++) {
    int colorNum = random(1, 4);
    Serial.println(colorNum);
    if (colorNum == 1) {
      mission[i] = 'R';
    }
    if (colorNum == 2) {
      mission[i] = 'G';
    }
    if (colorNum == 3) {
      mission[i] = 'Y';
    }
  }
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
  delay(400);
  tone(BUZZER, 100);
  delay(300);
  noTone(BUZZER);
  delay(300);
  tone(BUZZER, 100);
  delay(300);
  noTone(BUZZER);
}

void correctBuzzer() {
  delay(400);
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
  mission[3] = 'N';
}

void correctAnswer() {
  correctBuzzer();
  regenerateMission();
  resetTheGame();
  level++;
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
    buzz(3000);
    currentAnswer = 'G';
  } else if (buttonYellowValue == 1) {
    digitalWrite(LED_YELLOW, HIGH);
    buzz(2000);
    currentAnswer = 'Y';
  } else {
    currentAnswer = 'I';
  }

  if (currentAnswer == mission[answerStep]) {
    Serial.println("Correct Answer");
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
  }


  delay(200);
}
