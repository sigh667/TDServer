// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "RCSProtocol.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::EnumDescriptor* _emRCS_Protocol_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* _emRCS_CommonProtocol_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* _emResultNormal_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* _emResultTextCmd_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* _emResultAccountUpdate_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* _emResultAccountRemove_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* _emResultRobotAIUpdate_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_RCSProtocol_2eproto() {
  protobuf_AddDesc_RCSProtocol_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "RCSProtocol.proto");
  GOOGLE_CHECK(file != NULL);
  _emRCS_Protocol_descriptor_ = file->enum_type(0);
  _emRCS_CommonProtocol_descriptor_ = file->enum_type(1);
  _emResultNormal_descriptor_ = file->enum_type(2);
  _emResultTextCmd_descriptor_ = file->enum_type(3);
  _emResultAccountUpdate_descriptor_ = file->enum_type(4);
  _emResultAccountRemove_descriptor_ = file->enum_type(5);
  _emResultRobotAIUpdate_descriptor_ = file->enum_type(6);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_RCSProtocol_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_RCSProtocol_2eproto() {
}

void protobuf_AddDesc_RCSProtocol_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\021RCSProtocol.proto*\'\n\017_emRCS_Protocol\022\024"
    "\n\020P_CommonProtocol\020\000*\275\001\n\025_emRCS_CommonPr"
    "otocol\022\020\n\014ComP_TextCmd\020\000\022\026\n\022ComP_Account"
    "Update\020\001\022\026\n\022ComP_AccountRemove\020\002\022\026\n\022ComP"
    "_RobotAIUpdate\020\003\022\022\n\016ComP_MSGStream\020\004\022\022\n\016"
    "ComP_MSGPrintf\020\005\022\020\n\014ComP_NetStat\020\006\022\020\n\014Co"
    "mP_LoadLua\020\007*8\n\017_emResultNormal\022\023\n\017R_Res"
    "ultSucceed\020\000\022\020\n\014R_ResultFail\020\001*=\n\020_emRes"
    "ultTextCmd\022\025\n\021R_TextCmd_Succeed\020\000\022\022\n\016R_T"
    "extCmd_Fail\020\001*O\n\026_emResultAccountUpdate\022"
    "\033\n\027R_AccountUpdate_Succeed\020\000\022\030\n\024R_Accoun"
    "tUpdate_Fail\020\001*O\n\026_emResultAccountRemove"
    "\022\033\n\027R_AccountRemove_Succeed\020\000\022\030\n\024R_Accou"
    "ntRemove_Fail\020\001*O\n\026_emResultRobotAIUpdat"
    "e\022\033\n\027R_RobotAIUpdate_Succeed\020\000\022\030\n\024R_Robo"
    "tAIUpdate_Fail\020\001", 616);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "RCSProtocol.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_RCSProtocol_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_RCSProtocol_2eproto {
  StaticDescriptorInitializer_RCSProtocol_2eproto() {
    protobuf_AddDesc_RCSProtocol_2eproto();
  }
} static_descriptor_initializer_RCSProtocol_2eproto_;

const ::google::protobuf::EnumDescriptor* _emRCS_Protocol_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return _emRCS_Protocol_descriptor_;
}
bool _emRCS_Protocol_IsValid(int value) {
  switch(value) {
    case 0:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* _emRCS_CommonProtocol_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return _emRCS_CommonProtocol_descriptor_;
}
bool _emRCS_CommonProtocol_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* _emResultNormal_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return _emResultNormal_descriptor_;
}
bool _emResultNormal_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* _emResultTextCmd_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return _emResultTextCmd_descriptor_;
}
bool _emResultTextCmd_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* _emResultAccountUpdate_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return _emResultAccountUpdate_descriptor_;
}
bool _emResultAccountUpdate_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* _emResultAccountRemove_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return _emResultAccountRemove_descriptor_;
}
bool _emResultAccountRemove_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* _emResultRobotAIUpdate_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return _emResultRobotAIUpdate_descriptor_;
}
bool _emResultRobotAIUpdate_IsValid(int value) {
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
