#ifndef StackPanel_h
#define StackPanel_h

#include "itemDef_t.h"

typedef struct stackPanel_s stackPanel_s;

typedef struct stackPanel_s {
	enum Orientation {kOrientationVertical, kOrientationHorizontal} orientation;
	Rectangle	rectangle;

	stackPanel_s *children[256];
	int childrenCount;

	itemDef_t *oldChildren[256];
	int oldChildrenCount;
} StackPanel;

void StackPanel_Draw(StackPanel *panel);
void StackPanel_DrawSubElement(StackPanel *element);
void StackPanel_Arrange(StackPanel *panel);
void StackPanel_Init(StackPanel *panel);

#endif