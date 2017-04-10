#include <ESPHelper.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define OLED_RESET LED_BUILTIN // 4
Adafruit_SSD1306 display(OLED_RESET);

#define PIN 10

netInfo homeNet = {.name = "HOME NETWORK", .mqtt = "your_mqtt_server", .mqtt_user = "mqtt-user", .mqtt_pass = "mqtt-password", .ssid = "your-network-id", .pass = "Your_network_pw"};

ESPHelper myESP(&homeNet);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(150, PIN, NEO_GRB + NEO_KHZ800);

long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Setting up... wait..");
  display.display();

  strip.begin();
  strip.show();

  myESP.OTA_enable();
  myESP.OTA_setPassword("password_for_wireless_updates");

  myESP.addSubscription("stripe/color");
  myESP.begin();
  myESP.setCallback(callback);

  display.clearDisplay();
  display.println("Initialisation finished");
  display.display();
}

void callback(char* topic, byte* payload, unsigned int length) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Message arrived [");
  display.println(topic);
  display.print("] ");
  String inString = "";
  for (int i = 0; i < length; i++) {
    display.print((char)payload[i]);
    inString += (char)payload[i];
  }
  display.println();
  display.println("WiFi connected");
  display.println("IP address: ");
  display.println(myESP.getIP());
  display.display();
  colorWipe(inString.toInt(), 50);;
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}


void loop() {
  myESP.loop();  //run the loop() method as often as possible - this keeps the network services running
  //Put application code here
  yield();
}

