// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "Serializer.h"
#include "FileSystem.h"
#include "Pi.h"
#include "Game.h"

namespace Serializer {

void GameSerializer::Write(const std::string &filename)
{
	if (!FileSystem::userFiles.MakeDirectory(Pi::SAVE_DIR_NAME))
		throw CouldNotOpenFileException();

	Serializer::Object so = m_game->Serialize(this);

	const std::string data = Json::FastWriter().write(so.GetJson());

	FILE *f = FileSystem::userFiles.OpenWriteStream(FileSystem::JoinPathBelow(Pi::SAVE_DIR_NAME, filename));
	if (!f) throw CouldNotOpenFileException();

	size_t nwritten = fwrite(data.data(), data.length(), 1, f);
	fclose(f);

	if (nwritten != 1) throw CouldNotWriteToFileException();
}

Serializer::Object GameSerializer::MakeRefObject(const ReferrableObject *o)
{
	Uint32 id = GetRefId(o);
	Serializer::Object so;
	so.Set("refId", id);
	return so;
}

Uint32 GameSerializer::GetRefId(const ReferrableObject *o)
{
	if (!o) return 0;
	std::map<const ReferrableObject*,Uint32>::iterator i = m_objects.find(o);
	if (i != m_objects.end())
		return (*i).second;
	m_objects.insert(i, std::make_pair(o, ++m_nextId));
	return m_nextId;
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

} /* end namespace Serializer */
