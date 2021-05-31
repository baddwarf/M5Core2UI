#include "M5Core2UI.h"

/*
 * 
 * UIobject : TFT_eSprite
 * 
 */

UIobject::UIobject(uint16_t posx, uint16_t posy, uint16_t width, uint16_t height)
{
  _width = width;
  _height = height;
  _posx = posx;
  _posy = posy;
  _type = UI_OT_OBJECT;
  _autoCreated = false;
  //this->createSprite(_width, _height);
}

void UIobject::setAutoCreated()
{
	_autoCreated = true;
}

bool UIobject::isAutoCreated()
{
	return(_autoCreated);
}

uint8_t UIobject::getType()
{
	return(_type);
}

void UIobject::printScreen()
{
  //this->pushSprite(_posx, _posy);
}

void UIobject::enable()
{
	// empty - template
}

void UIobject::disable()
{
	// empty - template
}

bool UIobject::isEnabled()
{
	return (true);
}

/*
 * 
 * UIbutton : UIobject
 * 
 */

UIbutton::UIbutton(uint16_t posx, uint16_t posy, uint16_t width, uint16_t height) :  UIobject(posx, posy, width, height),
	_touchzone(posx, posy, width, height)
{
	_caption = String("Hello");
	_txtColor = 0xFFFF;
	_bgColor = 0x761F;
	_enabled = true;
	_type = UI_OT_BUTTON;
	drawObject();
	//_touchzone = Button(posx, posy, width, height);
	//_color = 0x01F0;
}

void UIbutton::setCaption(String str)
{
  _caption = str;
//  drawObject();
}

bool UIbutton::wasPressed()
{
	return (_touchzone.wasPressed());
}

void UIbutton::disable()
{
	if (_enabled)
	{
	_touchzone.hide();
	_enabled = false;
	}
}

void UIbutton::enable()
{
	if (!_enabled)
	{
		_touchzone.draw();
		_enabled = true;
	}
}

bool UIbutton::isEnabled()
{
	return(_enabled);
}

void UIbutton::drawObject()
{
  M5.Lcd.fillRect(_posx, _posy, _width, _height, _bgColor);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextDatum(MC_DATUM);
  M5.Lcd.setTextColor(_txtColor);
  M5.Lcd.drawString(_caption, _posx + (_width / 2), _posy + (_height / 2));
}

void UIbutton::printScreen()
{
	drawObject();
}

void UIbutton::invertColor()
{
	uint16_t tmpColor;
	
	tmpColor = _bgColor;
	_bgColor = _txtColor;
	_txtColor = tmpColor;
	drawObject();
	printScreen();
}

/*
 * 
 * UIheader : TFT_eSprite
 * 
 */

UIheader::UIheader(uint16_t width, uint16_t height)
{
	_width = width;
	_height = height;
	_color = 0x0000;
	_bgColor = 0xFFFF;
	_left = "Page 1";
	_center = "00:00:00";
	_right = "100%";
}

void UIheader::drawObject()
{
	M5.Lcd.fillRect(0,0, _width, _height, _bgColor);
	M5.Lcd.setTextSize(1);
	M5.Lcd.setTextColor(_color);
	M5.Lcd.setTextDatum(ML_DATUM);
	M5.Lcd.drawString(_left, 10, _height / 2);
	M5.Lcd.setTextDatum(MC_DATUM);
	M5.Lcd.drawString(_center, _width / 2, _height / 2);
	M5.Lcd.setTextDatum(MR_DATUM);
	M5.Lcd.drawString(_right, _width, _height / 2);
}

void UIheader::setLeft(String S)
{
	_left = S;
}

void UIheader::setCenter(String S)
{
	_center = S;
}

void UIheader::setRight(String S)
{
	_right = S;
}

void UIheader::setColors(uint16_t color, uint16_t bgColor)
{
	Serial.print("Colors : ");
	Serial.print(color, HEX);
	Serial.print(", ");
	Serial.println(bgColor, HEX);
	_color = color;
	_bgColor = bgColor;
}

/*
 * 
 * UIpage : TFT_eSprite
 * 
 */

UIpage::UIpage(uint16_t width, uint16_t height)
{
  _height = _height;
  _width = width;
  _objectCount = 0;
  _isFull = false;
  _bgColor = 0xFFFF; // defautl background color
  _pageObjects = new UIobject *[MAX_PAGE_OBJECT];
  for (int i = 0; i < MAX_PAGE_OBJECT; i++)
    _pageObjects[i] = nullptr;
  M5.Lcd.fillRect(0, 0, _width, _height, _bgColor);
}


void UIpage::addObject(UIobject *object)
{
  if (!_isFull && _objectCount < MAX_PAGE_OBJECT)
  {
    _pageObjects[_objectCount] = object;
    _objectCount++;
    if (_objectCount == MAX_PAGE_OBJECT)
      _isFull = true;
  }
  else
    Serial.println("[UI] Can't add page - UI is full");
}

void UIpage::setTitle(String title)
{
  _title = title;
}

String UIpage::getTitle()
{
  return (_title);
}

UIobject *UIpage::getObject(uint16_t idx)
{
  if ((idx >= 0) && (idx < MAX_PAGE_OBJECT))
    return (_pageObjects[idx]);
}

void UIpage::printScreen()
{
  for (int i = 0; i < _objectCount; i++)
    _pageObjects[i]->printScreen();
}

void UIpage::disableButtons()
{
	for (int i = 0; i < _objectCount; i++)
	{
		if (_pageObjects[i]->getType() == UI_OT_BUTTON)
			_pageObjects[i]->disable();
	}
}

void UIpage::enableButtons()
{
	for (int i = 0; i < _objectCount; i++)
	{
		if (_pageObjects[i]->getType() == UI_OT_BUTTON)
			_pageObjects[i]->enable();
	}
}

uint8_t UIpage::getObjectCount()
{
	return(_objectCount);
}

/*
 * 
 * M5core2UI
 * 
 */

m5core2UI::m5core2UI(uint16_t width, uint16_t height) : _header(width, height)
{
  _pageCount = 0;
  _nextPage = 0;
  _currentPage = 0;
  _isFull = false;
  _pages = new UIpage *[MAX_PAGES];
  for (int i = 0; i < MAX_PAGES; i++)
    _pages[i] = nullptr;
}

void m5core2UI::setHeaderColors(uint16_t color, uint16_t bgColor)
{
	_header.setColors(color, bgColor);
}

void m5core2UI::drawHeader()
{
	_header.drawObject();
}

void m5core2UI::addPage(UIpage *page)
{
  if (!_isFull)
  {
    _pages[_nextPage] = page;
	_pages[_nextPage]->disableButtons(); // disable touch buttons at load
    if (_nextPage < (MAX_PAGES - 1))
    {
      _nextPage++;
      _pageCount++;
    }
    else
      _isFull = true;
  }
  else
    Serial.println("[UI] Can't add page - UI is full");
}

void m5core2UI::removePage(uint8_t page)
{
  Serial.println("[UI] Remove page " + String(page));
}

void m5core2UI::showUI()
{
  _pages[_currentPage]->enableButtons(); // ensure buttons are enabled at show
  _pages[_currentPage]->printScreen();
  _header.setLeft("Page " + String(_currentPage + 1));
  _header.drawObject();
}

UIpage *m5core2UI::getPage(uint8_t idx)
{
  if ((idx >= 0) && (idx < MAX_PAGES))
    return (_pages[idx]);
  else
    return (nullptr);
}

UIpage *m5core2UI::getCurrentPage()
{
	return(_pages[_currentPage]);
}

void m5core2UI::previousPage()
{
	_pages[_currentPage]->disableButtons();
  if (_pageCount > 1)
  {
    if (_currentPage > 0)
      _currentPage--;
    else
      _currentPage = (_pageCount - 1);
  }
  _pages[_currentPage]->enableButtons();
  _pages[_currentPage]->printScreen();
  _header.setLeft("Page " + String(_currentPage + 1));
  _header.drawObject();
}

void m5core2UI::nextPage()
{
	_pages[_currentPage]->disableButtons();
  if (_pageCount > 1)
  {
    if (_currentPage < (_pageCount - 1))
      _currentPage++;
    else
      _currentPage = 0;
  }
  _pages[_currentPage]->enableButtons();
  _pages[_currentPage]->printScreen();
  _header.setLeft("Page " + String(_currentPage + 1));
  _header.drawObject();
}

void m5core2UI::setHeaderLeft(String S)
{
	_header.setLeft(S);
}

void m5core2UI::setHeaderCenter(String S)
{
	_header.setCenter(S);
}

void m5core2UI::setHeaderRight(String S)
{
	_header.setRight(S);
}

uint8_t m5core2UI::getCurrentPageIndex()
{
	return(_currentPage + 1);
}