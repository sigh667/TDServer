#include "base/cppByteArray.h"

template<>void CPPByteArray::write(char t)
{
	tryWriteByte(t);
};
template<>void CPPByteArray::write(unsigned char t)
{
	tryWriteByte(t);
};

template<>void CPPByteArray::write(short t)
{
	tryWriteShort(t);
};
template<>void CPPByteArray::write(unsigned short t)
{
	tryWriteShort(t);
};
template<>void CPPByteArray::write(U29Int t)
{
	tryWriteU29Int(t);
};
template<>void CPPByteArray::write(int t)
{
	tryWriteInt(t);
};
template<>void CPPByteArray::write(unsigned int t)
{
	tryWriteInt(t);
};

