//
// Created by WauHu on 08.06.2024.
//
#include "ComponentApp.h"
#include "components/MenuComponent.h"
#include "M5Cardputer.h"
#include <SD.h>
#include <FS.h>
#include "ScreenManager.h"
#include "Utils.h"
#include "Audio.h"
#include "icon.h"

class AudioRecorder : public ComponentApp {
private:
    Audio audio;
public:
    Component &initApp() override {
        M5Cardputer.Speaker.stop();
        M5Cardputer.Speaker.end();
        audio.setPinout(41, 43, 42);
        std::vector<Action> actions;
        actions.emplace_back("record", "New recording", "", "", [this]() {
            M5Cardputer.Mic.begin();
            Utils::initCanvas();
            StatusBar::draw(true);
            canvas.setTextSize(1.5);
            canvas.drawString("Recording name", 7, 30);
            canvas.drawFastHLine(5, 45, 230, WHITE);
            canvas.drawFastHLine(5, 60, 230, WHITE);
            canvas.setCursor(8, 47);
            canvas.drawString("Press G0 to cancel", 7, 65);
            canvas.pushSprite(0, 0);
            String name = "";
            Utils::waitForInput(name);
            if (name == "" || name == "exit") {
                this->currentComponent->forceRerender();
                return;
            }
            // show recording screen
            Utils::initCanvas();
            StatusBar::draw(true);
            canvas.setTextSize(1.5);
            canvas.drawCenterString("Please wait...", 120, 40);
            canvas.pushSprite(0, 0);

            // prepare recording
            static constexpr const size_t record_length = 1920;
            static constexpr const size_t record_samplerate = 16000;
            int16_t rec_data[record_length];
            File recordFile;

            // Open file for recording
            if (!SD.exists("/hydra/rec/")) {
                SD.mkdir("/hydra/rec/");
            }
            if (SD.exists("/hydra/rec/" + name + ".wav")) {
                Utils::popup("File already exists", 0, 1.5);
                this->currentComponent->forceRerender();
                return;
            }

            recordFile = SD.open("/hydra/rec/" + name + ".wav", FILE_WRITE, true);
            if (!recordFile) {
                Utils::popup("Core Error\nFailed to open file", 0, 1.5);
                this->currentComponent->forceRerender();
                return;
            }

            // Write WAV header with placeholder data size
            saveWavHeader(recordFile, record_samplerate, 0xFFFFFFFF); // maximum 32-bit unsigned integer

            // begin recording
            // if key is pressed, wait for key release
            if (M5Cardputer.Keyboard.isPressed()) {
                while (M5Cardputer.Keyboard.isPressed()) {
                    delay(50);
                }
            }
            // record until key is pressed
            while (true) {
                M5Cardputer.update();
                // show recording screen
                Utils::initCanvas();
                StatusBar::draw(true);
                canvas.setTextSize(1.5);
                canvas.drawCenterString("Recording...", 120, 40);
                canvas.drawCenterString("Press any key to stop", 120, 60);
                canvas.pushSprite(0, 0);
                if (M5Cardputer.Mic.record(rec_data, record_length, record_samplerate)) {
                    // Write audio data to file
                    recordFile.write((uint8_t *)rec_data, record_length * sizeof(int16_t));
                } else {
                    Utils::popup("Core Error\nFailed to record", 0, 1.5);
                    this->currentComponent->forceRerender();
                    return;
                }
                if (M5.BtnA.wasPressed() || M5Cardputer.Keyboard.isPressed()) {
                    break;
                }
            }

            // end recording
            recordFile.close();
            Utils::initCanvas();
            StatusBar::draw(true);
            canvas.setTextSize(1.5);
            canvas.drawCenterString("Recording stopped", 120, 40);
            canvas.setTextSize(1);
            canvas.drawCenterString("Press any key to continue", 120, 60);
            canvas.pushSprite(0, 0);
            Utils::waitForKey();
            this->currentComponent->forceRerender();
        });

        actions.emplace_back("list", "List recordings", "", "", [this]() {
            Utils::initCanvas();
            StatusBar::draw(true);
            canvas.setTextSize(1.5);
            canvas.drawString("Please wait...", 7, 30);
            canvas.pushSprite(0, 0);
            File dir = SD.open("/hydra/rec");
            if (!dir) {
                Utils::popup("Core Error\nFailed to open directory", 0, 1.5);
                this->currentComponent->forceRerender();
                return;
            }
            std::vector<Action> recordings;
            while (true) {
                File entry = dir.openNextFile();
                if (!entry) {
                    break;
                }
                if (!entry.isDirectory()) {
                    // copy name to avoid dangling pointer
                    String name = entry.name();
                    recordings.emplace_back(name.c_str(), name.c_str(), "", "", [this, name]() {
                        Utils::initCanvas();
                        StatusBar::draw(true);
                        canvas.setTextSize(1.5);
                        canvas.drawCenterString("Playing...", 120, 40);
                        audio.setVolume(20);
                        audio.connecttoFS(SD, ("/hydra/rec/" + name).c_str());
                        while (audio.isRunning()) {
                            audio.loop();
                            M5Cardputer.update();
                            if (M5.BtnA.wasPressed() || M5Cardputer.Keyboard.isPressed()) {
                                audio.stopSong();
                                break;
                            }
                            Utils::initCanvas();
                            StatusBar::draw(true);
                            canvas.setTextSize(1.5);
                            canvas.drawCenterString("Playing...", 120, 40);
                            canvas.drawCenterString(name, 120, 60);
                            canvas.drawCenterString(String(audio.getAudioCurrentTime()) + "s/" + String(audio.getAudioFileDuration()) + "s", 120, 80);
                            canvas.pushSprite(0, 0);
                        }
                        this->currentComponent->forceRerender();
                    });
                }
                entry.close();
            }
            dir.close();

            recordings.emplace_back("back", "<- Back", "", "", [this]() {
                AppManager::getInstance().closeCurrentApp();
                AppManager::getInstance().openApp("AudioRecorder");
            });

            this->setComponent(new MenuComponent(*this, MENU_TYPE_LIST, recordings));
        });

        return *new MenuComponent(*this, MENU_TYPE_LIST, actions);
    }

    const uint8_t *getIcon() override {
        return rec_icon;
    }

    int getIconSize() override {
        return sizeof(rec_icon);
    }

    // Generate WAV header
    static void saveWavHeader(File file, uint32_t sampleRate, uint32_t dataSize) {
        char wav_header[44];
        uint32_t byteRate = sampleRate * 1 * sizeof(int16_t);
        uint16_t blockAlign = 1 * sizeof(int16_t);
        uint16_t bitsPerSample = 16;

        memcpy(wav_header, "RIFF", 4);
        uint32_t chunkSize = 36 + dataSize;
        memcpy(wav_header + 4, &chunkSize, 4);
        memcpy(wav_header + 8, "WAVE", 4);
        memcpy(wav_header + 12, "fmt ", 4);
        uint32_t subChunk1Size = 16;
        memcpy(wav_header + 16, &subChunk1Size, 4);
        uint16_t audioFormat = 1; // PCM
        memcpy(wav_header + 20, &audioFormat, 2);
        uint16_t numChannels = 1;
        memcpy(wav_header + 22, &numChannels, 2);
        memcpy(wav_header + 24, &sampleRate, 4);
        memcpy(wav_header + 28, &byteRate, 4);
        memcpy(wav_header + 32, &blockAlign, 2);
        memcpy(wav_header + 34, &bitsPerSample, 2);
        memcpy(wav_header + 36, "data", 4);
        memcpy(wav_header + 40, &dataSize, 4);

        file.write((uint8_t *)wav_header, 44);
    }
};