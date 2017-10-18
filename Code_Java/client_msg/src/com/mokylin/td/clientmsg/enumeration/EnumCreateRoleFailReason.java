
			
// Generated by the SocketProtoGenerationTool.  DO NOT EDIT!
package com.mokylin.td.clientmsg.enumeration;

//
public enum EnumCreateRoleFailReason
{

			
		// 名字已被占用
		NAME_SAME(0),

		
			
		// 名字包含不合法字符
		NAME_ILLEGAL_CHAR(1),

		
			
		// 名字太长
		NAME_TOO_LONG(2),

		
			
		// 名字太短
		NAME_TOO_SHORT(3),

		
			
		// 该体型尚未开放
		BODY_NOT_OPEN(4),

		
			
		// 该门派尚未开放
		MENPAI_NOT_OPEN(5)

;

	private final int value;
	
	EnumCreateRoleFailReason(int value){
		this.value= value;
	}
	
	public int getValue() {
		return value;
	}

	public static EnumCreateRoleFailReason valueOf(int _value) {
		for (EnumCreateRoleFailReason each : EnumCreateRoleFailReason.values()) {
			if (each.value == _value) {
				return each;
			}
		}
		return null;
	}
}

		