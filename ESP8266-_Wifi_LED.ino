#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;

const char* ssid = "<SSID>";
const char* password = "<PASSWORD>";
 
String form = "<!DOCTYPE html><html><head><title>ESP</title><link rel='stylesheet' type='text/css' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'><script src='http://code.jquery.com/jquery-3.2.1.min.js' integrity='sha256-hwg4gsxgFZhOsEEamdOYGBf13FyQuiTwlAQgxVSNgt4=' crossorigin='anonymous'></script><link href='https://gitcdn.github.io/bootstrap-toggle/2.2.2/css/bootstrap-toggle.min.css' rel='stylesheet'><script src='https://gitcdn.github.io/bootstrap-toggle/2.2.2/js/bootstrap-toggle.min.js'></script></head><body><div class='container'><div class='jumbotron'><h1 class='display-3'>Welcome to ESP!</h1><p class='lead'>Turn off, or turn on the led.</p><hr class='my-4'></div><div style='text-align: center;' class='container'><form id='form' action='led'><div class='form-check form-check-inline'><input id='toggle-one' name='state' checked type='checkbox' value=1><script> $(function() { $('#toggle-one').bootstrapToggle({ on: 'Turn On', off: 'Turn Off' }); }); $(function() { $('#toggle-one').on('change',function(){ $('#form').submit(); }); });</script></div></form></div></div></body></html>";
ESP8266WebServer server(80);
 
const int led = 2;
 
void handle_led() {
  // get the value of request argument "state" and convert it to an int
  int state = server.arg("state").toInt();
 
  digitalWrite(led, state);
  String message = String("LED is now le ") + ((state)?"on":"off");
  server.send(200, "text/html", "<!DOCTYPE html><html><head><title>ESP</title></head><body><div style='width=50%;'><p>" + message + "</p><br><a href='/'>Home Page</a></div></body></html>");
}
 
void setup(void) {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  server.on("/", [](){
   server.send(200, "text/html", form);
  });
 
   server.on("/led", handle_led);
 
  // Start the server
  server.begin();
  Serial.println("HTTP server started!!");
}
 
void loop(void) {
  // check for incoming client connections frequently in the main loop:
  server.handleClient();
}
