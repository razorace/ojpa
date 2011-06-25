#ifndef Rectangle_h
#define Rectangle_h

class Rectangle {
public:
	float x, y, w, h;

	Rectangle() {}
	Rectangle(float locX, float locY, float width, float height) {
		x = locX;
		y = locY;
		w = width;
		h = height;
	}

	void SetPosition(float locX, float locY) {
		x = locX;
		y = locY;
	}

	void SetSize(float width, float height) {
		w = width;
		h = height;
	}

	void Set(Rectangle rect) {
		x = rect.x;
		y = rect.y;
		w = rect.w;
		h = rect.h;
	}

	void Set(const Rectangle &rect) {
		x = rect.x;
		y = rect.y;
		w = rect.w;
		h = rect.h;
	}
};

#endif