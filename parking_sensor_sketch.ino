#include <FastLED.h>

// ——— LED CONFIGURATION ———
// This is tuned for 60 leds
#define LED_PIN           7      // GPIO pin for LED strip data line
#define NUM_LEDS          60     // Total number of LEDs on the strip
#define BRIGHTNESS        255    // LED brightness (0-255)
#define LEDS_PER_TRIPLET  3      // Number of LEDs grouped together
#define MAX_TRIPLETS      10     // Maximum number of triplets (30 LEDs per side)


// ——— DISTANCE SENSOR CONFIGURATION ———
#define ULTRASONIC_TRIG_PIN    5
#define ULTRASONIC_ECHO_PIN    12
#define SOUND_SPEED_CM_US      0.0343  // Speed of sound in cm/microsecond

// ——— PROXIMITY THRESHOLDS ———
#define DANGER_DISTANCE_CM     20      // Very close - all red
#define MAX_DETECTION_CM       200     // Maximum useful detection range
#define DISTANCE_PER_LEVEL     20      // Distance range per LED level
#define COLOR_CHANGE_LEVEL     3       // Level where color changes from red to white

// ——— TIMING ———
#define LOOP_DELAY_MS         100
#define STARTUP_BLINK_MS      500


CRGB ledStrip[NUM_LEDS];

/**
* Lights up a triplet of LEDs symmetrically from both ends of the strip
* @param tripletIndex: Which triplet to control (0-9)
* @param color: Color to set the triplet to
*/
void setSymmetricalTriplet(int tripletIndex, CRGB color) {
 int startIndex = tripletIndex * LEDS_PER_TRIPLET;
 
 // Light up triplet from the beginning of strip
 ledStrip[startIndex] = color;
 ledStrip[startIndex + 1] = color;
 ledStrip[startIndex + 2] = color;
 
 // Light up corresponding triplet from the end of strip (mirrored)
 ledStrip[NUM_LEDS - (startIndex + 1)] = color;
 ledStrip[NUM_LEDS - (startIndex + 2)] = color;
 ledStrip[NUM_LEDS - (startIndex + 3)] = color;
}

/**
* Sets the progress bar display from both ends toward center
* @param progressLevel: How many triplets to light up (1-10)
* @param color: Color for the lit triplets
*/
void setProximityDisplay(uint8_t progressLevel, CRGB color) {
 for(int triplet = 0; triplet < MAX_TRIPLETS; triplet++) {
   if(triplet < progressLevel) {
     setSymmetricalTriplet(triplet, color);
   } else {
     setSymmetricalTriplet(triplet, CRGB::Black);  // Turn off unused triplets
   }
 }
}

/**
* Measures distance using HC-SR04 ultrasonic sensor
* @return: Distance in centimeters
*/
float measureDistanceCM() {
 // Send ultrasonic pulse
 digitalWrite(ULTRASONIC_TRIG_PIN, LOW);  
 delayMicroseconds(2);  
 digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);  
 delayMicroseconds(10);  
 digitalWrite(ULTRASONIC_TRIG_PIN, LOW);  

 // Measure echo duration
 float pulseDuration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);

 // Convert duration to distance: (time * speed_of_sound) / 2
 return (pulseDuration * SOUND_SPEED_CM_US) / 2;
}

/**
* Startup indicator - blinks all LEDs red
*/
void startupBlink() {
 setProximityDisplay(MAX_TRIPLETS, CRGB::Red);
 FastLED.show();
 delay(STARTUP_BLINK_MS);
 setProximityDisplay(0, CRGB::Black);
 FastLED.show();
}

void setup() {
//  Serial.begin(115200);
 
 // Initialize LED strip
 FastLED.addLeds<WS2812B, LED_PIN, GRB>(ledStrip, NUM_LEDS);
 FastLED.setBrightness(BRIGHTNESS);
 
 delay(500);
 
 // Initialize ultrasonic sensor pins
 pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);  
 pinMode(ULTRASONIC_ECHO_PIN, INPUT);  

 // Signal successful startup
 startupBlink();
}

void loop() {
 float currentDistance = measureDistanceCM();
 
 // DANGER ZONE: Object very close (≤20cm) - All LEDs red
 if(currentDistance <= DANGER_DISTANCE_CM) {
   setProximityDisplay(MAX_TRIPLETS, CRGB::Red);
 } 
 // PROXIMITY ZONE: Object in detection range (20-200cm)
 else if(currentDistance > DANGER_DISTANCE_CM && currentDistance < MAX_DETECTION_CM) {
   // Calculate how many LED levels to show based on distance
   // Closer objects = more LEDs lit
   uint8_t distanceLevel = (uint8_t)(currentDistance / DISTANCE_PER_LEVEL);
   uint8_t ledsToLight = MAX_TRIPLETS - distanceLevel;
   
   // Choose color: Red for close objects, White for moderate distance
   CRGB proximityColor = (distanceLevel >= COLOR_CHANGE_LEVEL) ? CRGB::White : CRGB::Red;
   
   setProximityDisplay(ledsToLight, proximityColor);
 } 
 // SAFE ZONE: Object far away or out of range (>200cm) - Single green indicator
 else {
   setProximityDisplay(1, CRGB::Green);
 }
 
 // Update the LED display
 FastLED.show();
 delay(LOOP_DELAY_MS);
}
