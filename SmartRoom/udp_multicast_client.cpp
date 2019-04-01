#include <Streaming.h>
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

//Podpis centraly. Pokud dorazi tato zprava, jeji odesilatel je centrala
#define PODPIS_CENTRALY "JA JSEM TADY VELKY SEF"

// Prihlasovaci udaje k Wi-Fi
const char* ssid = "Klobouk";
const char* heslo = "************";

// Multicastova IP adresa, viz https://cs.wikipedia.org/wiki/IP_multicast
IPAddress multicast_ip(224, 0, 0, 0);
// UDP port
const uint16_t multicast_port = 8201;

// Trida pro UDP/multicast komunikaci
WiFiUDP udp;

// Funkce setup se zpracuje hned po startu
void setup() {
  // Uvitani v seriove lince
  // Pri rychlejsi zapis pouzivam knihovnu Streaming.h
  // Viz http://arduiniana.org/libraries/streaming/
  Serial.begin(115200);
  Serial << endl;
  Serial << "Demonstrace identifikace centraly pomoci multicastu" << endl;
  Serial << "Prihlasuji se k Wi-Fi: " << ssid << " ";

  // Spusteni Wi-Fi v rezimu klient a prihlaseni se k AP
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, heslo);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial << '.';
  }
  Serial << endl << "Pripojeno na IP adrese " << WiFi.localIP();
  Serial << ", " << WiFi.RSSI() << " dBm" << endl << endl;
  Serial << "K odeslani multicastoveho datagramu zadej: s" << endl << endl;

  // Spusteni UDP poslechu na portu 8201
  udp.begin(multicast_port);
}

// Funkce loop se opakuje stale dokola
void loop() {
  // Pokud skrze seriovou linku dorazi znak s,
  // odesli skrze multicast zadost o registraci a odpoved centraly
  while (Serial.available()) {
    char prikaz = Serial.read();
    if (prikaz == 's') {
      Serial << "Posilam na multicast adresu ";
      Serial << multicast_ip << ":" << multicast_port << endl;
      Serial << "Obsah zpravy: ";
      Serial << "REGISTRACE/Ambientni lampa/35/0,1,2,3/";
      Serial << String(ziskejMAC()) << endl;
      odesliMulticast("REGISTRACE/Ambientni lampa/35/0,1,2,3/" + String(ziskejMAC()));
    }
  }

  // Zjisti, jestli dorazila nejaka UDP data na port 8201
  // a zkontroluj, jestli je to odpoved centraly
  udp.parsePacket();
  while (udp.available()) {
    String odpoved = udp.readString();
    odpoved.trim();
    Serial << "Prichozi zprava od " << udp.remoteIP() << ": " << odpoved << endl;
    // Pokud zprava obsahuje podpis centraly,
    // mohu si ulozit IP adresu odesilatele, je to totiz centrala
    if (odpoved == PODPIS_CENTRALY) {
      Serial << "Vyborne, toto je IP adresa centraly!" << endl;
    }
    Serial << endl;
  }
}

// funcke pro odeslani textove zpravy skrze multicast
void odesliMulticast(String zprava) {
  udp.beginPacketMulticast(multicast_ip, multicast_port, WiFi.localIP());
  udp.write(zprava.c_str());
  udp.endPacket();
}

// Funkce pro ziskani MAC adresy
// a prevedeni do textoveho formatu
char* ziskejMAC() {
  uint8_t MAC[6];
  static char cMAC[19];
  WiFi.macAddress(MAC);
  sprintf(cMAC, "%02x:%02x:%02x:%02x:%02x:%02x", MAC[0], MAC[1], MAC[2], MAC[3], MAC[4], MAC[5]);
  return cMAC;
}
