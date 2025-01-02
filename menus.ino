//----------------------------------------------------MAIN MENU--------------------------------------------------------------------

void mainMenu() {
  mainm = new Menu(0.0, "Main Menu", true, c, down, up);
  mainm->addSection("Scannen", []() {
    activem = scanm;
    activem->setSelectedIndex(0);
    activem->render();
  });
  mainm->addSection("Select", []() {
    activem = selectm;
    activem->setSelectedIndex(0);
    activem->render();
  });
  mainm->addSection("Attack", []() {
    activem = attackm;
    activem->setSelectedIndex(0);
    activem->render();
  });
  mainm->addSection("Sniffer", []() {
    activem = snifferm;
    activem->setSelectedIndex(0);
    activem->render();
  });
}

//----------------------------------------------------END MAIN MENU--------------------------------------------------------------------




//----------------------------------------------------Scan MENUs--------------------------------------------------------------------

void scanMenu() {
  scanm = new Menu(1, "Scan Menu", true, c, down, up);
  scanm->addSection("Back", []() {
    activem = scanm->getParentMenu();
    activem->setSelectedIndex(0);
    activem->render();
  });
  scanm->addSection("AP scan", []() {
    apscanm();
    activem = apScanm;
    activem->render();
    scanAps();
  });
  scanm->addSection("ST scan", []() {
    stscanm();
    activem = stScanm;
    activem->render();
    scanSts();
  });
}




void apscanm() {
  apScanm = new Menu(1.1, "Scanning APs...", true, c, down, up);
  scanm->setSubMenu(apScanm);
}

void stscanm() {
  stScanm = new Menu(1.2, "Scanning STs...", true, c, down, up);
  scanm->setSubMenu(stScanm);
}

//----------------------------------------------------END Scan MENUs--------------------------------------------------------------------



//----------------------------------------------------Select MENUs--------------------------------------------------------------------

void selectMenu() {
  selectm = new Menu(2, "Select Menu", true, c, down, up);
  selectm->addSection("Back", []() {
    activem = selectm->getParentMenu();
    activem->setSelectedIndex(0);
    activem->render();
  });
  selectm->addSection("APs", []() {
    selectMenuAp();
    activem = selectAp;
    activem->setSelectedIndex(0);
    activem->render();
  });
  selectm->addSection("STs", []() {
    selectMenuSt();
    activem = selectSt;
    activem->setSelectedIndex(0);
    activem->render();
  });
}



void selectMenuAp() {
  selectAp = new Menu(2.1, "Select Menu     APs", true, c, down, up);
  selectAp->addSection("Back", []() {
    activem = selectAp->getParentMenu();
    activem->setSelectedIndex(0);
    activem->render();
  });
  if (firstApScan) {
    selectedApList.resize(apList.size(), false);
    for (int i = 0; i < apList.size(); i++) {
      if (selectedApList[i]) {
        selectAp->addSection(("* " + apList[i].ssid).c_str(), [i]() {
          selectedApList[i] = !selectedApList[i];
          if (selectedApList[i]) {
            activem->setSection("* " + activem->getSection(i + 1), i + 1);
          } else {
            activem->setSection(activem->getSection(i + 1).substr(2), i + 1);
          }
          activem->render();
        });
      } else {
        selectAp->addSection(apList[i].ssid.c_str(), [i]() {
          selectedApList[i] = !selectedApList[i];
          if (selectedApList[i]) {
            activem->setSection("* " + activem->getSection(i + 1), i + 1);
          } else {
            activem->setSection(activem->getSection(i + 1).substr(2), i + 1);
          }
          activem->render();
        });
      }
    }
  }
  selectm->setSubMenu(selectAp);
}



void selectMenuSt() {
  selectSt = new Menu(2.2, "Select Menu     STs", true, c, down, up);
  selectSt->addSection("Back", []() {
    activem = selectSt->getParentMenu();
    activem->setSelectedIndex(0);
    activem->render();
  });
  if (firstApScan) {
    selectedStList.resize(stList.size(), false);
    for (int i = 0; i < stList.size(); i++) {
      if (selectedStList[i]) {
        selectAp->addSection(("* " + stList[i].ssid).c_str(), [i]() {
          selectedStList[i] = !selectedStList[i];
          if (selectedStList[i]) {
            activem->setSection("* " + activem->getSection(i + 1), i + 1);
          } else {
            activem->setSection(activem->getSection(i + 1).substr(2), i + 1);
          }
          activem->render();
        });
      } else {
        selectSt->addSection(stList[i].ssid.c_str(), [i]() {
          selectedStList[i] = !selectedStList[i];
          if (selectedStList[i]) {
            activem->setSection("* " + activem->getSection(i + 1), i + 1);
          } else {
            activem->setSection(activem->getSection(i + 1).substr(2), i + 1);
          }
          activem->render();
        });
      }
    }
    selectm->setSubMenu(selectSt);
  }
}

//----------------------------------------------------END Select MENUs--------------------------------------------------------------------



//----------------------------------------------------START Attack MENUs--------------------------------------------------------------------

void attackMenu() {
  attackm = new Menu(3, "Attack Menu", true, c, down, up);
  attackm->addSection("Back", []() {
    activem = attackm->getParentMenu();
    activem->setSelectedIndex(0);
    activem->render();
  });
  attackm->addSection("Deauth", []() {
    deauthMenu();
    attackm->setSubMenu(deauthm);
    activem = deauthm;
    activem->setSelectedIndex(0);
    activem->render();
  });
}


void deauthMenu() {
  int allAps = std::count(std::begin(selectedApList), std::end(selectedApList), true);
  int allSts = std::count(std::begin(selectedStList), std::end(selectedStList), true);
  deauthm = new Menu(3.1, ("Deauth         " + String(allAps) + "  " + String(allSts)).c_str(), true, c, down, up);
  deauthm->addSection("Back", []() {
    activem = deauthm->getParentMenu();
    activem->setSelectedIndex(0);
    activem->render();
  });
  deauthm->addSection("Start", []() {
    activem->setTitle(!deauthing ? "Deauthing..." : "Deauth");
    activem->setSection(!deauthing ? "Stop" : "Start", 1);
    activem->setScroll(!deauthing ? false : true);
    deauthing = !deauthing;
    if(deauthing)setLedAttack();
    else setLedNormal();
    activem->render();
  });
}

//----------------------------------------------------END Attack MENUs--------------------------------------------------------------------


//----------------------------------------------------START SNIFFER MENUs--------------------------------------------------------------------

void snifferMenu() {
  snifferm = new Menu(4, "Sniffers", true, c, down, up);
  snifferm->addSection("Back", []() {
    activem = snifferm->getParentMenu();
    activem->setSelectedIndex(0);
    curChannel = 0;
    activem->render();
  });
  snifferm->addSection("Monitor", []() {
    activem = packetMonitorm;
    setLedMonitor();
    packetMonitor();
  });
}


void packetMonitorMenu() {
  packetMonitorm = new Menu(4.1, "", true, c, down, up, []() {
      curChannel--;
      monitor = false;
      packetMonitor();
    },
    []() {
      curChannel++;
      monitor = false;
      packetMonitor();
    });
  packetMonitorm->addSection("", []() {
    activem = packetMonitorm->getParentMenu();
    monitor = false;
    setLedNormal();
    activem->render();
  });
}
//----------------------------------------------------END SNIFFER MENUs--------------------------------------------------------------------
