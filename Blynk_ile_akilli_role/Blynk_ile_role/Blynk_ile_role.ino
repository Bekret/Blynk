#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Blynk uygulamasından alacağımız anaharı buraya yazıyoruz.
char auth[] = "Blynk_uygulama_token";

char ssid[] = "Wifi_adi";
char pass[] = "Wifi_sifre";

// Led ve Buton pinlerimizi tanımlıyoruz.
int role = D1;
int buton = D2;

BlynkTimer timer;
void checkPhysicalButton();

int roleDurum = LOW;
int butonDurum = HIGH;

// Blynk sunucusuna her bağlandığımızda 
BLYNK_CONNECTED() {
  // Butonun son durumunu istiyoruz.
  Blynk.syncVirtual(V0);
}

// Uygulamada butonumuza bastığımızda röle durumunu değiştiriyoruz.
BLYNK_WRITE(V2) {
  roleDurum = param.asInt();
  digitalWrite(role, roleDurum);
}

void checkPhysicalButton()
{
  if (digitalRead(buton) == LOW) {
    //Butonun seriye bağlamasını önlüyoruz.
    if (butonDurum != LOW) {

      roleDurum = !roleDurum;
      digitalWrite(role, roleDurum);
      
      Blynk.virtualWrite(V2, roleDurum);
    }
    butonDurum = LOW;
  } else {
    butonDurum = HIGH;
  }
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  pinMode(role, OUTPUT);
  pinMode(buton, INPUT_PULLUP);
  digitalWrite(role, roleDurum);

  // Fonksiyonu her 100ms de bir çağırıyoruz.
  timer.setInterval(100L, checkPhysicalButton);
}

void loop()
{
  Blynk.run();
  timer.run();
}
