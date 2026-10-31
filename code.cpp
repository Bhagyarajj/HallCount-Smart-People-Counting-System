
#define PIR_A 2
#define PIR_B 3

int insideCount = 0;
int entryCount = 0;
int exitCount = 0;

int previousA = LOW;
int previousB = LOW;

unsigned long firstDetectionTime = 0;
unsigned long detectionWindow = 1500;

int firstSensor = 0;

void setup() {
  Serial.begin(9600);

  pinMode(PIR_A, INPUT);
  pinMode(PIR_B, INPUT);

  Serial.println("Hall Person Counter");
  Serial.println("-------------------");
  delay(30000); // PIR sensor warm-up
}

void loop() {

  int sensorA = digitalRead(PIR_A);
  int sensorB = digitalRead(PIR_B);

  unsigned long currentTime = millis();

  // Detect first sensor activation
  if (firstSensor == 0) {

    if (sensorA == HIGH && previousA == LOW) {
      firstSensor = 1;
      firstDetectionTime = currentTime;
    }

    else if (sensorB == HIGH && previousB == LOW) {
      firstSensor = 2;
      firstDetectionTime = currentTime;
    }
  }

  // Check second sensor within detection window
  else {

    if (currentTime - firstDetectionTime <= detectionWindow) {

      if (firstSensor == 1 && sensorB == HIGH) {

        // A followed by B: Entering
        entryCount++;
        insideCount++;

        Serial.println("Person ENTERED");
        printCounts();

        firstSensor = 0;
      }

      else if (firstSensor == 2 && sensorA == HIGH) {

        // B followed by A: Exiting
        exitCount++;

        if (insideCount > 0) {
          insideCount--;
        }

        Serial.println("Person EXITED");
        printCounts();

        firstSensor = 0;
      }
    }

    else {
      // Reset if no second sensor is detected
      firstSensor = 0;
    }
  }

  previousA = sensorA;
  previousB = sensorB;

  delay(20);
}

void printCounts() {
  Serial.print("Total Entered: ");
  Serial.println(entryCount);

  Serial.print("Total Exited: ");
  Serial.println(exitCount);

  Serial.print("Persons Inside: ");
  Serial.println(insideCount);

  Serial.println("-------------------");
}
