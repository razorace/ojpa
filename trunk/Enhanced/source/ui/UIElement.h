#ifndef UIElement_h
#define UIElement_h

#include "Rectangle.h"

class UIElement {
	protected:
		Rectangle	rectangle;

	public:

		UIElement() {}
		~UIElement() {}

		Rectangle *GetRectangle() {
			return &rectangle;
		}

		void SetRectangle(Rectangle rect) {
			rectangle = rect;
		}

		virtual void Arrange() {}
		virtual void Draw() {}
};

#endif