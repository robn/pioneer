// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef SAVELOAD_H
#define SAVELOAD_H

#include <cstdio>
#include "SDL_stdinc.h"
#include "json/json.h"

struct SavedGameCorruptException {};
struct SavedGameWrongVersionException {};
struct CouldNotOpenFileException {};
struct CouldNotWriteToFileException {};

class Game;

namespace SaveLoad {

// helper methods for safely storing/retrieving doubles
inline char *DoubleToHexFloat(double v) {
	static char buf[32];
	snprintf(buf, sizeof(buf), "%a", v);
	return buf;
}
inline double HexFloatToDouble(const char *buf) {
	double v;
	sscanf(buf, "%lf", &v);
	return v;
}
inline double HexFloatToDouble(const std::string &str) {
	return HexFloatToDouble(str.c_str());
}

// SaveLoad object. holds the complete state of one game object the convention
// for game objects that expect to be saved is that they will have a
// constructor that accepts a SaveLoad::Object as its only argument and a Save
// method that returns one. this convention is not currently enforced, but is
// highly recommended
//
// most of the time a Save method will use the default Object constructor and
// the Set methods to store values in a basic dictionary. to store other
// objects, call their Save methods and include the result
//
// certain special-case objects (usually those masquerading as primitive types
// eg vector3) may want to use a non-dictionary style to store themselves. in
// that case, they can create a raw Json::Value object and wrap an Object
// around it, and later in construction can use the GetJson method to get it
// back. in this case, the behaviour of the Set/Get methods are undefined. if
// you're not sure if you need this variant, you probably don't.
//
// if the object you're trying to create is part of a class hierarchy that has
// parent classes that need to be deserialized as well, the convention is to
// call the parent class' Save method, and then add your own data to it. (this
// of course means that there could be key conflicts, but that likely means you
// have a naming or other design problem somewhere in your hierarchy). during
// construction, oass the object up to the parent constructor in an
// initiialiser.
//
// the Body classes demonstrate the "right" way to do all this

class Object {
public:
	// standard constructor
	Object() : m_json(Json::objectValue) {}

	// special constructor for caller-provided JSON structures
	Object(const Json::Value &json) : m_json(json) {}

	// to get the underlying JSON struture for special-case stuff
	const Json::Value &GetJson() const { return m_json; }

	// convert entire object to string
	std::string ToString() const;

	// dump to stdout
	void Dump() const;

	// setters for primitive types. only valid if the standard constructor was used
	void Set(const std::string &k, bool v)               { m_json[k] = v; }
	void Set(const std::string &k, Uint32 v)             { m_json[k] = Json::UInt(v); }
	void Set(const std::string &k, Uint64 v)             { m_json[k] = Json::UInt64(v); }
	void Set(const std::string &k, Sint32 v)             { m_json[k] = Json::Int(v); }
	void Set(const std::string &k, Sint64 v)             { m_json[k] = Json::Int64(v); }
	void Set(const std::string &k, double v)             { m_json[k] = DoubleToHexFloat(v); }
	void Set(const std::string &k, float v)              { m_json[k] = DoubleToHexFloat(double(v)); }
	void Set(const std::string &k, const std::string &v) { m_json[k] = v; }
	void Set(const std::string &k, const char *v)        { Set(k, std::string(v)); }
	void Set(const std::string &k, const Object &v)      { m_json[k] = v.GetJson(); }
	void Set(const std::string &k, const Json::Value &v) { m_json[k] = v; }

	// getters for primitive types. third argument is an optional default value to use if the key was not found. only valid if the standard constructor was used
	void Get(const std::string &k, bool &v, bool d = false) const                   { v = m_json.get(k, d).asBool(); }
	void Get(const std::string &k, Uint32 &v, Uint32 d = 0) const                   { v = m_json.get(k, Json::UInt(d)).asUInt(); }
	void Get(const std::string &k, Uint64 &v, Uint64 d = 0) const                   { v = m_json.get(k, Json::UInt64(d)).asUInt64(); }
	void Get(const std::string &k, Sint32 &v, Sint32 d = 0) const                   { v = m_json.get(k, Json::Int(d)).asInt(); }
	void Get(const std::string &k, Sint64 &v, Sint64 d = 0) const                   { v = m_json.get(k, Json::Int64(d)).asInt64(); }
	void Get(const std::string &k, double &v, double d = 0.0) const                 { v = HexFloatToDouble(m_json.get(k, DoubleToHexFloat(d)).asString()); }
	void Get(const std::string &k, float &v, float d = 0.0f) const                  { v = float(HexFloatToDouble(m_json.get(k, DoubleToHexFloat(double(d))).asString())); }
	void Get(const std::string &k, std::string &v, const std::string &d = "") const { v = m_json.get(k, d).asString(); }
	void Get(const std::string &k, Object &v, const Object &d = Object()) const     { v = Object(m_json.get(k, d.GetJson())); }
	void Get(const std::string &k, Json::Value &v,
        const Json::Value &d = Json::Value(Json::nullValue)) const                  { v = m_json.get(k, d); }

	// get Object by key, for use in constructor initialisers
	Object GetObject(const std::string &k) const { return Object(m_json.get(k, Object().GetJson())); }

private:
	Json::Value m_json;
};


class SaveContext;
class LoadContext;

class RefObject {
protected:
	RefObject() {}
	RefObject(const SaveLoad::Object &so, LoadContext *lc);
	virtual SaveLoad::Object Save(SaveContext *sc) const;
};

// class to track cross-object references
// during save, put objects in, get numeric identifiers out
// during load, record objects and their ids when you see them. later when you
// find an id, put it in and get the corresponding object out
class RefTracker {
public:
	RefTracker() : m_nextId(1) {}

	// return a unique identifier for this object. if it hasn't been seen
	// before, returns a fresh id, otherwise returns the same one as last time
	// passing a null will always return 0
	Uint32 GetRefId(const RefObject *o);

	// record an object with a specific id. pretty much like GetId, except you
	// set the id. if the object or id is already used, does nothing
	void AddRefId(const RefObject *o, Uint32 refId);

	// get the object for the given id. returns null if its not found
	RefObject *GetRefObject(Uint32 refId);

private:
	Uint32 m_nextId;
	std::map<const RefObject*,Uint32> m_idForObject;
	std::map<Uint32,const RefObject*> m_objectForId;
};


class SaveContext {
public:
	SaveContext(Game *game) : m_game(game) {}

	// Write throws exceptions on failure
	void Write(const std::string &filename);

	Uint32 GetRefId(const RefObject *o) { return m_rt.GetRefId(o); }

private:
	Game *m_game;
	RefTracker m_rt;
};


class LoadContext {
public:
	LoadContext() {}

	Game *Read(const std::string &filename);

	template <typename T> void Fixup(const SaveLoad::Object &so, const std::string &key, T **loc) {
		Uint32 refId;
		so.Get(key, refId);
		Fixup(refId, loc);
	}

	template <typename T> void Fixup(Uint32 refId, T **loc) {
		m_fixups[refId] = reinterpret_cast<void**>(loc);
#ifdef DEBUG
		*loc = reinterpret_cast<T*>(0x5a5a5a5a);
#endif
	}

	void AddRefId(const RefObject *o, Uint32 refId) { m_rt.AddRefId(o, refId); }

private:
	RefTracker m_rt;

	void ApplyFixups();
	std::map<Uint32,void**> m_fixups;
};


#if 0
XXX DESERIALIZER
	class Reader {
	public:
		Reader();
		Reader(const std::string &data);
		Reader(FILE *fptr);
		bool AtEnd();
		void Seek(int pos);
		Uint8 Byte();
		bool Bool();
		Uint16 Int16();
		Uint32 Int32();
		Uint64 Int64();
		float Float ();
		double Double ();
		std::string String();
		vector3d Vector3d();
		Quaternionf RdQuaternionf();
		Reader RdSection(const std::string &section_label_expected) {
			if (section_label_expected != String()) {
				throw SavedGameCorruptException();
			}
			Reader section = Reader(String());
			section.SetStreamVersion(StreamVersion());
			return section;
		}
		/** Best not to use these except in templates */
		void Auto(Sint32 *x) { *x = Int32(); }
		void Auto(Sint64 *x) { *x = Int64(); }
		void Auto(float *x) { *x = Float(); }
		void Auto(double *x) { *x = Double(); }
		int StreamVersion() const { return m_streamVersion; }
		void SetStreamVersion(int x) { m_streamVersion = x; }
	private:
		std::string m_data;
		size_t m_pos;
		int m_streamVersion;
	};
#endif


}

#endif /* _SERIALIZE_H */
