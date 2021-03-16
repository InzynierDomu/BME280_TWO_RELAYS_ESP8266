#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_BME280.h>
#include "Config.h"

Adafruit_BME280 m_bme_sensor;       ///< BME280 i2c temperature, humidity and pressure sensor 
WiFiClient m_espClient;             ///< WiFi client
PubSubClient m_client(m_espClient); ///< MQTT client

/**
 * @brief change digital output state
 * @param pin output pin to change
 * @param state '0' or '1' for low or high state to change
 */
void change_output(byte pin, byte* state)
{
  if ((char)*state == '1')
  {
    digitalWrite(pin, HIGH); 
    Serial.println("High");
  } 
  else if ((char)*state == '0')
  {
    digitalWrite(pin, LOW);
    Serial.println("Low"); 
  }
  else
  {
    Serial.println("Message unrecognized");
  }
}

/**
 * @brief callback after MQTT message arrived
 * @param topic topic where message arrived
 * @param payload contet of message
 * @param lenght content lenght
 */ 
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.print((char)*payload);
  Serial.println();

  if(config::lamp_topic.equals(topic))
  {
    change_output(config::relay_lamp_pin, payload);
  }
  else if(config::humidifier_topic.equals(topic))
  {
    change_output(config::relay_humidifier_pin, payload);
  }
}

/**
 * @brief BME280 initialization
 */ 
void Bme_init()
{
  if(m_bme_sensor.begin(BME280_ADDRESS_ALTERNATE))
  {
    Serial.println("BME sensor alt"); 
  }
  else if(m_bme_sensor.begin(BME280_ADDRESS))
  {
    Serial.println("BME sensor");
  }
  else
  {
    Serial.println("BME not found");
  }
}

/**
 * @brief WiFi initialization
 */ 
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(config::wifi_ssid);

  WiFi.begin(config::wifi_ssid, config::wifi_pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/**
 * @brief reconnect to MQTT and subscribe
 */ 
void reconnect() {
  while (!m_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (m_client.connect("ESP8266Client", config::mqtt_user, config::mqtt_pass)) {
      Serial.println("connected");
      m_client.subscribe(config::sub_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(m_client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

/**
 * @brief main setup
 */ 
void setup() {
  pinMode(config::relay_lamp_pin, OUTPUT); 
  pinMode(config::relay_humidifier_pin, OUTPUT);    
  digitalWrite(config::relay_lamp_pin, LOW); 
  digitalWrite(config::relay_humidifier_pin, LOW); 

  Serial.begin(9600);
  setup_wifi();
  m_client.setServer(config::mqtt_addr, config::mqtt_port);
  m_client.setCallback(callback);

  Bme_init();
}

/**
 * @brief main loop
 */ 
void loop() {
  if (!m_client.connected()) {
    reconnect();
  }
  m_client.loop();
  
  static long last_loop_time = 0;
  long loop_time = millis();
  if(loop_time - last_loop_time > config::repet_time_period)
  {
    char buf[10];
    float measure = m_bme_sensor.readTemperature();
    sprintf(buf, "%f", measure);
    m_client.publish(config::thermometer_topic, buf);
    measure = m_bme_sensor.readHumidity();
    sprintf(buf, "%f", measure);
    m_client.publish(config::humidity_topic, buf);
    last_loop_time = millis();
    Serial.println("Message published");
  }
}