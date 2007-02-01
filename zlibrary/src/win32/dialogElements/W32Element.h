/*
 * Copyright (C) 2007 Nikolay Pultsin <geometer@mawhrin.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef __W32ELEMENT_H__
#define __W32ELEMENT_H__

#include <vector>
#include <string>

#include <windows.h>

#include <shared_ptr.h>

class W32Element {

public:
	static int allocateString(WORD *p, const std::string &text);

public:
	struct Size {
		short Width;
		short Height;

		Size() : Width(0), Height(0) {}
		Size(short w, short h) : Width(w), Height(h) {}
	};

protected:
	W32Element();

public:
	virtual ~W32Element();

	virtual void allocate(WORD *p) const = 0;
	virtual int allocationSize() const = 0;
	virtual void setVisible(bool visible) = 0;
	virtual int controlNumber() const = 0;
	virtual Size minimumSize() const = 0;
	virtual void setPosition(int x, int y, Size size) = 0;
	virtual void setDimensions(Size charDimension) = 0;

private:
	W32Element(const W32Element&);
	const W32Element &operator = (const W32Element&);
};

typedef shared_ptr<W32Element> W32ElementPtr;
typedef std::vector<W32ElementPtr> W32ElementList;

class W32Box : public W32Element {

public:
	W32Box();
	void addElement(W32ElementPtr element);

	void allocate(WORD *p) const;
	int allocationSize() const;
	void setVisible(bool visible);
	int controlNumber() const;

	void setHomogeneous(bool homogeneous);
	void setMargins(int top, int bottom, int left, int right);
	void setSpacing(int spacing);

protected:
	bool homogeneous() const { return myHomogeneous; }
	int topMargin() const { return myTopMargin; }
	int bottomMargin() const { return myBottomMargin; }
	int leftMargin() const { return myLeftMargin; }
	int rightMargin() const { return myRightMargin; }
	int spacing() const { return mySpacing; }
	void setDimensions(Size charDimension);

protected:
	W32ElementList myElements;

private:
	bool myHomogeneous;
	int myTopMargin, myBottomMargin, myLeftMargin, myRightMargin;
	int mySpacing;
};

class W32HBox : public W32Box {

public:
	W32HBox();

	Size minimumSize() const;
	void setPosition(int x, int y, Size size);
};

class W32VBox : public W32Box {

public:
	W32VBox();

	Size minimumSize() const;
	void setPosition(int x, int y, Size size);
};

class W32Control : public W32Element {

protected:
	W32Control(DWORD style, WORD id, const std::string &text);

private:
	void allocate(WORD *p) const;
	int allocationSize() const;
	void setVisible(bool visible);
	int controlNumber() const;
	Size minimumSize() const;
	void setPosition(int x, int y, Size size);

	virtual const std::string &className() const = 0;

private:
	DWORD myStyle;
	int myX, myY;
	WORD myId;

protected:
	Size mySize;
	std::string myText;
};

class W32PushButton : public W32Control {

public:
	W32PushButton(WORD id, const std::string &text);
	void setDimensions(Size charDimension);

	const std::string &className() const;
};

class W32CheckBox : public W32Control {

public:
	W32CheckBox(WORD id, const std::string &text);
	void setDimensions(Size charDimension);

	const std::string &className() const;
};

class W32LineEditor : public W32Control {

public:
	W32LineEditor(WORD id, const std::string &text);
	void setDimensions(Size charDimension);

	const std::string &className() const;
};

class W32Panel {

public:
	W32Panel(HWND mainWindow, const std::string &caption);
	~W32Panel();
	DLGTEMPLATE *dialogTemplate();
	void setElement(W32ElementPtr element);
	W32Element::Size charDimension() const;

private:
	W32Element::Size myCharDimension;
	W32Element::Size mySize;
	std::string myCaption;

	W32ElementPtr myElement;

	mutable WORD *myAddress;
};

#endif /* __W32ELEMENT_H__ */
