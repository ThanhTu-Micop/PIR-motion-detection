#define BLYNK_TEMPLATE_ID "TMPL6AK6T1Q8h"
#define BLYNK_TEMPLATE_NAME "niceh"
#define BLYNK_AUTH_TOKEN "???????????????????"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <BlynkSimpleEsp32.h>
#include "time.h"

// ========== PHẦN CỨNG ==========
#define PIR     27
#define BUZZER  26
#define LED     25
#define BUTTON  14

// ========== MẠNG ==========
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

// ========== THINGSPEAK ==========
// Copy Write API Key của ThingSpeak Channel vào đây
String THINGSPEAK_API_KEY = "??????????????????";
const char* THINGSPEAK_SERVER = "http://api.thingspeak.com/update";

// Channel fields:
// field1 = detectionCount
// field2 = hour
// field3 = minute
const unsigned long THINGSPEAK_INTERVAL = 15000;
unsigned long lastThingSpeakSend = 0;

// ========== THỜI GIAN ==========
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7 * 3600;
const int daylightOffset_sec = 0;

// ========== BIẾN TRẠNG THÁI ==========
bool armed = true;
bool lastMotionState = false;
unsigned long lastMotionTime = 0;
unsigned long lastButtonPress = 0;
bool lastButtonState = HIGH;
int detectionCount = 0;

// ========== THAM SỐ ==========
const unsigned long MOTION_COOLDOWN = 10000;
const unsigned long BUTTON_DEBOUNCE = 300;
const unsigned long ALARM_DURATION = 2000;

BlynkTimer timer;
unsigned long alarmStartTime = 0;
bool alarmActive = false;

// ========== HÀNG ĐỢI GỬI THINGSPEAK ==========
// Lưu tạm khi mất WiFi hoặc chưa đủ 15 giây
struct LogEntry {
  int count;
  String timestamp;
  String date;
  int hour;
  int minute;
};

const int QUEUE_SIZE = 20;
LogEntry logQueue[QUEUE_SIZE];
int queueHead = 0;
int queueTail = 0;
int queueLen = 0;

void enqueue(LogEntry e) {
  if (queueLen >= QUEUE_SIZE) {
    Serial.println("⚠️ Queue đầy, bỏ qua bản ghi cũ nhất");

    // Ghi đè bản ghi cũ nhất
    logQueue[queueTail] = e;
    queueTail = (queueTail + 1) % QUEUE_SIZE;
    queueHead = (queueHead + 1) % QUEUE_SIZE;
    return;
  }

  logQueue[queueTail] = e;
  queueTail = (queueTail + 1) % QUEUE_SIZE;
  queueLen++;
}

bool dequeue(LogEntry& e) {
  if (queueLen <= 0) return false;

  e = logQueue[queueHead];
  queueHead = (queueHead + 1) % QUEUE_SIZE;
  queueLen--;
  return true;
}

// ─────────────────────────────────────────────
// LẤY TIMESTAMP
// ─────────────────────────────────────────────
struct MyTimeInfo {
  String timestamp;
  String date;
  int hour;
  int minute;
};

MyTimeInfo getTimeInfo();

MyTimeInfo getTimeInfo() {
  struct tm t;
  MyTimeInfo ti;

  if (!getLocalTime(&t)) {
    unsigned long s = millis() / 1000;
    ti.hour = (s / 3600) % 24;
    ti.minute = (s / 60) % 60;

    char buf[30];
    sprintf(buf, "%02d:%02d:%02d --/--/----", ti.hour, ti.minute, (int)(s % 60));

    ti.timestamp = String(buf);
    ti.date = "--/--/----";
    return ti;
  }

  ti.hour = t.tm_hour;
  ti.minute = t.tm_min;

  char ts[25], dt[12];
  strftime(ts, sizeof(ts), "%H:%M:%S %d/%m/%Y", &t);
  strftime(dt, sizeof(dt), "%d/%m/%Y", &t);

  ti.timestamp = String(ts);
  ti.date = String(dt);
  return ti;
}

// ─────────────────────────────────────────────
// ĐỒNG BỘ DỮ LIỆU LÊN BLYNK
// ─────────────────────────────────────────────
void syncBlynkStatus() {
  if (!Blynk.connected()) return;

  Blynk.virtualWrite(V0, alarmActive ? 1 : 0);      // Alarm LED/status
  Blynk.virtualWrite(V1, getTimeInfo().timestamp);  // Latest time
  Blynk.virtualWrite(V2, armed ? 1 : 0);            // Armed switch
  Blynk.virtualWrite(V3, detectionCount);           // Counter
}

// ─────────────────────────────────────────────
// GỬI LÊN THINGSPEAK
// field1 = detectionCount
// field2 = hour
// field3 = minute
// ─────────────────────────────────────────────
bool sendToThingSpeak(const LogEntry& e) {
  if (WiFi.status() != WL_CONNECTED) return false;

  if (THINGSPEAK_API_KEY.indexOf("PASTE_YOUR") >= 0 || THINGSPEAK_API_KEY.length() < 8) {
    Serial.println("⚠️ Chưa điền THINGSPEAK_API_KEY!");
    return false;
  }

  unsigned long now = millis();

  // ThingSpeak nên gửi cách nhau tối thiểu 15 giây
  if (now - lastThingSpeakSend < THINGSPEAK_INTERVAL) {
    return false;
  }

  HTTPClient http;

  String url = String(THINGSPEAK_SERVER) + "?api_key=" + THINGSPEAK_API_KEY + "&field1=" + String(e.count) + "&field2=" + String(e.hour) + "&field3=" + String(e.minute);

  http.begin(url);
  int code = http.GET();

  bool ok = (code == 200);

  if (ok) {
    String response = http.getString();

    if (response.toInt() > 0) {
      Serial.println("✅ ThingSpeak OK: #" + String(e.count) + " | hour=" + String(e.hour) + " | minute=" + String(e.minute));
    } else {
      Serial.println("⚠️ ThingSpeak trả về 0: gửi quá nhanh hoặc key/channel sai");
      ok = false;
    }
  } else {
    Serial.println("❌ ThingSpeak lỗi HTTP " + String(code));
  }

  http.end();

  if (ok) {
    lastThingSpeakSend = now;
  }

  return ok;
}

// Gửi từng bản ghi trong queue, không block chương trình
void flushQueue() {
  if (queueLen <= 0) return;
  if (WiFi.status() != WL_CONNECTED) return;

  unsigned long now = millis();

  // Chưa đủ 15 giây thì đợi lần timer sau
  if (now - lastThingSpeakSend < THINGSPEAK_INTERVAL) return;

  LogEntry e = logQueue[queueHead];

  if (sendToThingSpeak(e)) {
    queueHead = (queueHead + 1) % QUEUE_SIZE;
    queueLen--;
    Serial.println("📤 Đã gửi queue, còn lại: " + String(queueLen));
  }
}

// ─────────────────────────────────────────────
// BLYNK NON-BLOCKING RECONNECT
// ─────────────────────────────────────────────
void checkConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠️ WiFi mất kết nối");
    return;
  }

  if (!Blynk.connected()) {
    Serial.println("⚠️ Reconnect Blynk...");
    Blynk.connect(3000);
  }

  flushQueue();
}

// ─────────────────────────────────────────────
// KIỂM TRA NÚT NHẤN
// ─────────────────────────────────────────────
void checkButton() {
  bool cur = digitalRead(BUTTON);
  unsigned long now = millis();

  if (cur == LOW && lastButtonState == HIGH && now - lastButtonPress > BUTTON_DEBOUNCE) {
    armed = !armed;

    Serial.println(armed ? "🔒 NÚT: BẬT HỆ THỐNG" : "🔓 NÚT: TẮT HỆ THỐNG");

    if (!armed) {
      digitalWrite(BUZZER, LOW);
      digitalWrite(LED, LOW);
      alarmActive = false;
    }

    syncBlynkStatus();

    // Nháy LED báo đã bấm nút
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED, HIGH);
      delay(80);
      digitalWrite(LED, LOW);
      delay(80);
    }

    lastButtonPress = now;
  }

  lastButtonState = cur;
}

// ─────────────────────────────────────────────
// KIỂM TRA PIR — luôn chạy dù Blynk/WiFi mất
// ─────────────────────────────────────────────
void checkMotion() {
  if (!armed) {
    if (alarmActive || digitalRead(LED) || digitalRead(BUZZER)) {
      digitalWrite(BUZZER, LOW);
      digitalWrite(LED, LOW);
      alarmActive = false;
      syncBlynkStatus();
    }
    return;
  }

  bool motion = digitalRead(PIR);
  unsigned long now = millis();

  if (motion && now - lastMotionTime > MOTION_COOLDOWN) {
    alarmActive = true;
    alarmStartTime = now;

    digitalWrite(BUZZER, HIGH);
    digitalWrite(LED, HIGH);

    detectionCount++;

    MyTimeInfo ti = getTimeInfo();
    Serial.println("🚨 #" + String(detectionCount) + " — " + ti.timestamp);

    // Đồng bộ lên Blynk
    if (Blynk.connected()) {
      Blynk.virtualWrite(V0, 1);
      Blynk.virtualWrite(V1, ti.timestamp);
      Blynk.virtualWrite(V2, armed ? 1 : 0);
      Blynk.virtualWrite(V3, detectionCount);
    }

    // Tạo log cho ThingSpeak
    LogEntry entry = { detectionCount, ti.timestamp, ti.date, ti.hour, ti.minute };

    // Gửi ngay nếu được, nếu không thì đưa vào queue
    if (!sendToThingSpeak(entry)) {
      enqueue(entry);
      Serial.println("📥 Đưa vào queue ThingSpeak, queue: " + String(queueLen));
    }

    lastMotionTime = now;
  }

  // Tắt còi sau ALARM_DURATION
// Tắt LED + Buzzer sau ALARM_DURATION
  if (alarmActive && now - alarmStartTime > ALARM_DURATION) {
  digitalWrite(BUZZER, LOW);
  digitalWrite(LED, LOW);

  alarmActive = false;
  syncBlynkStatus();
  }

  }
// ─────────────────────────────────────────────
// BLYNK CALLBACKS
// ─────────────────────────────────────────────

// V2: Switch bật/tắt hệ thống từ app Blynk
BLYNK_WRITE(V2) {
  armed = (param.asInt() == 1);

  Serial.println(armed ? "📱 BLYNK: BẬT HỆ THỐNG" : "📱 BLYNK: TẮT HỆ THỐNG");

  if (!armed) {
    digitalWrite(BUZZER, LOW);
    digitalWrite(LED, LOW);
    alarmActive = false;
  }

  syncBlynkStatus();
}

// V4: Button reset counter từ app Blynk
BLYNK_WRITE(V4) {
  if (param.asInt() == 1) {
    detectionCount = 0;
    Serial.println("🔄 Reset bộ đếm");

    if (Blynk.connected()) {
      Blynk.virtualWrite(V3, 0);
      Blynk.virtualWrite(V1, "Da reset bo dem");
    }

    // Gửi trạng thái reset lên ThingSpeak nếu đủ thời gian
    MyTimeInfo ti = getTimeInfo();
    LogEntry entry = { detectionCount, ti.timestamp, ti.date, ti.hour, ti.minute };

    if (!sendToThingSpeak(entry)) {
      enqueue(entry);
      Serial.println("📥 Queue bản ghi reset ThingSpeak");
    }
  }
}

BLYNK_CONNECTED() {
  Serial.println("✅ Blynk OK!");

  Blynk.syncVirtual(V2);

  Blynk.virtualWrite(V0, alarmActive ? 1 : 0);
  Blynk.virtualWrite(V1, "Cho phat hien...");
  Blynk.virtualWrite(V3, detectionCount);

  flushQueue();
}

// ─────────────────────────────────────────────
// SETUP
// ─────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n╔════════════════════════════════════╗");
  Serial.println("║ PIR Alarm + Blynk + ThingSpeak     ║");
  Serial.println("╚════════════════════════════════════╝");

  pinMode(PIR, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  digitalWrite(BUZZER, LOW);
  digitalWrite(LED, LOW);

  // WiFi
  Serial.print("📡 WiFi...");
  WiFi.begin(ssid, pass);

  unsigned long t0 = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 15000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi OK — " + WiFi.localIP().toString());
  } else {
    Serial.println("\n⚠️ WiFi chưa được — hệ thống chạy offline");
  }

  // Blynk non-blocking
  Blynk.config(auth);
  Blynk.connect(5000);

  // NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  delay(2000);
  Serial.println("⏰ NTP đồng bộ xong");

  // Timer
  timer.setInterval(100L, checkButton);
  timer.setInterval(150L, checkMotion);
  timer.setInterval(15000L, checkConnection);
  timer.setInterval(15000L, flushQueue);

  // Feedback LED
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
  }

  Serial.println("\n✅ SẴN SÀNG!");
  Serial.println("📱 Blynk: điều khiển bật/tắt + reset");
  Serial.println("📊 ThingSpeak: field1=count, field2=hour, field3=minute");
}

// ─────────────────────────────────────────────
// LOOP
// ─────────────────────────────────────────────
void loop() {
  if (Blynk.connected()) {
    Blynk.run();
  }

  timer.run();
}
