#include <ESP8266WiFi.h>

const char ssid[] = "e900";
const char password[] = "********";

void setup() {
	// on démarre le port série
	Serial.begin(115200);
	// On attend "un peu" que le buffer soit prêt
	delay(10);
	// On efface la configuration précédente
	WiFi.disconnect(true);
	// Initialisation de la connection
	WiFi.begin(ssid, password);
	// Test pour déterminer quand la connection est prete
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
	}
	// Affichage des informations
	Serial.print("\nConnecte a ");
	Serial.print(ssid);
	Serial.print(" avec l'ip ");
	Serial.println(WiFi.localIP());
}
void loop() {
}
