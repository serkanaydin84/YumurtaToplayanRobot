#include <Servo.h>

#define csOrta_s0 13
#define csOrta_s1 12
#define csOrta_s2 11
#define csOrta_s3 10
#define csOrta_out 9

#define kled 7
#define yled 6
#define mled 5

Servo servoOrta;
int degree;
int kirmizi, yesil, mavi = 0;

String bizimRengimiz = "kirmizi";  // arkadaki renk sensörünün okuduğu değer atanacak
String yumurtaRengi;    // ortadaki renk sensörünün okuduğu değer atanacak

void setup() {
  pinMode(csOrta_s0, OUTPUT);
  pinMode(csOrta_s1, OUTPUT);
  pinMode(csOrta_s2, OUTPUT);
  pinMode(csOrta_s3, OUTPUT);
  pinMode(csOrta_out, INPUT);
  pinMode(kled, OUTPUT);
  pinMode(yled, OUTPUT);
  pinMode(mled, OUTPUT);
  
  // Arduino için Frekans değerini ayarladık
  digitalWrite(csOrta_s0, HIGH);
  digitalWrite(csOrta_s1, LOW);
  Serial.begin(9600);

  // Orta Servo tanımlanıyor
  servoOrta.attach(3);
}

void loop() {
  //kalibreEt();
  kalibreEdilmis();
  yumurtaRengiOKu();

  //servoOrtaKalibreEt();
  servoOrtaDon(yumurtaRengi);
}

void servoOrtaKalibreEt() {
  degree = 90;
  servoOrta.write(degree);
}

// gelen renk değerlerine göre orta servo ayarlanıyor
// beyaz = düz
// kırmızı = sol
// mavi = sağ dönme işlemleri yapılıyor
void servoOrtaDon(String renk) {
  if (renk == "beyaz") {  // düz ol
    degree = 90;
    servoOrta.write(degree);
  } else if (bizimRengimiz == renk) {  // sola dön
    degree = 180;
    servoOrta.write(degree);
  } else {  // sağa dön
    degree = 0;
    servoOrta.write(degree);
  }
}

void kalibreEt() {
  // Kırmızı rengi belirleme
  digitalWrite(csOrta_s2, LOW);
  digitalWrite(csOrta_s3, LOW);
  kirmizi = pulseIn(csOrta_out, LOW);
  Serial.print("Kırmızı renk: ");
  Serial.print(kirmizi);
  Serial.print("\t");
  delay(1000);

  
  // Yesil rengi belirleme
  digitalWrite(csOrta_s2, HIGH);
  digitalWrite(csOrta_s3, HIGH);
  yesil = pulseIn(csOrta_out, LOW);
  Serial.print("Yeşil renk: ");
  Serial.print(yesil);
  Serial.print("\t");
  delay(1000);

  
  // Mavi rengi belirleme
  digitalWrite(csOrta_s2, LOW);
  digitalWrite(csOrta_s3, HIGH);
  mavi = pulseIn(csOrta_out, LOW);
  Serial.print("Mavi renk: ");
  Serial.print(mavi);
  Serial.println("\t");
  delay(1000);
}

void kalibreEdilmis() {
  delay(1000);
  // Kırmızı rengi belirleme
  digitalWrite(csOrta_s2, LOW);
  digitalWrite(csOrta_s3, LOW);
  kirmizi = pulseIn(csOrta_out, LOW);
  kirmizi = map(kirmizi, 48, 180, 0, 100);
  Serial.print("Kırmızı renk: ");
  Serial.print(kirmizi);
  Serial.print("\t");
  delay(1000);

  
  // Yesil rengi belirleme
  digitalWrite(csOrta_s2, HIGH);
  digitalWrite(csOrta_s3, HIGH);
  yesil = pulseIn(csOrta_out, LOW);
  yesil = map(yesil, 43, 210, 0, 100);
  Serial.print("Yeşil renk: ");
  Serial.print(yesil);
  Serial.print("\t");
  delay(1000);

  
  // Mavi rengi belirleme
  digitalWrite(csOrta_s2, LOW);
  digitalWrite(csOrta_s3, HIGH);
  mavi = pulseIn(csOrta_out, LOW);
  mavi = map(mavi, 40, 190, 0, 100);
  Serial.print("Mavi renk: ");
  Serial.print(mavi);
  Serial.println("\t");
  delay(1000);
}

void yumurtaRengiOKu() {
  // beyaz algılanıyor
  if (kirmizi < 0 and mavi < 0 and yesil < 0) {
    digitalWrite(kled, HIGH);
    digitalWrite(yled, HIGH);
    digitalWrite(mled, HIGH);
    yumurtaRengi = "beyaz";
    Serial.println("Beyaz zemin algılandı");
  }
  // kırmızı algılanıyor
  else if (kirmizi<90 and kirmizi < mavi and kirmizi < yesil) {
    digitalWrite(kled, HIGH);
    digitalWrite(yled, LOW);
    digitalWrite(mled, LOW);
    yumurtaRengi = "kirmizi";
    Serial.println("Kırmızı yumurta algılandı");
  }
  // mavi algılanıyor
  else if (mavi < kirmizi and mavi < yesil ) {
    digitalWrite(mled, HIGH);
    digitalWrite(kled, LOW);
    digitalWrite(yled, LOW);
    yumurtaRengi = "mavi";
    Serial.println("Mavi yumurta algılandı");
  }
}
