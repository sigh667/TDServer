// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "WABWProtocol.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::EnumDescriptor* _emPW2BW_Protocol_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* _emW2BW_Protocol_PvpWar_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* _emPBW2W_Protocol_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* _emBW2W_Protocol_Common_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_WABWProtocol_2eproto() {
  protobuf_AddDesc_WABWProtocol_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "WABWProtocol.proto");
  GOOGLE_CHECK(file != NULL);
  _emPW2BW_Protocol_descriptor_ = file->enum_type(0);
  _emW2BW_Protocol_PvpWar_descriptor_ = file->enum_type(1);
  _emPBW2W_Protocol_descriptor_ = file->enum_type(2);
  _emBW2W_Protocol_Common_descriptor_ = file->enum_type(3);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_WABWProtocol_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_WABWProtocol_2eproto() {
}

void protobuf_AddDesc_WABWProtocol_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\022WABWProtocol.proto*\214\001\n\021_emPW2BW_Protoc"
    "ol\022\034\n\030P_W2BW_Protocol_Register\020\000\022\036\n\032P_W2"
    "BW_Protocol_GameConnet\020\001\022\032\n\026P_W2BW_Proto"
    "col_PvpWar\020\002\022\035\n\031P_W2BW_Protocol_SinglePv"
    "p\020\003*\320\001\n\027_emW2BW_Protocol_PvpWar\022$\n W2BW_"
    "Protocol_PvpWar_SingleMatch\020\000\022\"\n\036W2BW_Pr"
    "otocol_PvpWar_TeamMatch\020\001\022\"\n\036W2BW_Protoc"
    "ol_PvpWar_ExitMatch\020\002\022#\n\037W2BW_Protocol_P"
    "vpWar_ReadyEnter\020\003\022\"\n\036W2BW_Protocol_PvpW"
    "ar_ExitEnter\020\004*J\n\021_emPBW2W_Protocol\022\031\n\025P"
    "_BW2W_Protocol_Scene\020\000\022\032\n\026P_BW2W_Protoco"
    "l_Common\020\001*@\n\027_emBW2W_Protocol_Common\022%\n"
    "!BW2W_Protocol_Common_TransGameMsg\020\000", 516);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "WABWProtocol.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_WABWProtocol_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_WABWProtocol_2eproto {
  StaticDescriptorInitializer_WABWProtocol_2eproto() {
    protobuf_AddDesc_WABWProtocol_2eproto();
  }
} static_descriptor_initializer_WABWProtocol_2eproto_;

const ::google::protobuf::EnumDescriptor* _emPW2BW_Protocol_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return _emPW2BW_Protocol_descriptor_;
}
bool _emPW2BW_Protocol_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* _emW2BW_Protocol_PvpWar_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return _emW2BW_Protocol_PvpWar_descriptor_;
}
bool _emW2BW_Protocol_PvpWar_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* _emPBW2W_Protocol_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return _emPBW2W_Protocol_descriptor_;
}
bool _emPBW2W_Protocol_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* _emBW2W_Protocol_Common_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return _emBW2W_Protocol_Common_descriptor_;
}
bool _emBW2W_Protocol_Common_IsValid(int value) {
  switch(value) {
    case 0:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
