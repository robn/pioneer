// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "SaveLoad.h"
#include "FileSystem.h"
#include "Pi.h"
#include "Game.h"

namespace SaveLoad {

std::string Object::ToString() const
{
	return Json::FastWriter().write(m_json);
}

void Object::Dump() const
{
	printf("%s\n", ToString().c_str());
}


RefObject::RefObject(const SaveLoad::Object &so, LoadContext *lc)
{
	Uint32 refId;
	so.Get("refId", refId);
	assert(refId);
	lc->AddRefId(this, refId);
}

SaveLoad::Object RefObject::Save(SaveContext *sc) const
{
	Uint32 refId = sc->GetRefId(this);
	SaveLoad::Object so;
	so.Set("refId", refId);
	return so;
}


Uint32 RefTracker::GetRefId(const RefObject *o)
{
	if (!o) return 0;
	std::map<const RefObject*,Uint32>::iterator i = m_idForObject.find(o);
	if (i != m_idForObject.end())
		return (*i).second;
	Uint32 refId = m_nextId;
	AddRefId(o, refId);
	return refId;
}

void RefTracker::AddRefId(const RefObject *o, Uint32 refId)
{
	m_idForObject.insert(std::make_pair(o, refId));
	m_objectForId.insert(std::make_pair(refId, o));
	if (m_nextId <= refId) m_nextId = refId+1;
}

RefObject *RefTracker::GetRefObject(Uint32 refId)
{
	if (!refId) return 0;
	std::map<Uint32,const RefObject*>::iterator i = m_objectForId.find(refId);
	if (i != m_objectForId.end())
        // const cast because the caller almost certainly will want to use it
		return const_cast<RefObject*>((*i).second);
	assert(!refId); // non-zero refId should always have a valid object
	return 0;
}


void SaveContext::Write(const std::string &filename)
{
	if (!FileSystem::userFiles.MakeDirectory(Pi::SAVE_DIR_NAME))
		throw CouldNotOpenFileException();

	SaveLoad::Object so = m_game->Save(this);

	const std::string data = so.ToString();

	FILE *f = FileSystem::userFiles.OpenWriteStream(FileSystem::JoinPathBelow(Pi::SAVE_DIR_NAME, filename));
	if (!f) throw CouldNotOpenFileException();

	size_t nwritten = fwrite(data.data(), data.length(), 1, f);
	fclose(f);

	if (nwritten != 1) throw CouldNotWriteToFileException();
}


Game *LoadContext::Read(const std::string &filename)
{
	RefCountedPtr<FileSystem::FileData> fd = FileSystem::userFiles.ReadFile(FileSystem::JoinPathBelow(Pi::SAVE_DIR_NAME, filename));

	Json::Value data;
	if (!Json::Reader().parse(fd->GetData(), fd->GetData()+fd->GetSize(), data)) throw SavedGameCorruptException();

	Game *game = new Game(SaveLoad::Object(data), this);
	ApplyFixups();

	return game;
}

void LoadContext::ApplyFixups()
{
	for (std::map<Uint32,void**>::iterator i = m_fixups.begin(); i != m_fixups.end(); ++i)
		*((*i).second) = m_rt.GetRefObject((*i).first);
}



#if 0
Reader::Reader(): m_data(""), m_pos(0) {
}
Reader::Reader(const std::string &data):
	m_data(data),
	m_pos(0) {

}
Reader::Reader(FILE *fptr): m_pos(0) {
	m_data = "";
	while (!feof(fptr)) m_data.push_back(fgetc(fptr));
	printf(SIZET_FMT " characters in savefile\n", m_data.size());
}
bool Reader::AtEnd() { return m_pos >= m_data.size(); }
void Reader::Seek(int pos) { m_pos = pos; }
Uint8 Reader::Byte() {
#ifdef DEBUG
	assert(m_pos < m_data.size());
#endif /* DEBUG */
	return Uint8(m_data[m_pos++]);
}
bool Reader::Bool() {
	return Byte() != 0;
}
Uint16 Reader::Int16()
{
	int t1, t2;
	t2 = Byte();
	t1 = Byte();
	return ((t1 << 8) | t2);
}
Uint32 Reader::Int32(void)
{
	int t1, t2, t3, t4;
	t4 = Byte();
	t3 = Byte();
	t2 = Byte();
	t1 = Byte();
	return ((t1 << 24) | (t2 << 16) | (t3 << 8) | t4);
}
Uint64 Reader::Int64(void)
{
	Uint64 t1, t2, t3, t4, t5, t6, t7, t8;
	t8 = Byte();
	t7 = Byte();
	t6 = Byte();
	t5 = Byte();
	t4 = Byte();
	t3 = Byte();
	t2 = Byte();
	t1 = Byte();
	return ((t1<<56) | (t2<<48) | (t3<<40) | (t4<<32) | (t5 << 24) | (t6 << 16) | (t7 << 8) | t8);
}

float Reader::Float ()
{
	unsigned int i;
	union {
		unsigned char c[sizeof (float)];
		float f;
	} p;

	for (i=0; i<sizeof (float); i++) {
		p.c[i] = Byte ();
	}
	return p.f;
}

double Reader::Double ()
{
	unsigned int i;
	union {
		unsigned char c[sizeof (double)];
		double f;
	} p;

	for (i=0; i<sizeof (double); i++) {
		p.c[i] = Byte ();
	}
	return p.f;
}

std::string Reader::String()
{
	int size = Int32();
	if (size == 0) return "";

	std::string buf;
	buf.reserve(size-1);

	for (int i=0; i<size-1; i++) {
		buf.push_back(char(Byte()));
	}
	Byte();// discard null terminator
	return buf;
}

vector3d Reader::Vector3d()
{
	vector3d v;
	v.x = Double();
	v.y = Double();
	v.z = Double();
	return v;
}

Quaternionf Reader::RdQuaternionf()
{
	Quaternionf q;
	q.w = Float();
	q.x = Float();
	q.y = Float();
	q.z = Float();
	return q;
}
#endif

} /* end namespace SaveLoad */
