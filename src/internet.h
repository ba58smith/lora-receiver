#ifndef _INTERNET_H_
#define _INTERNET_H_

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "config.h"

/**
 * @brief Class that manages all connections to, and interactions with, the Internet.
 */

class Internet {

private:
    const char* wifi_ssid_ = SSID; //"KeyAlmostWest";
    const char* wifi_pw_ = PASSWORD; //"sfaesfae";
    String jims_website_url_ = "http://www.totalcareprog.com/cgi-bin/butchmonitor_save.php";

public:
    
    /**
     * @brief Construct a new Internet object.
     */
    
    // BAS: add back after figuring out how to make UI and Internet point to each other
    // Internet(UI* ui) : ui_{ui} {}
    // and remove this one:
    Internet() {}

    /**
     * @brief Called during setup() and by transmitToWeb if necessary.
     */

    void connect_to_wifi() {
        uint8_t attempts = 0;
        Serial.print("Connecting to wifi ");
        WiFi.begin(wifi_ssid_, wifi_pw_);
        while (WiFi.status() != WL_CONNECTED && attempts < 10) {
            Serial.print(". ");
            delay(500);
            attempts++;
        }
        Serial.println("");
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("Not connected to wifi");
            delay(500);
        }
        else {
            Serial.println("Connected to " + WiFi.localIP().toString());
        }
    }

    // BAS: if I start sending data to Arduino IO, this will go away.
    // But keep it during the transition.
    bool transmit_to_web() {
        // BAS: add back after figuring out how to make UI and Internet point to each other
        // ui_->turnOnLed(); // both LED's
        Serial.println("Transmitting to Jim's website");
        bool success = false;
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("wifi not connected");
            connect_to_wifi();
        }
        else {
            Serial.println("Connected to wifi");
            HTTPClient http;
            String serverPath = jims_website_url_ + "?battery1=" + String(battery1, 2) 
                              + "&battery2=" + String(battery2, 2) + "&battery3=" + String(battery3) 
                              + "&temp=" + String(yourTemp) + "&humidity=" + String(yourHumidity) 
                              + "&pressure=" + String(yourPressure, 2);
            // Your Domain name with URL path or IP address with path
            http.begin(serverPath.c_str());
            // Send HTTP GET request
            int httpResponseCode = http.GET();
            if (httpResponseCode > 0) {
                Serial.print("HTTP Response code: ");
                Serial.println(httpResponseCode);
                Serial.println(http.getString());
                success = true;
            }
            else {
                Serial.print("Error code: ");
                Serial.println(httpResponseCode);
            }
            // Free resources
            http.end();
        }
        // BAS: add back after figuring out how to make UI and Internet point to each other
        // ui_->turnOFFLed(); // both LED's
        return success;

    } // end transmit to web


    String get_ssid() {
        return wifi_ssid_;
    }

    bool connected_to_wifi() {
        return WiFi.status() == WL_CONNECTED;
    }

}; // class Internet

#endif // _INTERNET_H_