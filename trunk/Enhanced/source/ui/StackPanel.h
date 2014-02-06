#ifndef StackPanel_h
#define StackPanel_h

#include "itemDef_t.h"
#include <list>

enum Orientation {
	kOrientationVertical,
	kOrientationHorizontal
};

class StackPanel {
	public:
		StackPanel(void);

		Orientation orientation;
		Rectangle	rectangle;

		StackPanel *children[256];
		int childrenCount;

		itemDef_t *oldChildren[256];
		int oldChildrenCount;
};

typedef struct {
	Orientation orientation;
	std::list<StackPanel*> children;
	std::list<itemDef_t*> oldChildren;
	Rectangle	rectangle;
} StackPanelS;

void StackPanel_Draw(StackPanel *panel);
void StackPanel_DraSubElement(StackPanel *element);
void StackPanel_Arrange(StackPanel *panel);

#endif