#ifndef WEB_H
#define WEB_H

#include "config.h"
#include "stringMethods.h"

char ssid[] = "test_wifi";        // your network SSID (name)
char pass[] = "password";        // your network password (use for WPA, or use as key for WEP)
WiFiServer web_server(80);
int web_status = WL_IDLE_STATUS;

void printWiFiStatus();

bool apSetup() {
  Serial.println("Access Point Web Server");

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    return false;
  }

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  web_status = WiFi.beginAP(ssid, pass);
  if (web_status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // start the web server on port 80
  web_server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();
  return true;
}

int apRun() {
  // compare the previous status to the current status
  if (web_status != WiFi.status()) {
    // it has changed update the variable
    web_status = WiFi.status();

    if (web_status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      Serial.println("Device connected to AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }

  WiFiClient client = web_server.available();   // listen for incoming clients
  String new_ssid = "";
  String new_password = "";
  bool setSSID = false;
  bool setPassword = false;
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        // Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html; charset=us-ascii");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<form action=\"/action_page\">");
            client.print("<label for=\"ssid\">SSID:</label><br>");
            client.print("<input type=\"text\" id=\"ssid\" name=\"ssid\"><br>");
            client.print("<label for=\"password\">Password</label><br>");
            client.print("<input type=\"password\" id=\"password\" name=\"password\"><br><br>");
            client.print("<input type=\"submit\" value=\"Submit\">");
            client.print("</form>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        if (c == '&') {
          setSSID = false;
        }

        if (c == ' ' && setPassword == true) {
          setPassword = false;
        }

        if (setSSID == true) {
          new_ssid += c;
        }

        if (setPassword == true) {
          new_password += c;
        }

        if (currentLine.endsWith("ssid=")) {
          setSSID = true;
        }
        if (currentLine.endsWith("password=")) {
          setPassword = true;
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");

    char* ssidToStore = mallocString(new_ssid.length());
    strcpy (ssidToStore, new_ssid.c_str());
    char* passToStore = mallocString(new_password.length());
    strcpy (passToStore, new_password.c_str());

    bool stored = storeWifiData(ssidToStore, passToStore);

    free(ssidToStore);
    free(passToStore);
    return stored;
  }
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}

#endif
