#include "StackPanel.h"

void Item_UpdatePosition(itemDef_t *item);
void Item_SetScreenCoords(itemDef_t *item, float x, float y);

StackPanel::StackPanel(void) :
	orientation(kOrientationVertical) {

}

void StackPanel::Arrange() {
	const int ChildMargin = 5;
	int x = rectangle.x;
	int y = rectangle.y;

	for(std::list<itemDef_t*>::iterator i = oldChildren.begin(); i != oldChildren.end(); ++i) {
		itemDef_t *child = (*i);

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

		if(orientation == kOrientationVertical) {
			y += child->window.rectClient.h;
		}
		else {
			x += child->window.rectClient.w;
		}
	}

	for(std::list<UIElement*>::iterator i = children.begin(); i != children.end(); ++i) {
		UIElement *child = *i;
		Rectangle *rect = child->GetRectangle();
		rect->x = x;
		rect->y = y;

		//child->GetRectangle()->SetPosition(x, y);

		if(orientation == kOrientationVertical) {
			y += rect->h;
		}
		else {
			x += rect->w;
		}

		child->Arrange();
	}
}