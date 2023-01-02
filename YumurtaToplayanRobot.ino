#include <Servo.h>

// ***************************
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

// MZ-80 Sensörleri için tanımlamalarımızı yapıyoruz
#define kLed 11           // Kırmızı ledlerimizi bağladık
#define yLed 10           // Ve dahi Yeşil ledimizi bağladık
#define mz80_SagOn A0     //sağ ön sensörümüzü A0 numaralı pine bağladık sensörümüzü
#define mz80_SolOn A1     //sol ön sensörümüzü A1 numaralı pine bağladık sensörümüzü
#define mz80_SagArka A2   //sağ arka sensörümüzü A2 numaralı pine bağladık sensörümüzü
#define mz80_SolArka A3   //sol arka sensörümüzü A3 numaralı pine bağladık sensörümüzü
#define mz80_Arka A4      //arka sensörümüzü A4 numaralı pine bağladık sensörümüzü

Servo servoOrta;
Servo servoArka;

int degreeOrta, degreeArka;
int yumurtaKirmizi, yumurtaYesil, yumurtaMavi = 0;
int zeminKirmizi, zeminYesil, zeminMavi = 0;

int yumurtaSayisi = 0;

bool start = true;        // ilk başta zemini algılayacak ve bu değişken bir defalığına bizimRengimiz değişkenine zemin değerini atadıktan sonra false değerini alarak yarışma sonuna kadar değişmeyecek
bool koseneGit = false;   // yumurta sayısı yeteri kadar olduğunda ya da belli bir süre geçtiğinde köşesine gidip yumurtaları bırakmak için tanımlandı
String bizimRengimiz;     // arkadaki renk sensörünün okuduğu değer atanacak
String yumurtaRengi;      // ortadaki renk sensörünün okuduğu değer atanacak
String zeminRengi;        // arkadaki renk sensörünün okuduğu değer atanacak,

int pos = 0;

void setup() {
  // *************************************
  // RENK SENSÖR AYARLARI
  pinMode(csOrta_s0, OUTPUT);  pinMode(csOrta_s1, OUTPUT);  pinMode(csOrta_s2, OUTPUT);  pinMode(csOrta_s3, OUTPUT);  pinMode(csOrta_out, INPUT);
  pinMode(csArka_s0, OUTPUT);  pinMode(csArka_s1, OUTPUT);  pinMode(csArka_s2, OUTPUT);  pinMode(csArka_s3, OUTPUT);  pinMode(csArka_out, INPUT);
  
  // Arduino için Frekans değerini ayarladık
  digitalWrite(csOrta_s0, HIGH);  digitalWrite(csOrta_s1, LOW);  digitalWrite(csArka_s0, HIGH);  digitalWrite(csArka_s1, LOW);
  //pinMode(kled, OUTPUT);  pinMode(yled, OUTPUT);  pinMode(mled, OUTPUT);

  // **************************************
  // MZ-80 MESAFE SENSÖR AYARLARI
  pinMode(kLed,OUTPUT);           // ledleri çıkış olarak tanımladık
  pinMode(yLed,OUTPUT);           // ledleri çıkış olarak tanımladık
  pinMode(mz80_SagOn, INPUT);     //Kızılotesi sensoru giriş olarak tanımladık
  pinMode(mz80_SolOn, INPUT);     //Kızılotesi sensoru giriş olarak tanımladık
  pinMode(mz80_SagArka, INPUT);   //Kızılotesi sensoru giriş olarak tanımladı
  pinMode(mz80_SolArka, INPUT);   //Kızılotesi sensoru giriş olarak tanımladık
  pinMode(mz80_Arka, INPUT);      //Kızılotesi sensoru giriş olarak tanımladı

  // **************************************
  // SERVO TANIMLARI
  servoOrta.attach(3);    // Orta Servo tanımlanıyor
  servoArka.attach(9);    // Arka Servo tanımlanıyor
    
  Serial.begin(9600);
}

void loop() {
  //kalibreEt();
  //kalibreEtZemin();
  ortaRenkSensorOku();
  arkaRenkSensorOku();
  yumurtaRengiOku();
  zeminRengiOku();

  //servoOrtaKalibreEt();
  servoOrtaDon(yumurtaRengi);
  //servoArkaDon(zeminRengi);

  // MZ-80 leri çalıştıryoruz
  mz80_SensorEngelAlgila();
  //mz80_SensorEngelAlgila(mz80_SolOn);
  //mz80_SolOnSensor();
  //mz80_SagArkaSensor();
  //mz80_SolArkaSensor();
  //mz80_ArkaSensor();  
}

// **************************************************************
// MZ-80 lerden gelen değerlere göre kırmızı ve yeşil ledleri yakıyoruz
void mz80_SensorEngelAlgila() {
  if (analogRead(mz80_SagOn) < 1000 || analogRead(mz80_SolOn) < 1000 || analogRead(mz80_SagArka) < 1000)  //engel yoksa kırmızı ledi söndür
  { digitalWrite(kLed, 1); digitalWrite(yLed, 0);}
  else                                //engel varsa kırmızı ledi yak
  { digitalWrite(kLed, 0); digitalWrite(yLed, 1);}
}




void mz80_SolOnSensor() {
  if (analogRead(mz80_SolOn) < 1000)  //engel yoksa kırmızı ledi söndür
  { digitalWrite(kLed, 1); digitalWrite(yLed, 0);}
  else                                //engel varsa kırmızı ledi yak
  { digitalWrite(kLed, 0); digitalWrite(yLed, 1);}
}
void mz80_SagArkaSensor() {
  if (analogRead(mz80_SagArka) < 1000)  //engel yoksa kırmızı ledi söndür
  { digitalWrite(kLed, 1); digitalWrite(yLed, 0);}
  else                                //engel varsa kırmızı ledi yak
  { digitalWrite(kLed, 0); digitalWrite(yLed, 1);}
}
void mz80_SolArkaSensor() {
  if (analogRead(mz80_SolArka) < 1000)  //engel yoksa kırmızı ledi söndür
  { digitalWrite(kLed, 1); digitalWrite(yLed, 0);}
  else                                //engel varsa kırmızı ledi yak
  { digitalWrite(kLed, 0); digitalWrite(yLed, 1);}
}
void mz80_ArkaSensor() {
  if (analogRead(mz80_Arka) < 1000)  //engel yoksa kırmızı ledi söndür
  { digitalWrite(kLed, 1); digitalWrite(yLed, 0);}
  else                                //engel varsa kırmızı ledi yak
  { digitalWrite(kLed, 0); digitalWrite(yLed, 1);}
}

// **************************************************************
// gelen renk değerlerine göre orta servo ayarlanıyor
void servoOrtaDon(String renk) {
  if (renk == "beyaz") {  // düz ol
    degreeOrta = 90;
    servoOrta.write(degreeOrta);
  } else if (bizimRengimiz == renk) {  // sola dön
    degreeOrta = 180;
    servoOrta.write(degreeOrta);
    yumurtaSayisi++;  // toplanlanan kendi yumurta sayımızı arttırıyoruz
  } else {  // sağa dön
    degreeOrta = 0;
    servoOrta.write(degreeOrta);
  }
}

// kendi rengimize geldiğinde açılacak
// rakip veya beyaz renge geldiğinde kapanacak
void servoArkaDon(String renk) {
  if (renk == "beyaz" || renk != bizimRengimiz) {
    degreeArka = 80;
    servoArka.write(degreeArka);
  } else {
    degreeArka = 20;
    servoArka.write(degreeArka);
  }
}

void ortaRenkSensorOku() {
  delay(50);
  // Kırmızı rengi belirleme
  digitalWrite(csOrta_s2, LOW);
  digitalWrite(csOrta_s3, LOW);
  yumurtaKirmizi = pulseIn(csOrta_out, LOW);
  yumurtaKirmizi = map(yumurtaKirmizi, 48, 180, 0, 100);
  /*
  Serial.print("Kırmızı renk: ");
  Serial.print(yumurtaKirmizi);
  Serial.print("\t");
  */
  delay(50);

  
  // Yesil rengi belirleme
  digitalWrite(csOrta_s2, HIGH);
  digitalWrite(csOrta_s3, HIGH);
  yumurtaYesil = pulseIn(csOrta_out, LOW);
  yumurtaYesil = map(yumurtaYesil, 43, 210, 0, 100);
  /*
  Serial.print("Yeşil renk: ");
  Serial.print(yumurtaYesil);
  Serial.print("\t");
  */
  delay(50);

  
  // Mavi rengi belirleme
  digitalWrite(csOrta_s2, LOW);
  digitalWrite(csOrta_s3, HIGH);
  yumurtaMavi = pulseIn(csOrta_out, LOW);
  yumurtaMavi = map(yumurtaMavi, 40, 190, 0, 100);
  /*
  Serial.print("Mavi renk: ");
  Serial.print(yumurtaMavi);
  Serial.println("\t");
  */
  delay(50);
}


void arkaRenkSensorOku() {
  delay(50);
  // Kırmızı ZEMİN rengi belirleme
  digitalWrite(csArka_s2, LOW);
  digitalWrite(csArka_s3, LOW);
  zeminKirmizi = pulseIn(csArka_out, LOW);
  zeminKirmizi = map(zeminKirmizi, 48, 180, 0, 100);
  /*
  Serial.print("ZEMİN Kırmızı renk: ");
  Serial.print(zeminKirmizi);
  Serial.print("\t");
  */
  delay(50);

  
  // Yesil ZEMİN rengi belirleme
  digitalWrite(csArka_s2, HIGH);
  digitalWrite(csArka_s3, HIGH);
  zeminYesil = pulseIn(csArka_out, LOW);
  zeminYesil = map(zeminYesil, 43, 210, 0, 100);
  /*
  Serial.print("ZEMİN Yeşil renk: ");
  Serial.print(zeminYesil);
  Serial.print("\t");
  */
  delay(50);

  
  // Mavi ZEMİN rengi belirleme
  digitalWrite(csArka_s2, LOW);
  digitalWrite(csArka_s3, HIGH);
  zeminMavi = pulseIn(csArka_out, LOW);
  zeminMavi = map(zeminMavi, 40, 190, 0, 100);
  /*
  Serial.print("ZEMİN Mavi renk: ");
  Serial.print(zeminMavi);
  Serial.println("\t");
  */
  delay(50);
}

void yumurtaRengiOku() {
  // Orta Sensör beyaz zemin algılanıyor
  if (yumurtaKirmizi < 0 and yumurtaMavi < 0 and yumurtaYesil < 0) {
    yumurtaRengi = "beyaz";
    Serial.println("Orta Sensör Beyaz zemin algıladı");
  }
  // Orta Sensör kırmızı algılanıyor
  else if (yumurtaKirmizi<90 and yumurtaKirmizi < yumurtaMavi and yumurtaKirmizi < yumurtaYesil) {
    yumurtaRengi = "kirmizi";
    Serial.println("Orta Sensör Kırmızı yumurta algıladı");
  }
  // Orta Sensör mavi algılanıyor
  else if (yumurtaMavi < yumurtaKirmizi and yumurtaMavi < yumurtaYesil ) {
    yumurtaRengi = "mavi";
    Serial.println("Orta Sensör Mavi yumurta algıladı");
  }
}

void zeminRengiOku() {
  // Arka Sensör beyaz zemin algılanıyor
  if (zeminKirmizi < 0 and zeminMavi < 0 and zeminYesil < 0) {
    zeminRengi = "beyaz";
    Serial.println("Arka Sensör Beyaz ZEMİN algıladı");
  }
  // Arka Sensör kırmızı zemin algılanıyor
  else if (zeminKirmizi<90 and zeminKirmizi < zeminMavi and zeminKirmizi < zeminYesil) {
    zeminRengi = "kirmizi";
    if (start == true) {  // ilk sefer çalışacak sonra çalışmayacak
      bizimRengimiz = "kirmizi";
      start = false;  
    }
    
    Serial.println("Arka Sensör Kırmızı ZEMİN algıladı");
  }
  // Arka Sensör mavi zemin algılanıyor
  else if (zeminMavi < zeminKirmizi and zeminMavi < zeminYesil ) {
    zeminRengi = "mavi";
    if (start == true) {  // ilk sefer çalışacak sonra çalışmayacak
      bizimRengimiz = "mavi";
      start = false;  
    }
    
    Serial.println("Arka Sensör Mavi ZEMİN algıladı");
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
