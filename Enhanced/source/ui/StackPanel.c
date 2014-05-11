#include "StackPanel.h"

extern void Item_UpdatePosition(itemDef_t *item);
extern void Item_SetScreenCoords(itemDef_t *item, float x, float y);
extern void Item_Paint(itemDef_t *item);

void StackPanel_Init(StackPanel *panel) {
	memset(panel, 0, sizeof(StackPanel));
	panel->orientation = kOrientationVertical;
	panel->childrenCount = 0;
	panel->oldChildrenCount = 0;
}

void StackPanel_Arrange(StackPanel *panel) {
	int i;
	int x = panel->rectangle.x;
	int y = panel->rectangle.y;
	
	for(i = 0; i < panel->oldChildrenCount; i++) {
		itemDef_t *child = panel->oldChildren[i];

		int margin = 0;
		if(child->window.border != 0) {
			margin = child->window.borderSize;
		}

		child->window.rectClient.x = x;
		child->window.rectClient.y = y;

		//child->window.rectClient.SetPosition(x, y);
		//child->textRect.SetPosition(x + ChildMargin, y + ChildMargin);
		//child->window.rect.SetPosition(x + ChildMargin, y + ChildMargin);
		Item_UpdatePosition(child);

		if(panel->orientation == kOrientationVertical) {
			y += child->window.rectClient.h;
		}
		else {
			x += child->window.rectClient.w;
		}
	}

	for(i = 0; i < panel->childrenCount; i++) {
		StackPanel *child = panel->children[i];
		child->rectangle.x = x;
		child->rectangle.y = y;

		//child->GetRectangle()->SetPosition(x, y);

		if(panel->orientation == kOrientationVertical) {
			y += child->rectangle.h;
		}
		else {
			x += child->rectangle.w;
		}

		StackPanel_Arrange(child);
	}
}

void StackPanel_Draw(StackPanel *panel) {
	int i;

	for(i = 0; i < panel->oldChildrenCount; i++) {
		itemDef_t *child = panel->oldChildren[i];

		Item_Paint(child);
	}

	for(i = 0; i < panel->childrenCount; i++) {
		StackPanel_DrawSubElement(panel->children[i]);
	}
}


void StackPanel_DrawSubElement(StackPanel *element) {
	if(element != NULL) {
		int i;

		for(i = 0; i < element->oldChildrenCount; i++) {
			itemDef_t *child = element->oldChildren[i];

			Item_Paint(child);
		}

		for(i = 0; i < element->childrenCount; i++) {
			StackPanel_DrawSubElement(element->children[i]);
		}
	}
}