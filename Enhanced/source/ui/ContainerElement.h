#ifndef ContainerElement_h
#define ContainerElement_h

#include "UIElement.h"
#include "itemDef_t.h"
#include <list>

void Item_Paint(itemDef_t *item);

class ContainerElement : public UIElement {
	protected:
		std::list<UIElement*> children;
		std::list<itemDef_t*> oldChildren;

		void DrawElement(UIElement *element) {
			//...

			ContainerElement *container = dynamic_cast<ContainerElement*>(element);

			if(container != nullptr) {
				std::list<itemDef_t*> childOldChildren = container->GetOldChildren();

				for(std::list<itemDef_t*>::iterator i = childOldChildren.begin(); i != childOldChildren.end(); ++i) {
					itemDef_t *child = (*i);

					Item_Paint(child);
				}

				std::list<UIElement*> childChildren = container->GetChildren();

				for(std::list<UIElement*>::iterator i = childChildren.begin(); i != childChildren.end(); ++i) {
					DrawElement(*i);
				}
			}
		}

	public:
		std::list<itemDef_t*> GetOldChildren() {
			return oldChildren;
		}

		std::list<UIElement*> GetChildren() {
			return children;
		}

		ContainerElement() {}
		~ContainerElement() {}

		void AddOldElement(itemDef_t* item) {
			oldChildren.push_back(item);
		}

		void AddElement(UIElement* element) {
			children.push_back(element);
		}

		void Draw() {
			for(std::list<itemDef_t*>::iterator i = oldChildren.begin(); i != oldChildren.end(); ++i) {
				itemDef_t *child = (*i);

				Item_Paint(child);
			}

			for(std::list<UIElement*>::iterator i = children.begin(); i != children.end(); ++i) {
				DrawElement(*i);
			}
		}
};

#endif