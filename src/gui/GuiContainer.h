#ifndef _GUICONTAINER_H
#define _GUICONTAINER_H
/*
 * Parent of all widgets that contain other widgets.
 */

#include "GuiWidget.h"
#include <list>

namespace Gui {
	class Container: public Widget {
	public:
		Container();
		virtual ~Container();
		bool OnMouseDown(GuiExtra::MouseButtonEvent *e);
		bool OnMouseUp(GuiExtra::MouseButtonEvent *e);
		bool OnMouseMotion(GuiExtra::MouseMotionEvent *e);
		void RemoveAllChildren();
		void DeleteAllChildren();
		void GetChildPosition(const Widget *child, float outPos[2]) const;
		int GetNumChildren() { return m_children.size(); }
		virtual void Draw();
		void ShowChildren();
		void HideChildren();
		virtual void Show();
		virtual void ShowAll();
		virtual void HideAll();
		virtual void OnChildResizeRequest(Widget *) = 0;
		void SetBgColor(const float rgb[3]);
		void SetBgColor(float r, float g, float b, float a);
		void SetTransparency(bool a) { m_transparent = a; }
		virtual void UpdateAllChildSizes() = 0;
		void RemoveChild(Widget *w);
		// only fired if child widgets do not eat event
		sigc::signal<void, GuiExtra::MouseButtonEvent*> onMouseButtonEvent;
	private:
		void _OnMouseLeave();
		void _OnSetSize();
		bool HandleMouseEvent(GuiExtra::MouseButtonEvent *e);
		float m_bgcol[4];
		bool m_transparent;
	protected:
		void PrependChild(Widget *w, float x, float y);
		void AppendChild(Widget *w, float x, float y);
		void MoveChild(Widget *w, float x, float y);

		struct widget_pos {
			Widget *w;
			float pos[2];
			Uint32 flags;
		};
		std::list<widget_pos> m_children;
	};
}

#endif /* _GUICONTAINER_H */

