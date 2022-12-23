#include <Servo.h>

// renk sensörü tanımlamaları
#define csOrta_s0 13
#define csOrta_s1 12
#define csOrta_s2 8
#define csOrta_s3 7
#define csOrta_out 4


#define csArka_s0 13
#define csArka_s1 12
#define csArka_s2 8
#define csArka_s3 7
#define csArka_out 2

//  RGB LED için kullanıyor
#define kled 7
#define yled 6
#define mled 5

Servo servoOrta;
Servo servoArka;

int degreeOrta;
int yumurtaKirmizi, yumurtaYesil, yumurtaMavi = 0;
int zeminKirmizi, zeminYesil, zeminMavi = 0;

String bizimRengimiz = "kirmizi";  // arkadaki renk sensörünün okuduğu değer atanacak
String yumurtaRengi;    // ortadaki renk sensörünün okuduğu değer atanacak
String zeminRengi;    // arkadaki renk sensörünün okuduğu değer atanacak

void setup() {
  pinMode(csOrta_s0, OUTPUT);  pinMode(csOrta_s1, OUTPUT);  pinMode(csOrta_s2, OUTPUT);  pinMode(csOrta_s3, OUTPUT);  pinMode(csOrta_out, INPUT);

  pinMode(csArka_s0, OUTPUT);  pinMode(csArka_s1, OUTPUT);  pinMode(csArka_s2, OUTPUT);  pinMode(csArka_s3, OUTPUT);  pinMode(csArka_out, INPUT);
  
  pinMode(kled, OUTPUT);  pinMode(yled, OUTPUT);  pinMode(mled, OUTPUT);
  
  // Arduino için Frekans değerini ayarladık
  digitalWrite(csOrta_s0, HIGH);  digitalWrite(csOrta_s1, LOW);  digitalWrite(csArka_s0, HIGH);  digitalWrite(csArka_s1, LOW);

  servoOrta.attach(3);    // Orta Servo tanımlanıyor
  servoArka.attach(9);    // Arka Servo tanımlanıyor
    
  Serial.begin(9600);
}

void loop() {
  //kalibreEt();
  //kalibreEtZemin();
  kalibreEdilmis();
  kalibreEdilmisZemin();
  yumurtaRengiOku();

  //servoOrtaKalibreEt();
  servoOrtaDon(yumurtaRengi);
}

// gelen renk değerlerine göre orta servo ayarlanıyor
// beyaz = düz
// kırmızı = sol
// mavi = sağ dönme işlemleri yapılıyor
void servoOrtaDon(String renk) {
  if (renk == "beyaz") {  // düz ol
    degreeOrta = 90;
    servoOrta.write(degreeOrta);
  } else if (bizimRengimiz == renk) {  // sola dön
    degreeOrta = 180;
    servoOrta.write(degreeOrta);
  } else {  // sağa dön
    degreeOrta = 0;
    servoOrta.write(degreeOrta);
  }
}

void kalibreEdilmis() {
  delay(1000);
  // Kırmızı rengi belirleme
  digitalWrite(csOrta_s2, LOW);
  digitalWrite(csOrta_s3, LOW);
  yumurtaKirmizi = pulseIn(csOrta_out, LOW);
  yumurtaKirmizi = map(yumurtaKirmizi, 48, 180, 0, 100);
  Serial.print("Kırmızı renk: ");
  Serial.print(yumurtaKirmizi);
  Serial.print("\t");
  delay(50);

  
  // Yesil rengi belirleme
  digitalWrite(csOrta_s2, HIGH);
  digitalWrite(csOrta_s3, HIGH);
  yumurtaYesil = pulseIn(csOrta_out, LOW);
  yumurtaYesil = map(yumurtaYesil, 43, 210, 0, 100);
  Serial.print("Yeşil renk: ");
  Serial.print(yumurtaYesil);
  Serial.print("\t");
  delay(50);

  
  // Mavi rengi belirleme
  digitalWrite(csOrta_s2, LOW);
  digitalWrite(csOrta_s3, HIGH);
  yumurtaMavi = pulseIn(csOrta_out, LOW);
  yumurtaMavi = map(yumurtaMavi, 40, 190, 0, 100);
  Serial.print("Mavi renk: ");
  Serial.print(yumurtaMavi);
  Serial.println("\t");
  delay(50);
}


void kalibreEdilmisZemin() {
  delay(1000);
  // Kırmızı ZEMİN rengi belirleme
  digitalWrite(csArka_s2, LOW);
  digitalWrite(csArka_s3, LOW);
  zeminKirmizi = pulseIn(csArka_out, LOW);
  zeminKirmizi = map(zeminKirmizi, 48, 180, 0, 100);
  Serial.print("ZEMİN Kırmızı renk: ");
  Serial.print(zeminKirmizi);
  Serial.print("\t");
  delay(50);

  
  // Yesil ZEMİN rengi belirleme
  digitalWrite(csArka_s2, HIGH);
  digitalWrite(csArka_s3, HIGH);
  zeminYesil = pulseIn(csArka_out, LOW);
  zeminYesil = map(zeminYesil, 43, 210, 0, 100);
  Serial.print("ZEMİN Yeşil renk: ");
  Serial.print(zeminYesil);
  Serial.print("\t");
  delay(50);

  
  // Mavi ZEMİN rengi belirleme
  digitalWrite(csArka_s2, LOW);
  digitalWrite(csArka_s3, HIGH);
  zeminMavi = pulseIn(csArka_out, LOW);
  zeminMavi = map(zeminMavi, 40, 190, 0, 100);
  Serial.print("ZEMİN Mavi renk: ");
  Serial.print(zeminMavi);
  Serial.println("\t");
  delay(50);
}

void yumurtaRengiOku() {
  // beyaz algılanıyor
  if (yumurtaKirmizi < 0 and yumurtaMavi < 0 and yumurtaYesil < 0) {
    digitalWrite(kled, HIGH);
    digitalWrite(yled, HIGH);
    digitalWrite(mled, HIGH);
    yumurtaRengi = "beyaz";
    Serial.println("Beyaz zemin algılandı");
  }
  // kırmızı algılanıyor
  else if (yumurtaKirmizi<90 and yumurtaKirmizi < yumurtaMavi and yumurtaKirmizi < yumurtaYesil) {
    digitalWrite(kled, HIGH);
    digitalWrite(yled, LOW);
    digitalWrite(mled, LOW);
    yumurtaRengi = "kirmizi";
    Serial.println("Kırmızı yumurta algılandı");
  }
  // mavi algılanıyor
  else if (yumurtaMavi < yumurtaKirmizi and yumurtaMavi < yumurtaYesil ) {
    digitalWrite(mled, HIGH);
    digitalWrite(kled, LOW);
    digitalWrite(yled, LOW);
    yumurtaRengi = "mavi";
    Serial.println("Mavi yumurta algılandı");
  }
}

void zeminRengiOku() {
  // beyaz zemin algılanıyor
  if (zeminKirmizi < 0 and zeminMavi < 0 and zeminYesil < 0) {
    digitalWrite(kled, HIGH);
    digitalWrite(yled, HIGH);
    digitalWrite(mled, HIGH);
    zeminRengi = "beyaz";
    Serial.println("Beyaz zemin algılandı");
  }
}







/* ******************** */
/* KALİBRASYON AYARLARI */
/* ******************** */

void servoOrtaKalibreEt() {
  degreeOrta = 90;
  servoOrta.write(degreeOrta);
  servoArka.write(100);
}

void kalibreEt() {
  // Kırmızı rengi belirleme
  digitalWrite(csOrta_s2, LOW);
  digitalWrite(csOrta_s3, LOW);
  yumurtaKirmizi = pulseIn(csOrta_out, LOW);
  Serial.print("Kırmızı renk: ");
  Serial.print(yumurtaKirmizi);
  Serial.print("\t");
  delay(1000);

  
  // Yesil rengi belirleme
  digitalWrite(csOrta_s2, HIGH);
  digitalWrite(csOrta_s3, HIGH);
  yumurtaYesil = pulseIn(csOrta_out, LOW);
  Serial.print("Yeşil renk: ");
  Serial.print(yumurtaYesil);
  Serial.print("\t");
  delay(1000);

  
  // Mavi rengi belirleme
  digitalWrite(csOrta_s2, LOW);
  digitalWrite(csOrta_s3, HIGH);
  yumurtaMavi = pulseIn(csOrta_out, LOW);
  Serial.print("Mavi renk: ");
  Serial.print(yumurtaMavi);
  Serial.println("\t");
  delay(1000);
}


void kalibreEtZemin() {
  // Kırmızı rengi belirleme
  digitalWrite(csOrta_s2, LOW);
  digitalWrite(csOrta_s3, LOW);
  zeminKirmizi = pulseIn(csArka_out, LOW);
  Serial.print("ZEMIN Kırmızı renk: ");
  Serial.print(zeminKirmizi);
  Serial.print("\t");
  delay(1000);

  
  // Yesil rengi belirleme
  digitalWrite(csOrta_s2, HIGH);
  digitalWrite(csOrta_s3, HIGH);
  zeminYesil = pulseIn(csArka_out, LOW);
  Serial.print("ZEMIN Yeşil renk: ");
  Serial.print(zeminYesil);
  Serial.print("\t");
  delay(1000);

  
  // Mavi rengi belirleme
  digitalWrite(csOrta_s2, LOW);
  digitalWrite(csOrta_s3, HIGH);
  zeminMavi = pulseIn(csArka_out, LOW);
  Serial.print("ZEMIN Mavi renk: ");
  Serial.print(zeminMavi);
  Serial.println("\t");
  delay(1000);
}
