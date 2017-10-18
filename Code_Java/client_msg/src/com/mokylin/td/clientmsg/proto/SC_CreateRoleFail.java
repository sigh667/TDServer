
// Generated by the SocketProtoGenerationTool.  DO NOT EDIT!
package com.mokylin.td.clientmsg.proto;

import io.netty.buffer.ByteBuf;
import java.util.List;
import java.io.UnsupportedEncodingException;
import javax.transaction.NotSupportedException;
import com.mokylin.bleach.core.collection.BitArray;
import com.mokylin.td.clientmsg.enumeration.*;
import com.mokylin.td.clientmsg.commondata.*;
import com.mokylin.td.clientmsg.core.ICommunicationDataBase;
import com.mokylin.td.clientmsg.core.SerializationHelper;
import com.mokylin.td.clientmsg.ProtoSerializationDefine;


//服务器通知，角色创建失败
public class SC_CreateRoleFail implements ICommunicationDataBase{	
    BitArray __mask__ = new BitArray(1);

	// 失败原因
	private EnumCreateRoleFailReason _failReason;
		
	public SC_CreateRoleFail(){
	}
	
	public int getSerializationID(){
		return ProtoSerializationDefine.SC_CREATEROLEFAIL;
	}


	public EnumCreateRoleFailReason getfailReason() {
		return this._failReason;
	}
	public void setfailReason(EnumCreateRoleFailReason __failReason) {
      SerializationHelper.writeMask(__mask__, 0x1);
		this._failReason = __failReason;
	}
		
	public ByteBuf toBytes(ByteBuf __targetBytes) throws UnsupportedEncodingException, NotSupportedException {
        SerializationHelper.writeBitArray(__targetBytes, this.__mask__);

		if(SerializationHelper.readMask(this.__mask__, 0x1) == true)
            SerializationHelper.writeU29Int(__targetBytes, this._failReason.getValue());
		
		return __targetBytes;
	}

	public void fromBytes(ByteBuf __serializationBytes) throws UnsupportedEncodingException, InstantiationException, IllegalAccessException, NotSupportedException {
        this.__mask__ = SerializationHelper.readBitArray(__serializationBytes);

		if(SerializationHelper.readMask(this.__mask__, 0x1) == true)
            this._failReason = EnumCreateRoleFailReason.valueOf(SerializationHelper.readU29Int(__serializationBytes));
		
	}

}

		