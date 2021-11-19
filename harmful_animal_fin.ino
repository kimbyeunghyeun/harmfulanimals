#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial MP3Module(12, 13);
DFRobotDFPlayerMini MP3Player;
void printDetail(uint8_t type, int value);
int ranNumber;

ICACHE_RAM_ATTR void mp3() {
  char m = 0;
  if (Serial.available()) {
    m = Serial.read(); // 받은 신호 저장
    Serial.println(m);

    switch (m) {
      case '1': //사람 탐지 - 사이렌 랜덤재생
        ranNumber = random(2, 12);
        MP3Player.play(ranNumber);
        Serial.end();
        delay(5000);
        Serial.begin(115200);
        Serial.write(int(ranNumber));
        break;

      case '2': //고라니, 멧돼지 탐지 - 호랑이 울음소리
        digitalWrite(4, HIGH);
        MP3Player.play(13);
        Serial.end();
        delay(5000);
        Serial.begin(115200);
        Serial.write("Boar Detected !!");
        break;

      case '3': //고라니,호랑이 탐지 야간 LED or 전자펜스
        
        break;

      case '4': //화재감지 사이렌 및 LED점등
        digitalWrite(16, HIGH);
        MP3Player.play(14);
        Serial.end();
        delay(5000);
        Serial.begin(115200);
        Serial.write("Fire Detected !!");
        break;

      case '0': // 재생 정지
        digitalWrite(16, LOW);
        digitalWrite(4, LOW);
        MP3Player.pause();
        break;
    }

    if (MP3Player.available()) {
      printDetail(MP3Player.readType(), MP3Player.read());
    }
  }
}

void setup () {
  MP3Module.begin(9600);
  Serial.begin (115200);

  pinMode(5, INPUT_PULLUP); //불꽃감지센서

  pinMode(16, OUTPUT); //불꽃감지LED
  pinMode(4, OUTPUT); //헤드라이트LED
  digitalWrite(16, LOW);
  digitalWrite(4, LOW);
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!MP3Player.begin(MP3Module)) { // MP3 모듈을 초기화합니다. 초기화에 실패하면 오류를 발생시킵니다.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));

  MP3Player.volume(15);  // 볼륨을 조절합니다. 0~30까지 설정이 가능합니다.
  MP3Player.play(1);    // 시작시 시작음 재생
  delay(2000);
}

void loop () {
  mp3();
}

void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out"));
      break;
    case WrongStack:
      Serial.println(F("WrongStack"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card In"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Out"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Insert");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Remove");
      break;
    case DFPlayerPlayFinished:
      Serial.println(F("Number"));
      Serial.println(value);
      Serial.println(F("Play Finished"));
      break;
    case DFPlayerError:
      Serial.println(F("Error"));
      break;
    default:
      break;
  }
}
