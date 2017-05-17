#ifndef FIELD_H

enum FieldFlag
{
	TARGET = 1,
	THRONE = 2,
	KING = 4,
	WHITE = 8,
	BLACK = 16,
	BLOCKING = 32
};

class Field
{
public:
	uint8_t flags = 0;
	void setFlags(uint8_t flags);
	void removeFlags(uint8_t flags);
	bool hasFlags(uint8_t flags);
};

inline void Field::setFlags(uint8_t flags)
{
	this->flags = (this->flags | flags);
}

inline void Field::removeFlags(uint8_t flags)
{
	this->flags = (this->flags & ~flags);
}

inline bool Field::hasFlags(uint8_t flags)
{
	bool result = this->flags & flags;

	return result;
}

#define FIELD_H
#endif // !FIELD_H



