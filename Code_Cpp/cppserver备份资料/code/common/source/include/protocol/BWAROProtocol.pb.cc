// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "BWAROProtocol.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::EnumDescriptor* _emPBW2RO_Protocol_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* _emPRO2BW_Protocol_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_BWAROProtocol_2eproto() {
  protobuf_AddDesc_BWAROProtocol_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "BWAROProtocol.proto");
  GOOGLE_CHECK(file != NULL);
  _emPBW2RO_Protocol_descriptor_ = file->enum_type(0);
  _emPRO2BW_Protocol_descriptor_ = file->enum_type(1);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_BWAROProtocol_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_BWAROProtocol_2eproto() {
}

void protobuf_AddDesc_BWAROProtocol_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\023BWAROProtocol.proto*M\n\022_emPBW2RO_Proto"
    "col\022\035\n\031P_BW2RO_Protocol_Register\020\000\022\030\n\024P_"
    "BW2RO_Protocol_Max\020\001*I\n\022_emPRO2BW_Protoc"
    "ol\022\031\n\025P_RO2BW_Protocol_None\020\000\022\030\n\024P_RO2BW"
    "_Protocol_Max\020\001", 175);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "BWAROProtocol.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_BWAROProtocol_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_BWAROProtocol_2eproto {
  StaticDescriptorInitializer_BWAROProtocol_2eproto() {
    protobuf_AddDesc_BWAROProtocol_2eproto();
  }
} static_descriptor_initializer_BWAROProtocol_2eproto_;

const ::google::protobuf::EnumDescriptor* _emPBW2RO_Protocol_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return _emPBW2RO_Protocol_descriptor_;
}
bool _emPBW2RO_Protocol_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* _emPRO2BW_Protocol_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return _emPRO2BW_Protocol_descriptor_;
}
bool _emPRO2BW_Protocol_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
