// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: WALProtocol.proto

#ifndef PROTOBUF_WALProtocol_2eproto__INCLUDED
#define PROTOBUF_WALProtocol_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_WALProtocol_2eproto();
void protobuf_AssignDesc_WALProtocol_2eproto();
void protobuf_ShutdownFile_WALProtocol_2eproto();


enum _emPW2L_Protocol {
  P_W2L_Protocol_Operator = 0,
  P_W2L_Protocol_Max = 1
};
bool _emPW2L_Protocol_IsValid(int value);
const _emPW2L_Protocol _emPW2L_Protocol_MIN = P_W2L_Protocol_Operator;
const _emPW2L_Protocol _emPW2L_Protocol_MAX = P_W2L_Protocol_Max;
const int _emPW2L_Protocol_ARRAYSIZE = _emPW2L_Protocol_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emPW2L_Protocol_descriptor();
inline const ::std::string& _emPW2L_Protocol_Name(_emPW2L_Protocol value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emPW2L_Protocol_descriptor(), value);
}
inline bool _emPW2L_Protocol_Parse(
    const ::std::string& name, _emPW2L_Protocol* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emPW2L_Protocol>(
    _emPW2L_Protocol_descriptor(), name, value);
}
enum _emW2L_Protocol_Operator {
  W2L_Operator_AccountLoginAsk = 0,
  W2L_Operator_AccountLogout = 1,
  W2L_Operator_AccountUnlogin = 2,
  W2L_Operator_GameEnter = 3,
  W2L_Operator_GameLeave = 4,
  W2L_Operator_Max = 5
};
bool _emW2L_Protocol_Operator_IsValid(int value);
const _emW2L_Protocol_Operator _emW2L_Protocol_Operator_MIN = W2L_Operator_AccountLoginAsk;
const _emW2L_Protocol_Operator _emW2L_Protocol_Operator_MAX = W2L_Operator_Max;
const int _emW2L_Protocol_Operator_ARRAYSIZE = _emW2L_Protocol_Operator_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emW2L_Protocol_Operator_descriptor();
inline const ::std::string& _emW2L_Protocol_Operator_Name(_emW2L_Protocol_Operator value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emW2L_Protocol_Operator_descriptor(), value);
}
inline bool _emW2L_Protocol_Operator_Parse(
    const ::std::string& name, _emW2L_Protocol_Operator* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emW2L_Protocol_Operator>(
    _emW2L_Protocol_Operator_descriptor(), name, value);
}
enum _emPL2W_Protocol {
  P_L2W_Protocol_Operator = 0,
  P_L2W_Protocol_Max = 1
};
bool _emPL2W_Protocol_IsValid(int value);
const _emPL2W_Protocol _emPL2W_Protocol_MIN = P_L2W_Protocol_Operator;
const _emPL2W_Protocol _emPL2W_Protocol_MAX = P_L2W_Protocol_Max;
const int _emPL2W_Protocol_ARRAYSIZE = _emPL2W_Protocol_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emPL2W_Protocol_descriptor();
inline const ::std::string& _emPL2W_Protocol_Name(_emPL2W_Protocol value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emPL2W_Protocol_descriptor(), value);
}
inline bool _emPL2W_Protocol_Parse(
    const ::std::string& name, _emPL2W_Protocol* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emPL2W_Protocol>(
    _emPL2W_Protocol_descriptor(), name, value);
}
enum _emL2W_Operator_Protocol {
  L2W_Operator_AccountLoginAck = 0,
  L2W_Operator_AccountLogout = 1,
  L2W_Operator_AccountUnlogin = 2,
  L2W_Operator_GameEnter = 3,
  L2W_Operator_GameLeave = 4,
  L2W_Operator_Max = 5
};
bool _emL2W_Operator_Protocol_IsValid(int value);
const _emL2W_Operator_Protocol _emL2W_Operator_Protocol_MIN = L2W_Operator_AccountLoginAck;
const _emL2W_Operator_Protocol _emL2W_Operator_Protocol_MAX = L2W_Operator_Max;
const int _emL2W_Operator_Protocol_ARRAYSIZE = _emL2W_Operator_Protocol_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emL2W_Operator_Protocol_descriptor();
inline const ::std::string& _emL2W_Operator_Protocol_Name(_emL2W_Operator_Protocol value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emL2W_Operator_Protocol_descriptor(), value);
}
inline bool _emL2W_Operator_Protocol_Parse(
    const ::std::string& name, _emL2W_Operator_Protocol* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emL2W_Operator_Protocol>(
    _emL2W_Operator_Protocol_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< _emPW2L_Protocol>() {
  return _emPW2L_Protocol_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< _emW2L_Protocol_Operator>() {
  return _emW2L_Protocol_Operator_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< _emPL2W_Protocol>() {
  return _emPL2W_Protocol_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< _emL2W_Operator_Protocol>() {
  return _emL2W_Operator_Protocol_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_WALProtocol_2eproto__INCLUDED
