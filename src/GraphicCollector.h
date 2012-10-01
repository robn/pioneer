#ifndef _GRAPHICCOLLECTOR_H
#define _GRAPHICCOLLECTOR_H

class Graphic;

class GraphicCollector {
public:
	typedef std::vector<Graphic*> GraphicList;

	void AddOpaque(Graphic *g) { m_opaque.push_back(g); }
	void AddTransparent(Graphic *g) { m_transparent.push_back(g); }

	GraphicList::const_iterator BeginOpaque() const { return m_opaque.begin(); }
	GraphicList::const_iterator EndOpaque() const { return m_opaque.end(); }

	GraphicList::const_iterator BeginTransparent() const { return m_transparent.begin(); }
	GraphicList::const_iterator EndTransparent() const { return m_transparent.end(); }

private:
	GraphicList m_opaque;
	GraphicList m_transparent;
};

#endif
