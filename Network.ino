void startWiFi() {
  if (!cfg.WiFimode) setupAP();   // режим точки доступа
  else setupLocal();              // подключаемся к точке
}

void setupAP() {
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  delay(100);
  IPAddress local_ip(192,168,1,1);
  IPAddress gateway(192,168,1,1);
  IPAddress subnet(255,255,255,0);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(cfg.APssid, cfg.APpassword);
  configureServer();
  server.begin();
  Serial.println("HTTP server started");
  delay(500);
}

void setupLocal() {
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  delay(100);
  WiFi.hostname("domofon.local");
  WiFi.begin(cfg.STAssid, cfg.STApassword);
}

void configureServer() {
  
  while (adminPanelFile.available()) {
    adminPanelCode += adminPanelFile.read();
  }
  server.on("/", handle_OnConnect);
}

void handle_OnConnect() {
  server.send(200, "text/html", adminPanelCode);
}
