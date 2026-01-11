#include <Servo.h>
#include <ESP8266WiFi.h>

// Servo Setup
Servo myservo;
const int servoPin = D4; // GPIO2 (D4) for Servo motor

// Ultrasonic Sensor Setup
const int trigPin = D1;  // GPIO5 (D1) for TRIG pin of Ultrasonic sensor
const int echoPin = D2;  // GPIO4 (D2) for ECHO pin of Ultrasonic sensor

// IR Sensor and Built-in LED
const int irSensorPin = D5;   // GPIO14 (D5) for IR sensor
const int ledPin = LED_BUILTIN; // Built-in LED (GPIO2)

int sensorValue = 0;          // Variable to store IR sensor state
long distance;                // Variable to store Ultrasonic distance

// Wi-Fi Setup
const char *ssid = "AndroidAP216F";       // Replace with your WiFi SSID
const char *password = "012345678"; // Replace with your WiFi password

// Initialize WiFi connection
void setupWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
}

// Function to initialize the Ultrasonic Sensor
void setupUltrasonic() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

// Function to get distance from Ultrasonic Sensor
long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

// Function to control the Servo (lid operation)
void lid(){
myservo.write(180);  // Send a high signal to move forward
  delay(300);         // Wait for 5 seconds

  // Stop the motor
  myservo.write(90);   // Send the stop signal (90 degrees is usually neutral)
  delay(5000);         // Pause for 2 seconds

  // Rotate backward (e.g., full speed in the opposite direction)
  myservo.write(0);    // Send a low signal to move backward
  delay(300);         // Wait for 5 seconds

  // Stop the motor again
  myservo.write(90);   // Stop signal
  delay(5000); 
}


// Function to handle the sequence: Open Lid -> Close Lid -> Call Ultrasonic
void handleLidAndUltrasonic() {
  // Open and close the lid
  lid();
  
  // Call ultrasonic sensor function after lid operation
  long distance = getDistance();
  Serial.print("Ultrasonic Sensor: Distance = ");
  Serial.print(distance);
  Serial.println(" cm");
}

void setup() {
  // Begin Serial communication for debugging
  Serial.begin(115200);

  // Initialize WiFi connection
  setupWiFi();

  // Set up Ultrasonic sensor
  setupUltrasonic();

  // Set up Servo
  myservo.attach(servoPin);

  // Set up IR sensor and LED
  pinMode(irSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);

  Serial.println("Setup complete.");
}

void loop() {
  // Read IR sensor state
  sensorValue = digitalRead(irSensorPin);
  
  // Print raw sensor value for debugging
  Serial.print("No Object Detected ");
  // Print raw sensor value (0 or 1)

  if (sensorValue == LOW) { // Adjust based on your sensor's behavior
    Serial.println("IR Sensor: Object detected!");
    digitalWrite(ledPin, LOW);  // Turn on the LED (active LOW)
    
    // Open lid and call ultrasonic sensor function
    handleLidAndUltrasonic();
    
    delay(5000);  // Wait a few seconds before next detection
  } else {
    digitalWrite(ledPin, HIGH);  // Turn off the LED
  }

  delay(500);  // Small delay to stabilize readings
}
