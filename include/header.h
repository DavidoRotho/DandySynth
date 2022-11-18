#ifndef __HEADER
#define __HEADER

#include <vector>
#include <string>
#include <MIDI.h>
#include <Encoder.h>
#include "MCP_DAC.h"
#include <SPI.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>


#define OSCIS 4
#define BUFSIZE 64
#define WAVETABLE_SIZE 16
#define WAVE_SIZE 361


// Because of the limited number of pins available on the Circuit Playground Boards
// Software SPI is used
#define TFT_CS        31
#define TFT_RST       28 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC        29
#define TFT_BACKLIGHT PIN_A3 // Display backlight pin


class DandyDisplay
{
public:
    DandyDisplay();
    void setupDisplay();
    void runDisplay();

    void encoderPosPush(int pos, int push);

private:
    void mainMenu();
    void connMatrix();
    void waveFormDisplay();
    void errorScreen();
    void title(std::string title);

    void menu(std::string title, std::vector<std::string> text, int encPos);

    enum displayMode
    {
        MAIN,
        CONN,
        WAVE
    };

    enum displayMode currentDisplaymode = MAIN;


    signed int encoderOffset = 0;

    signed int encoderPos = 0;
    int encoderPush = 0;

    Adafruit_ST7789 *tft;
};


class DandySynth
{
private:

    DandyDisplay *display;

    typedef enum WAVE_TYPE { SINE, SQUARE, TRIANGLE, COMBO, REAL, WAVETABLE } WAVE_TYPE;

    struct EnvelopeSettings
    {
        float attackStartValue;
        float attackEndValue;
        float attackTimeEnd;
        
        float sustainValue;
        float sustainTimeEnd;

        float releaseValueStart;
        float releaseValueEnd;
        float releaseTime; // Higher is quicker
    };

    struct EnvelopeSettings basicOneShotEnv = {
        .attackStartValue = 0.0,
        .attackEndValue = 1.0,
        .attackTimeEnd = 0.1,
        .sustainValue = 0.9,
        .sustainTimeEnd = 0.5,
        .releaseValueStart = 0.9,
        .releaseValueEnd = 0.0,
        .releaseTime = 0.3
    };

public:
    float p0, p1, p2, p3, p4, p5;

    float velParam;

    float pi = 3.1415926;

	#define LFOS 4
	float lfoWaveValues[LFOS];

    // LOOKUP TABLES
    float sine[361];
    float square[361];
    float table[WAVETABLE_SIZE][WAVE_SIZE];

    MCP4921 MCP;


    int16_t outBuffer[BUFSIZE]; // For wave output

    int bufPos = 0;
    int encPos = 0;
    int encPush = 0;
    float fmFreq = 1.;
    int waveTablePos = 0;

    static byte controlValues[256];
    static byte lastCCNum;
    static byte lastCCVal;
    static uint32_t noteTime;
    static byte lastNote;
    static byte lastChan;
    static byte lastVel;
    static uint32_t noteTimes[OSCIS+1];
    static byte lastNotes[OSCIS+1];


    void generateWaveTables();
    void processLFOs(int now);

    float getOscilatorsOuput(int now);

    float getNoteWave(float freq, int now, int slice);
    float getNoteSquare(float freq, int now);
    float getNoteSine(float freq, int now);
    float getNoteSineR(float freq, int now);
    float getNoteSineGoofy(float freq, int now);
    float freqTo360Index(float freq, int now);
    float freqToRadIndex(float freq, int now);

    float sampleWave(WAVE_TYPE waveType, float freq, int now, float param);

    static void handleNoteOn(byte channel, byte pitch, byte velocity);
    static void handleMIDIControlChange(byte channel, byte pitch, byte velocity);
    float envelope(float t, struct DandySynth::EnvelopeSettings set);

    void run(uint32_t now);
    void setup();

    float noteToFreq(float note);


    void setP0(float v);
    void setP1(float v);
    void setP2(float v);
    void setP3(float v);
    void setP4(float v);
    void setP5(float v);


    void setEncPos(int pos);
    void setEncPush(int pushed);

};

#endif