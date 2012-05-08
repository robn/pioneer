#ifndef _TAGGED_H
#define _TAGGED_H

#include <set>
#include <string>

struct lua_State;

class Tagged {
public:
	void AddTag(const std::string &tag) { m_set.insert(tag); }
	void RemoveTag(const std::string &tag) { m_set.erase(tag); }
	bool HasTag(const std::string &tag) const { return m_set.find(tag) != m_set.end(); }

	typedef std::set<std::string>::const_iterator TagIterator;
	const TagIterator TagsBegin() const { return m_set.begin(); }
	const TagIterator TagsEnd() const { return m_set.end(); }

	// adds tags from lua table. takes lua state and stack index of table.
	// returns number of tags added
	unsigned int AddTagsFromLua(lua_State *L, int stackIndex);

	// attribute methods for lua
	static int l_attr_tags(lua_State *L);

private:
	std::set<std::string> m_set;
};

#endif
