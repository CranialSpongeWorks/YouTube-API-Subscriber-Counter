/*******************************************************************
  This sketch is for YouTube API Subscriber Count
/*******************************************************************
	Read YouTube Channel statistics from the YouTube API and print them to the serial monitor

	Compatible Boards:
	* Any ESP8266 board
	* Any ESP32 board

	Written by Brian Lough
	YouTube: https://www.youtube.com/brianlough
 *******************************************************************/
// ----------------------------
// Standard Libraries
// ----------------------------
#include <DigitLedDisplay.h>
#if defined(ESP8266)
	#include <ESP8266WiFi.h>
#elif defined(ESP32)
	#include <WiFi.h>
#endif
#include <WiFiClientSecure.h>

// initialize with pin numbers for data, cs, and clock
DigitLedDisplay leddisplay = DigitLedDisplay(13, 15, 14);

// ----------------------------
// Additional Libraries - each of these will need to be installed
// ----------------------------

// Library for connecting to the YouTube API
// https://github.com/witnessmenow/arduino-youtube-api
// (search for "youtube" in the Arduino Library Manager)
#include <YoutubeApi.h>

// Library used for parsing Json from the API responses
// https://github.com/bblanchon/ArduinoJson
// (search for "Arduino Json" in the Arduino Library Manager)
#include <ArduinoJson.h>

//------- Replace the following! ------
const char ssid[] = "ssid";                              // your network SSID (name)
const char password[] = "password";                     // your network password
#define CHANNEL_ID "channel ID"               // YouTube channel url
#define API_KEY "key"   // your Google API key
//------- ---------------------- ------

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long timeBetweenRequests =  60 * 1000;             // 60 Seconds, in milliseconds

void setup() {
  leddisplay.setDigitLimit(8);                              //setup MAX7219 Digit Limit
  leddisplay.printDigit(0);                                 //default display

	Serial.begin(115200);                                     //setup Serial Monitor
  delay(100);

	// Set WiFi to 'station' mode and disconnect from the AP if it was previously connected
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);

	Serial.print("\nConnecting to WiFi: ");
	Serial.println(ssid);

  // Connect to the WiFi network
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(500);
	}
	Serial.println("\nWiFi connected!");
	Serial.print("IP address: ");
	IPAddress ip = WiFi.localIP();
	Serial.println(ip);

	#ifdef ESP8266
	// Required if you are using ESP8266 V2.5 or above
	client.setInsecure();
	#endif

	// Uncomment for extra debugging info
	// api._debug = true;
}
long i = 0;

void loop() {
	if(api.getChannelStatistics(CHANNEL_ID)) {
		Serial.println("\n---------Stats-------------");

		Serial.print("Subscriber #: ");
		Serial.println(api.channelStats.subscriberCount);

		Serial.print("      View #: ");
		Serial.println(api.channelStats.viewCount);

    Serial.print("     Video #: ");
    Serial.println(api.channelStats.videoCount);

    Serial.print("   Comment #: ");
    Serial.println(api.channelStats.commentCount);

		// Probably not needed :)
		// Serial.print("hiddenSubscriberCount: ");
		// Serial.println(api.channelStats.hiddenSubscriberCount);

		Serial.println("----------------------------");
    leddisplay.printDigit(api.channelStats.subscriberCount);
	}
	delay(timeBetweenRequests);
}
