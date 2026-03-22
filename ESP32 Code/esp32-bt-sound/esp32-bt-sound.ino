/**
 * @file streams-talkie-a2dp.ino
 * @author Phil Schatzmann
 * @copyright GPLv3
 * Using TalkiePCM to generate audio to be sent to a Bluetooth Speaker 
 * Needs Arduino-LibHelix & Arduino-AudioTools by schatzmann
 */

// https://www.101soundboards.com/tts/697666-kobe-bryant-hd-tts-computer-ai-voice
 // https://github.com/pschatzmann/arduino-libhelix/releases
 // https://github.com/pschatzmann/arduino-audio-tools/releases

 // https://github.com/pschatzmann/ESP32-A2DP?tab=readme-ov-file#a2dp-source-music-sender

#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "AudioTools/Communication/A2DPStream.h"
#include "TalkiePCM.h" // https://github.com/pschatzmann/TalkiePCM
#include "Vocab_US_Large.h"
#include "Vocab_US_Gemini.h"

// LibHelix
#include <AACDecoderHelix.h>
#include <CommonHelix.h>
#include <MP3DecoderHelix.h>
#include <ConfigHelix.h>


//const char* name = "IE PRO BT Module";  // Replace with your device name
// 第一次配对时：填写你的确切耳机名字并让耳机进入配对模式（例如 "Borui Pixel Buds Pro" 或者 "[Unknown]"）
// 成功连接一次后：把名字改成 ""（留空），这样 ESP32 每次重启就会直接读取芯片底层记忆的 MAC 地址进行“秒连”，不需要耳机再进配对模式！
const char* name = "Borui Pixel Buds Pro";

AudioInfo from(8000, 2, 16);  // TTS
AudioInfo to(44100, 2, 16);   // A2DP

A2DPStream a2dp;
FormatConverterStream out(a2dp);
// talkie is submitting too many individual samples, so we buffer them
BufferedStream bs(1024, out); 
TalkiePCM voice(bs, from.channels);

void setup() {
  Serial.begin(115200);
  
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning); // Changed to Debug to see all scanned devices
  Serial.println("Starting...");

  // setup conversion to provide stereo at 44100hz
  out.begin(from, to);

  // setup a2dp
  auto cfg = a2dp.defaultConfig(TX_MODE);
  cfg.name = name;
  cfg.silence_on_nodata = true;  // allow delays with silence
  cfg.wait_for_connection = false; // We will handle the wait ourselves to show feedback
  cfg.auto_reconnect = true;     // <--- Added: Save MAC address to reconnect without scanning name
  a2dp.begin(cfg);
  a2dp.setVolume(0.3);

  Serial.print("Waiting for connection to ");
  Serial.println(name);
  while (!a2dp.isConnected()) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nA2DP Started");
}

void loop() {
  voice.say(sp2_DANGER);
  Serial.println("Danger...");
  voice.say(sp2_DANGER);

  voice.silence(500); // add a tiny pause
  voice.say(sp_MAN_KOBE);
  Serial.println("Man");
  voice.silence(500);
  
  voice.say(sp2_INTRUDER);
  Serial.println("Intruder...");
  voice.say(sp2_ALERT);
  Serial.println("Alert...");
  voice.say(sp4_LEFT);
  Serial.println("Left...");
  voice.say(sp2_SIX);
  Serial.println("Six...");
  voice.say(sp2_METER);
  Serial.println("Meter...");
  
  voice.silence(500);
  voice.say(sp2_FIVE);
  Serial.println("FIVE...");
  voice.say(sp2_METER);
  Serial.println("Meter...");

  voice.silence(500);
  voice.say(sp2_FIVE);
  Serial.println("Five...");
  voice.say(sp2_METER);
  Serial.println("Meter...");

  voice.silence(500);
  voice.say(sp2_TWO);
  Serial.println("Two...");
  voice.say(sp2_METER);
  Serial.println("Meter...");

  bs.flush();
  voice.silence(3000);
}
