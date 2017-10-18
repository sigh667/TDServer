/*------------- netStream.h
* Copyright (C): 2011 Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.01
* Date         : 2011/8/19 12:08:30
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "bufferStream.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/message.h"
/*************************************************************/
class CNetStream	: public CBufferStream ,public ::google::protobuf::io::ZeroCopyOutputStream
{
private:
	bool		m_bHaveNext;
public:
	explicit CNetStream(char* pBuffer, uint32 uSize);
	explicit CNetStream();

protected:
	virtual bool	Next(void** data, int* size);
	virtual void	BackUp(int count);
	virtual bool	Skip(int count);
	virtual ::google::protobuf::int64	ByteCount() const;
public:
	inline	bool	ParseFromStream(::google::protobuf::Message& msg)
	{
		::google::protobuf::io::CodedInputStream in( (const ::google::protobuf::uint8*)getSpareBuffer(),getSpareSize());
		return msg.ParseFromCodedStream(&in);
	}

	inline	bool	SerializeToStream(const ::google::protobuf::Message& msg)
	{
		m_bHaveNext = true;
		::google::protobuf::io::CodedOutputStream out(this);
		if ( !msg.SerializeToCodedStream(&out) )
			return false;

		return skipPosition( out.ByteCount() );
	}
};
    