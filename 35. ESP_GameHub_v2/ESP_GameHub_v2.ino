#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>

ESP8266WebServer server(80);

const IPAddress apIP(192,168,4,1);
const IPAddress netMsk(255,255,255,0);

bool offlineMode = false;

String getContentType(String filename) {
  if (filename.endsWith(".html")) return "text/html";
  if (filename.endsWith(".css")) return "text/css";
  if (filename.endsWith(".js")) return "application/javascript";
  if (filename.endsWith(".png")) return "image/png";
  if (filename.endsWith(".jpg") || filename.endsWith(".jpeg")) return "image/jpeg";
  return "text/plain";
}

bool loadOfflineFlag() {
  if (!LittleFS.exists("/offline_mode")) return false;
  File f = LittleFS.open("/offline_mode", "r");
  if (!f) return false;
  char buf[2] = {0};
  f.readBytes(buf, 1);
  f.close();
  return buf[0] == '1';
}

void saveOfflineFlag(bool enabled) {
  if (enabled) {
    File f = LittleFS.open("/offline_mode", "w");
    if (f) { f.print("1"); f.close(); }
  } else {
    if (LittleFS.exists("/offline_mode")) LittleFS.remove("/offline_mode");
  }
}

void saveWiFiCredentials(const String& ssid, const String& password) {
  File f = LittleFS.open("/wifi_creds", "w");
  if (f) {
    f.println(ssid);
    f.println(password);
    f.close();
  }
}

bool loadWiFiCredentials(String& ssid, String& password) {
  if (!LittleFS.exists("/wifi_creds")) return false;
  File f = LittleFS.open("/wifi_creds", "r");
  if (!f) return false;
  ssid = f.readStringUntil('\n');
  password = f.readStringUntil('\n');
  ssid.trim();
  password.trim();
  f.close();
  return true;
}

void handleRoot() {
  if (offlineMode) {
    // Serve captive portal HTML setup page
    String html = R"rawliteral(
<html><body>
<h1>GameHub WiFi Setup</h1>
<form method='POST' action='/connect'>
  SSID:<br><input type='text' name='ssid'><br>
  Password:<br><input type='password' name='password'><br>
  <input type='submit' value='Connect to WiFi'>
</form>
<hr>
<form method='POST' action='/offline'>
  <input type='submit' value='Play Offline'>
</form>
<p>Press reset anytime to return here.</p>
</body></html>
)rawliteral";
    server.send(200, "text/html", html);
  } else {
    // Online mode — serve index.html from LittleFS (your game)
    if (LittleFS.exists("/index.html")) {
      File f = LittleFS.open("/index.html", "r");
      server.streamFile(f, "text/html");
      f.close();
    } else {
      server.send(404, "text/plain", "index.html not found");
    }
  }
}


void handleConnect() {
  if (server.hasArg("ssid") && server.hasArg("password")) {
    String ssid = server.arg("ssid");
    String password = server.arg("password");

    saveWiFiCredentials(ssid, password);
    saveOfflineFlag(false);

    String html = "<html><body><h2>Trying to connect to WiFi...</h2>";
    html += "<p>Please disconnect from 'GameHubAP' and connect to your WiFi network.</p>";
    html += "<p>Then access the ESP at its IP on your network (check Serial Monitor for IP).</p>";
    html += "<p>Device will reboot now.</p></body></html>";
    server.send(200, "text/html", html);

    delay(3000);
    ESP.restart();
  } else {
    server.send(400, "text/plain", "Missing SSID or password");
  }
}

void handleOffline() {
  saveOfflineFlag(true);

  String html = "<html><body><h2>Offline Mode Enabled</h2>";
  html += "<p>Redirecting to game hub...</p>";
  html += "<script>window.location.href = '/index.html';</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void serveFileOr404() {
  String path = server.uri();
  if (path == "/") path = "/index.html";

  if (LittleFS.exists(path)) {
    File f = LittleFS.open(path, "r");
    server.streamFile(f, getContentType(path));
    f.close();
  } else {
    server.send(404, "text/plain", "404: File Not Found");
  }
}

void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost connection, trying to reconnect...");
    
    String ssid, password;
    if (loadWiFiCredentials(ssid, password)) {
      WiFi.disconnect();
      WiFi.begin(ssid.c_str(), password.c_str());
      
      unsigned long startAttemptTime = millis();
      // Try to connect for 10 seconds
      while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
        delay(500);
        Serial.print(".");
      }
      Serial.println();

      if (WiFi.status() == WL_CONNECTED) {
        Serial.print("Reconnected! IP: ");
        Serial.println(WiFi.localIP());
      } else {
        Serial.println("Failed to reconnect to WiFi");
      }
    } else {
      Serial.println("No WiFi credentials saved.");
    }
  }
}

void setup() {
  Serial.begin(9600);
  delay(100);

  if (!LittleFS.begin()) {
    Serial.println("Failed to mount LittleFS");
    return;
  }

  offlineMode = loadOfflineFlag();

  if (offlineMode) {
    Serial.println("Starting offline AP mode");
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, netMsk);
    WiFi.softAP("GameHubAP_Offline", "12345678");

    server.on("/", handleRoot);
    server.on("/offline", HTTP_POST, handleOffline);
    server.on("/connect", HTTP_POST, handleConnect);
    server.onNotFound(serveFileOr404);
    server.begin();

    Serial.println("AP started. Connect to GameHubAP_Offline, open http://192.168.4.1/");
  } else {
    String ssid, pass;
    if (loadWiFiCredentials(ssid, pass)) {
      Serial.printf("Trying to connect to saved WiFi: %s\n", ssid.c_str());
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid.c_str(), pass.c_str());

      unsigned long startTime = millis();
      while (WiFi.status() != WL_CONNECTED && millis() - startTime < 15000) {
        delay(500);
      }

      if (WiFi.status() == WL_CONNECTED) {
        Serial.print("Connected to WiFi! IP: ");
        Serial.println(WiFi.localIP());

        // **Start server here too for station mode:**
        server.on("/", handleRoot);
        server.on("/offline", HTTP_POST, handleOffline);
        server.on("/connect", HTTP_POST, handleConnect);
        server.onNotFound(serveFileOr404);
        server.begin();

        Serial.println("HTTP server started on station mode");
      } else {
        Serial.println("Failed to connect. Starting AP for config.");
        LittleFS.remove("/wifi_creds");
        saveOfflineFlag(true);
        ESP.restart();
      }
    } else {
      Serial.println("No saved WiFi credentials. Starting AP for config.");
      saveOfflineFlag(true);
      ESP.restart();
    }
  }
}

void loop() {
  // **Always handle client requests**
  server.handleClient();
  if (!offlineMode) {
  checkWiFiConnection();
  }
}
