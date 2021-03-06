// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: WADIPProtocol.proto

#ifndef PROTOBUF_WADIPProtocol_2eproto__INCLUDED
#define PROTOBUF_WADIPProtocol_2eproto__INCLUDED

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
void  protobuf_AddDesc_WADIPProtocol_2eproto();
void protobuf_AssignDesc_WADIPProtocol_2eproto();
void protobuf_ShutdownFile_WADIPProtocol_2eproto();


enum _emPWADIP_Protocol {
  P_WADIP_Http = 1
};
bool _emPWADIP_Protocol_IsValid(int value);
const _emPWADIP_Protocol _emPWADIP_Protocol_MIN = P_WADIP_Http;
const _emPWADIP_Protocol _emPWADIP_Protocol_MAX = P_WADIP_Http;
const int _emPWADIP_Protocol_ARRAYSIZE = _emPWADIP_Protocol_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emPWADIP_Protocol_descriptor();
inline const ::std::string& _emPWADIP_Protocol_Name(_emPWADIP_Protocol value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emPWADIP_Protocol_descriptor(), value);
}
inline bool _emPWADIP_Protocol_Parse(
    const ::std::string& name, _emPWADIP_Protocol* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emPWADIP_Protocol>(
    _emPWADIP_Protocol_descriptor(), name, value);
}
enum _emWADIP_Http_Protocol {
  WADIP_Http_SendMail = 1,
  WADIP_Http_CommonResult = 2
};
bool _emWADIP_Http_Protocol_IsValid(int value);
const _emWADIP_Http_Protocol _emWADIP_Http_Protocol_MIN = WADIP_Http_SendMail;
const _emWADIP_Http_Protocol _emWADIP_Http_Protocol_MAX = WADIP_Http_CommonResult;
const int _emWADIP_Http_Protocol_ARRAYSIZE = _emWADIP_Http_Protocol_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emWADIP_Http_Protocol_descriptor();
inline const ::std::string& _emWADIP_Http_Protocol_Name(_emWADIP_Http_Protocol value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emWADIP_Http_Protocol_descriptor(), value);
}
inline bool _emWADIP_Http_Protocol_Parse(
    const ::std::string& name, _emWADIP_Http_Protocol* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emWADIP_Http_Protocol>(
    _emWADIP_Http_Protocol_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< _emPWADIP_Protocol>() {
  return _emPWADIP_Protocol_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< _emWADIP_Http_Protocol>() {
  return _emWADIP_Http_Protocol_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_WADIPProtocol_2eproto__INCLUDED
