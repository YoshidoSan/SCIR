const char* ssid = "";
const char* password = "";

const char* mqttServer = "mqtt3.thingspeak.com";
const int mqttPort = 1883; // Port MQTT dla ThingSpeak
const char* clientID = "esp32Client";  // Unikalne ID klienta MQTT
const char* writeAPIKey = "0M73RU1X0C0P82V0"; // Klucz do zapisu kanału ThingSpeak
unsigned long channelID = 2718645; // ID kanału ThingSpeak
