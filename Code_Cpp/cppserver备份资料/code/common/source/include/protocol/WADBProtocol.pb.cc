// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "WADBProtocol.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::EnumDescriptor* _emPW2DB_Protocol_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* _emW2DB_Protocol_Operator_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* _em_W2DB_Event_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* _emPDB2W_Protocol_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* _emDB2W_Protocol_Operator_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_WADBProtocol_2eproto() {
  protobuf_AddDesc_WADBProtocol_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "WADBProtocol.proto");
  GOOGLE_CHECK(file != NULL);
  _emPW2DB_Protocol_descriptor_ = file->enum_type(0);
  _emW2DB_Protocol_Operator_descriptor_ = file->enum_type(1);
  _em_W2DB_Event_descriptor_ = file->enum_type(2);
  _emPDB2W_Protocol_descriptor_ = file->enum_type(3);
  _emDB2W_Protocol_Operator_descriptor_ = file->enum_type(4);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_WADBProtocol_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_WADBProtocol_2eproto() {
}

void protobuf_AddDesc_WADBProtocol_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\022WADBProtocol.proto*(\n\021_emPW2DB_Protoco"
    "l\022\023\n\017P_W2DB_Operator\020\000*\177\n\031_emW2DB_Protoc"
    "ol_Operator\022\037\n\033W2DB_Operator_StartLoad_A"
    "sk\020\000\022\035\n\031W2DB_Operator_DBBlock_Ask\020\001\022\"\n\036W"
    "2DB_Operator_SetStartTime_Ask\020\002*\222\005\n\016_em_"
    "W2DB_Event\022\031\n\025W2DB_Event_World_Load\020\000\022\031\n"
    "\025W2DB_Event_World_Save\020\001\022!\n\035W2DB_Event_W"
    "orld_SetStartTime\020\002\022\033\n\027W2DB_Event_Accoun"
    "t_Load\020\n\022\037\n\033W2DB_Event_PlayerCreate_Ask\020"
    "\013\022\032\n\026W2DB_Event_Player_Load\020\024\022\032\n\026W2DB_Ev"
    "ent_Player_Save\020\025\022\034\n\030W2DB_Event_Player_O"
    "nline\020\026\022\037\n\033W2DB_Event_PlayerDelete_Ask\020\027"
    "\022\037\n\033W2DB_Event_PlayerSelect_Ask\020\030\022\036\n\032W2D"
    "B_Event_OfflineData_Add\020\031\022\036\n\032W2DB_Event_"
    "OfflineData_Del\020\032\022\035\n\031W2DB_Event_Faction_"
    "Create\020(\022\035\n\031W2DB_Event_Faction_Delete\020)\022"
    "\033\n\027W2DB_Event_Faction_Load\020*\022\033\n\027W2DB_Eve"
    "nt_Faction_Save\020+\022\037\n\033W2DB_Event_FactionB"
    "oss_Load\0202\022\037\n\033W2DB_Event_FactionBoss_Sav"
    "e\0203\022!\n\035W2DB_Event_FactionBoss_Delete\0204\022\031"
    "\n\025W2DB_Event_Manor_Load\020=\022\031\n\025W2DB_Event_"
    "Manor_Save\020>*(\n\021_emPDB2W_Protocol\022\023\n\017P_D"
    "B2W_Operator\020\000*\202\004\n\031_emDB2W_Protocol_Oper"
    "ator\022\034\n\030DB2W_Operator_DBRecv_Ack\020\000\022\037\n\033DB"
    "2W_Operator_WorldLoad_Ack\020\001\022!\n\035DB2W_Oper"
    "ator_AccountLoad_Ack\020\002\022\"\n\036DB2W_Operator_"
    "PlayerCreate_Ack\020\003\022\"\n\036DB2W_Operator_Play"
    "erDelete_Ack\020\004\022\"\n\036DB2W_Operator_PlayerSe"
    "lect_Ack\020\005\022 \n\034DB2W_Operator_PlayerLoad_A"
    "ck\020\006\022#\n\037DB2W_Operator_FactionCreate_Ack\020"
    "\007\022!\n\035DB2W_Operator_FactionLoad_Ack\020\010\022!\n\035"
    "DB2W_Operator_FactionSave_Ack\020\t\022%\n!DB2W_"
    "Operator_FactionBossLoad_Ack\020\n\022!\n\035DB2W_O"
    "perator_OfflineDataLoad\020\013\022\037\n\033DB2W_Operat"
    "or_ManorLoad_Ack\020\014\022\037\n\033DB2W_Operator_Mano"
    "rSave_Ack\020\r", 1411);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "WADBProtocol.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_WADBProtocol_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_WADBProtocol_2eproto {
  StaticDescriptorInitializer_WADBProtocol_2eproto() {
    protobuf_AddDesc_WADBProtocol_2eproto();
  }
} static_descriptor_initializer_WADBProtocol_2eproto_;

const ::google::protobuf::EnumDescriptor* _emPW2DB_Protocol_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return _emPW2DB_Protocol_descriptor_;
}
bool _emPW2DB_Protocol_IsValid(int value) {
  switch(value) {
    case 0:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* _emW2DB_Protocol_Operator_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return _emW2DB_Protocol_Operator_descriptor_;
}
bool _emW2DB_Protocol_Operator_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* _em_W2DB_Event_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return _em_W2DB_Event_descriptor_;
}
bool _em_W2DB_Event_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 10:
    case 11:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 40:
    case 41:
    case 42:
    case 43:
    case 50:
    case 51:
    case 52:
    case 61:
    case 62:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* _emPDB2W_Protocol_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return _emPDB2W_Protocol_descriptor_;
}
bool _emPDB2W_Protocol_IsValid(int value) {
  switch(value) {
    case 0:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* _emDB2W_Protocol_Operator_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return _emDB2W_Protocol_Operator_descriptor_;
}
bool _emDB2W_Protocol_Operator_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
