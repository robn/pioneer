// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef _PERSISTSYSTEMDATA_H
#define _PERSISTSYSTEMDATA_H

#include "SaveLoad.h"
#include "galaxy/SystemPath.h"
#include <map>

template <typename T>
class PersistSystemData {
public:
	void Clear() {
		m_dict.clear();
	}
	T Get(const SystemPath &path, T defaultValIfNotExists) const {
		typename std::map<SystemPath, T>::const_iterator i = m_dict.find(path.SystemOnly());
		if (i == m_dict.end()) return defaultValIfNotExists;
		else return (*i).second;
	}
	void Set(const SystemPath &path, T val) {
		m_dict[path.SystemOnly()] = val;
	}
	SaveLoad::Object Save() const {
		Json::Value dict(Json::arrayValue);
		for (typename std::map<SystemPath, T>::const_iterator i = m_dict.begin(); i != m_dict.end(); ++i) {
			SaveLoad::Object entry;
			entry.Set("path", (*i).first.Save());
			entry.Set("value", (*i).second);
			dict.append(entry.GetJson());
		}
		return dict;
	}
/* XXX DESERIALIZER
	static void Unserialize(SaveLoad::Reader &rd, PersistSystemData<T> *pd) {
		int num = rd.Int32();
		while (num-- > 0) {
			SystemPath path = SystemPath::Unserialize(rd);
			T val;
			rd.Auto(&val);
			pd->m_dict[path] = val;
		}
	}
*/
private:
	std::map<SystemPath, T> m_dict;
};

#endif /* _PERSISTSYSTEMDATA_H */
