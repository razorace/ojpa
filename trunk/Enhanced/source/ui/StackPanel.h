#ifndef StackPanel_h
#define StackPanel_h

#include "ContainerElement.h"

enum Orientation {
	kOrientationVertical,
	kOrientationHorizontal
};

class StackPanel : public ContainerElement {
	protected:
		Orientation orientation;

	public:
		StackPanel(void);

		Orientation GetOrientation() {
			return orientation;
		}

		void SetOrientation(Orientation value) {
			orientation = value;
		}

		void Arrange();
};

#endif