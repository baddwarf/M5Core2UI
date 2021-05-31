#include "Arduino.h"
#include "M5Core2.h"

#ifndef _M5_CORE2UI_H_
#define _M5_CORE2UI_H_

#define MAX_PAGES 8
#define MAX_PAGE_OBJECT 16

enum UIObjectType : uint8_t
{
	UI_OT_OBJECT = 1,
	UI_OT_BUTTON = 2, // object with touch zone
};

class UIobject
{
  protected:

    uint16_t _posx;
    uint16_t _posy;
    uint16_t _width;
    uint16_t _height;
	uint16_t _bgColor;
	uint16_t _txtColor;
	uint8_t _type;
	bool _autoCreated;
public:
    UIobject(uint16_t posx, uint16_t posy, uint16_t width, uint16_t height);
    virtual void printScreen();
	virtual void disable();
	virtual void enable();
	virtual bool isEnabled();
	void setAutoCreated();
	bool isAutoCreated();
	uint8_t getType();
};

class UIbutton : public UIobject
{
  private:
    String _caption;
	bool _enabled;
public:
  	Button _touchzone;
    UIbutton(uint16_t posx, uint16_t posy, uint16_t width, uint16_t height);
	void setCaption(String str);
	void printScreen();
    void drawObject();
	void invertColor();
	void disable();
	void enable();
	bool isEnabled();
	bool wasPressed();
};

class UIheader
{
private:
	uint16_t _color;
	uint16_t _bgColor;
	uint16_t _width;
	uint16_t _height;
	String _left;
	String _center;
	String _right;
	
public:
	UIheader(uint16_t width, uint16_t height);
	void drawObject();
	void setLeft(String S);
	void setCenter(String S);
	void setRight(String S);
	void drawCenter();
	void setColors(uint16_t color, uint16_t bgColor);
	
};

class UIpage
{
  private:
    bool _isFull;
    uint8_t _objectCount;
    uint16_t _width; //can be replaced by _iwidth
    uint16_t _height; // can be replaced by _iheight
	uint16_t _bgColor;

    UIobject **_pageObjects;
    String _title;

  public:
    UIpage(uint16_t width, uint16_t height);
    void setTitle(String title);
    String getTitle();
    void printScreen();
    void printObjects();
    void addObject(UIobject *object);
	void disableButtons();
	void enableButtons();
	uint8_t getObjectCount();
    UIobject *getObject(uint16_t idx);
};

class m5core2UI
{
  private:
    uint8_t _pageCount;
    uint8_t _currentPage;
    uint8_t _nextPage;
    bool _isFull;
    UIpage **_pages;
	UIheader _header;

  public:
    m5core2UI(uint16_t width, uint16_t height);
    void addPage(UIpage *page);
    void removePage(uint8_t page);
    void nextPage();
    void previousPage();
	void setHeaderColors(uint16_t color, uint16_t bgColor); // set Header color
	void drawHeader(); // draw UI header
	void setHeaderLeft(String S);
	void setHeaderCenter(String S);
	void setHeaderRight(String S);
    UIpage *getPage(uint8_t idx); // get page at given index
	UIpage *getCurrentPage(); // pointer to page
	uint8_t getCurrentPageIndex(); // current page index
    void showUI();
};

#endif
