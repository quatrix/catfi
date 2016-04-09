#include <SoftwareSerial.h>
#include <math.h>


SoftwareSerial esp8266_serial(2,3);


float loadA = 0; // kg
float analogvalA = 0;
float lastLoad = -999;

float loadB = 249; // kg
int analogvalB = 637; // analog reading taken with load B on the load cell

void setup() {
    Serial.begin(9600);
    esp8266_serial.begin(9600);

    delay(100);

    unsigned int sum = 0;

    for (int i=0; i< 10; i++) {
      sum += analogRead(A0);
      Serial.println(sum);
      delay(100);
    }
    analogvalA = sum / 10;
    Serial.println(analogvalA);
}



void loop() {
    float load = analogToLoad(analogRead(A0));

    if (lastLoad != load) {
        get("108.161.129.205", "/weight?grams="+String(load));
        lastLoad = load;
    }

    delay(1000);
}

void get(String ip, String path) {
    send(ip, "GET " + path + " HTTP/1.0\r\n\r\n");
}

void send(String dst_ip, String data) {
    String cmd = "AT+CIPSTART=\"TCP\",\"";
    cmd += dst_ip;
    cmd += "\",55669";

    esp8266_serial.println(cmd);

    if(esp8266_serial.find("Error"))
         return;

    esp8266_serial.print("AT+CIPSEND=");
    esp8266_serial.println(data.length()-3);

    delay(100);

    if(esp8266_serial.find(">")) {
        esp8266_serial.println(data);
    } else {
        esp8266_serial.println("AT+CIPCLOSE");
    }

}

float analogToLoad(float analogval){
    return mapfloat(analogval, analogvalA, analogvalB, loadA, loadB);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
