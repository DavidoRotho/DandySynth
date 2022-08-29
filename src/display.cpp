#include "header.h"


#if (SPI_INTERFACES_COUNT == 1)
	SPIClass* spi = &SPI;
#else
	SPIClass* spi = &SPI1;
#endif

__attribute__((optimize("unroll-loops")))
void DandyDisplay::errorScreen()
{
	std::vector<std::string> menuText;

	for (int i = 0; i < 5; i++)
		menuText.push_back("It's cooked!!!!");

	this->menu("RUH ROH", menuText, 0);
}

 void DandyDisplay::mainMenu()
{
	int encPos = this->encoderPos-this->encoderOffset;

	std::vector<std::string> menuText;

	menuText.push_back("Table Select");
	menuText.push_back("Matrix Select");
	menuText.push_back("Settings");
	menuText.push_back("Test");
	menuText.push_back("Test2");
	menuText.push_back("Test3");
	menuText.push_back("Test4");
	menuText.push_back("Test5");

	this->menu("Main Menu", menuText, encPos);
}


void DandyDisplay::menu(std::string title, std::vector<std::string> text, int encPos)
{
	tft->setTextColor(ST77XX_ORANGE);
	tft->setTextWrap(true);
	
	//tft->drawFastHLine(0, 0, 240, ST77XX_RED);
	tft->drawRect(1, 1, 239, 49, ST77XX_CYAN);

	tft->setCursor(10, 30);
	tft->print(title.c_str());

	int menuItemsLength = text.size();

	int menuOffset = min(max(encPos-5, 0), menuItemsLength-5);

	for (int i = 0; i < (min(5, menuItemsLength)); i++)
	{
		tft->setCursor(10, 50*i+80);

		int colour = ST77XX_RED;
		if (i+menuOffset == encPos)
			colour = ST77XX_CYAN;

		tft->drawRect(1, (50*i)+50, 239, 50, colour);
		tft->print(text[menuOffset+i].c_str());
	}
}

void DandyDisplay::encoderPosPush(int pos, int push)
{
	encoderPos = pos;
	encoderPush = push;
}

void DandyDisplay::connMatrix()
{

}

void DandyDisplay::waveFormDisplay()
{

}

DandyDisplay::DandyDisplay() // Constructor
{

}

void DandyDisplay::setupDisplay()
{
	tft = new Adafruit_ST7789(spi, TFT_CS, TFT_DC, TFT_RST);

	pinMode(TFT_BACKLIGHT, OUTPUT);
	digitalWrite(TFT_BACKLIGHT, HIGH); // Backlight on

	tft->init(240, 320);                // Init ST7789 240x320
	tft->setRotation(2);

	Serial.println(F("Initialized"));

	uint16_t time = millis();
	tft->fillScreen(ST77XX_BLACK);
	time = millis() - time;

	Serial.println(time, DEC);
	delay(500);

	tft->setFont(&FreeMonoBoldOblique12pt7b);

	// large block of text
	tft->fillScreen(ST77XX_BLACK);

	
	Serial.println("done");
	delay(1000);
}

void DandyDisplay::runDisplay()
{
	switch (currentDisplaymode)
	{
	case MAIN:
		mainMenu();
		break;
	case CONN:
		connMatrix();
		break;

	default:
		errorScreen();
		break;
	}
}