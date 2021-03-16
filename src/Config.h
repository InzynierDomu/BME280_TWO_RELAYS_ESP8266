namespace config
{
  const char* wifi_ssid = "ssid";     ///< WiFi ssid 
  const char* wifi_pass = "pass";     ///< WiFi password
  const char* mqtt_addr = "192.168.0.17";   ///< MQTT brocker ip
  const char* mqtt_user = "id";   ///< MQTT user name
  const char* mqtt_pass = "id";   ///< MQTT password
  const int mqtt_port = 1883;     ///< MQTT port
  const char* sub_topic = "bedroom/controller1/output/#";   ///< united topic for subscribed topics
  const String lamp_topic = "bedroom/controller1/output/plantlamp";   ///< topic to subscribe for output lamp
  const String humidifier_topic = "bedroom/controller1/output/humidifier";    ///< topic to subscribe for output humidifier
  const char* thermometer_topic = "bedroom/controller1/temp";   ///< topic to publish temperature from sensor
  const char* humidity_topic = "bedroom/controller1/hum";       ///< topic to publish humidity from sensor
  const byte relay_lamp_pin = D6;       ///< pin output for lamp
  const byte relay_humidifier_pin = D5; ///< pin output for humidifier
  const int repet_time_period = 60000;  ///< time what the measurement will be cycling sent
}
