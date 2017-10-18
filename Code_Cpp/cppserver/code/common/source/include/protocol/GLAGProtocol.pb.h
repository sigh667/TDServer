// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: GLAGProtocol.proto

#ifndef PROTOBUF_GLAGProtocol_2eproto__INCLUDED
#define PROTOBUF_GLAGProtocol_2eproto__INCLUDED

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
void  protobuf_AddDesc_GLAGProtocol_2eproto();
void protobuf_AssignDesc_GLAGProtocol_2eproto();
void protobuf_ShutdownFile_GLAGProtocol_2eproto();


enum _emPGL2G_Protocol {
  P_GL2G_Protocol_Start = 0,
  P_GL2G_Protocol_ClientMsg = 1,
  P_GL2G_Protocol_Trade = 2,
  P_GL2G_Protocol_SaleMoney = 3,
  P_GL2G_Protocol_Auction = 4,
  P_GL2G_Protocol_Common = 5,
  P_GL2G_Protocol_Friend = 6,
  P_GL2G_Protocol_WorldBoss = 7
};
bool _emPGL2G_Protocol_IsValid(int value);
const _emPGL2G_Protocol _emPGL2G_Protocol_MIN = P_GL2G_Protocol_Start;
const _emPGL2G_Protocol _emPGL2G_Protocol_MAX = P_GL2G_Protocol_WorldBoss;
const int _emPGL2G_Protocol_ARRAYSIZE = _emPGL2G_Protocol_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emPGL2G_Protocol_descriptor();
inline const ::std::string& _emPGL2G_Protocol_Name(_emPGL2G_Protocol value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emPGL2G_Protocol_descriptor(), value);
}
inline bool _emPGL2G_Protocol_Parse(
    const ::std::string& name, _emPGL2G_Protocol* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emPGL2G_Protocol>(
    _emPGL2G_Protocol_descriptor(), name, value);
}
enum _emGL2G_Protocol_Start {
  GL2G_Start_Register = 0,
  GL2G_Start_UnRegister = 1
};
bool _emGL2G_Protocol_Start_IsValid(int value);
const _emGL2G_Protocol_Start _emGL2G_Protocol_Start_MIN = GL2G_Start_Register;
const _emGL2G_Protocol_Start _emGL2G_Protocol_Start_MAX = GL2G_Start_UnRegister;
const int _emGL2G_Protocol_Start_ARRAYSIZE = _emGL2G_Protocol_Start_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emGL2G_Protocol_Start_descriptor();
inline const ::std::string& _emGL2G_Protocol_Start_Name(_emGL2G_Protocol_Start value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emGL2G_Protocol_Start_descriptor(), value);
}
inline bool _emGL2G_Protocol_Start_Parse(
    const ::std::string& name, _emGL2G_Protocol_Start* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emGL2G_Protocol_Start>(
    _emGL2G_Protocol_Start_descriptor(), name, value);
}
enum _emGL2G_Trade_Protocol {
  GL2G_Trade_Begin = 1,
  GL2G_Trade_End = 2,
  GL2G_Trade_check = 3,
  GL2G_Trade_delete = 4,
  GL2G_Trade_add = 5,
  GL2G_Trade_Return = 6
};
bool _emGL2G_Trade_Protocol_IsValid(int value);
const _emGL2G_Trade_Protocol _emGL2G_Trade_Protocol_MIN = GL2G_Trade_Begin;
const _emGL2G_Trade_Protocol _emGL2G_Trade_Protocol_MAX = GL2G_Trade_Return;
const int _emGL2G_Trade_Protocol_ARRAYSIZE = _emGL2G_Trade_Protocol_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emGL2G_Trade_Protocol_descriptor();
inline const ::std::string& _emGL2G_Trade_Protocol_Name(_emGL2G_Trade_Protocol value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emGL2G_Trade_Protocol_descriptor(), value);
}
inline bool _emGL2G_Trade_Protocol_Parse(
    const ::std::string& name, _emGL2G_Trade_Protocol* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emGL2G_Trade_Protocol>(
    _emGL2G_Trade_Protocol_descriptor(), name, value);
}
enum _emGL2G_SaleMoney_Protocol {
  GL2G_SaleMoney_sale_Fail = 0,
  GL2G_SaleMoney_salecancel_Suc = 1
};
bool _emGL2G_SaleMoney_Protocol_IsValid(int value);
const _emGL2G_SaleMoney_Protocol _emGL2G_SaleMoney_Protocol_MIN = GL2G_SaleMoney_sale_Fail;
const _emGL2G_SaleMoney_Protocol _emGL2G_SaleMoney_Protocol_MAX = GL2G_SaleMoney_salecancel_Suc;
const int _emGL2G_SaleMoney_Protocol_ARRAYSIZE = _emGL2G_SaleMoney_Protocol_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emGL2G_SaleMoney_Protocol_descriptor();
inline const ::std::string& _emGL2G_SaleMoney_Protocol_Name(_emGL2G_SaleMoney_Protocol value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emGL2G_SaleMoney_Protocol_descriptor(), value);
}
inline bool _emGL2G_SaleMoney_Protocol_Parse(
    const ::std::string& name, _emGL2G_SaleMoney_Protocol* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emGL2G_SaleMoney_Protocol>(
    _emGL2G_SaleMoney_Protocol_descriptor(), name, value);
}
enum _emGL2G_Auction_Protocol {
  GL2G_Auction_sale_Fail = 0,
  GL2G_Auction_salecancel_Suc = 1,
  GL2G_Auction_buy_Fail = 2
};
bool _emGL2G_Auction_Protocol_IsValid(int value);
const _emGL2G_Auction_Protocol _emGL2G_Auction_Protocol_MIN = GL2G_Auction_sale_Fail;
const _emGL2G_Auction_Protocol _emGL2G_Auction_Protocol_MAX = GL2G_Auction_buy_Fail;
const int _emGL2G_Auction_Protocol_ARRAYSIZE = _emGL2G_Auction_Protocol_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emGL2G_Auction_Protocol_descriptor();
inline const ::std::string& _emGL2G_Auction_Protocol_Name(_emGL2G_Auction_Protocol value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emGL2G_Auction_Protocol_descriptor(), value);
}
inline bool _emGL2G_Auction_Protocol_Parse(
    const ::std::string& name, _emGL2G_Auction_Protocol* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emGL2G_Auction_Protocol>(
    _emGL2G_Auction_Protocol_descriptor(), name, value);
}
enum _emGL2G_Friend_Protocol {
  GL2G_Friend_Add_Ask_Transfer = 0,
  GL2G_Friend_Answer_Transfer = 1,
  GL2G_Friend_Update = 2,
  GL2G_Friend_Syn = 3,
  GL2G_Friend_Add = 4
};
bool _emGL2G_Friend_Protocol_IsValid(int value);
const _emGL2G_Friend_Protocol _emGL2G_Friend_Protocol_MIN = GL2G_Friend_Add_Ask_Transfer;
const _emGL2G_Friend_Protocol _emGL2G_Friend_Protocol_MAX = GL2G_Friend_Add;
const int _emGL2G_Friend_Protocol_ARRAYSIZE = _emGL2G_Friend_Protocol_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emGL2G_Friend_Protocol_descriptor();
inline const ::std::string& _emGL2G_Friend_Protocol_Name(_emGL2G_Friend_Protocol value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emGL2G_Friend_Protocol_descriptor(), value);
}
inline bool _emGL2G_Friend_Protocol_Parse(
    const ::std::string& name, _emGL2G_Friend_Protocol* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emGL2G_Friend_Protocol>(
    _emGL2G_Friend_Protocol_descriptor(), name, value);
}
enum _emGL2G_WorldBoss_Protocol {
  GL2G_WorldBoss_Status_Ntf = 0,
  GL2G_WorldBoss_KillerInfo_Ack = 1
};
bool _emGL2G_WorldBoss_Protocol_IsValid(int value);
const _emGL2G_WorldBoss_Protocol _emGL2G_WorldBoss_Protocol_MIN = GL2G_WorldBoss_Status_Ntf;
const _emGL2G_WorldBoss_Protocol _emGL2G_WorldBoss_Protocol_MAX = GL2G_WorldBoss_KillerInfo_Ack;
const int _emGL2G_WorldBoss_Protocol_ARRAYSIZE = _emGL2G_WorldBoss_Protocol_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emGL2G_WorldBoss_Protocol_descriptor();
inline const ::std::string& _emGL2G_WorldBoss_Protocol_Name(_emGL2G_WorldBoss_Protocol value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emGL2G_WorldBoss_Protocol_descriptor(), value);
}
inline bool _emGL2G_WorldBoss_Protocol_Parse(
    const ::std::string& name, _emGL2G_WorldBoss_Protocol* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emGL2G_WorldBoss_Protocol>(
    _emGL2G_WorldBoss_Protocol_descriptor(), name, value);
}
enum _emPG2GL_Protocol {
  P_G2GL_Protocol_Player = 0,
  P_G2GL_Protocol_Trade = 1,
  P_G2GL_Protocol_SaleMoney = 2,
  P_G2GL_Protocol_Auction = 3,
  P_G2GL_Protocol_Common = 4,
  P_G2GL_Protocol_TopList = 5,
  P_G2GL_Protocol_Friend = 6,
  P_G2GL_Protocol_WorldBoss = 7
};
bool _emPG2GL_Protocol_IsValid(int value);
const _emPG2GL_Protocol _emPG2GL_Protocol_MIN = P_G2GL_Protocol_Player;
const _emPG2GL_Protocol _emPG2GL_Protocol_MAX = P_G2GL_Protocol_WorldBoss;
const int _emPG2GL_Protocol_ARRAYSIZE = _emPG2GL_Protocol_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emPG2GL_Protocol_descriptor();
inline const ::std::string& _emPG2GL_Protocol_Name(_emPG2GL_Protocol value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emPG2GL_Protocol_descriptor(), value);
}
inline bool _emPG2GL_Protocol_Parse(
    const ::std::string& name, _emPG2GL_Protocol* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emPG2GL_Protocol>(
    _emPG2GL_Protocol_descriptor(), name, value);
}
enum _emGLAG_Common_Protocol {
  GLAG_Common_FindPlayer = 0,
  GLAG_Common_FindPlayerAck = 1,
  GLAG_Common_Mail = 2,
  GLAG_Common_BossInfoUpdate = 3,
  GLAG_Common_BossListAsk = 4,
  GLAG_Common_SearchPlayer = 5,
  GLAG_Common_CallPlayer = 6,
  GLAG_Common_LocatePlayer = 7,
  GLAG_Common_LocatePlayerAck = 8
};
bool _emGLAG_Common_Protocol_IsValid(int value);
const _emGLAG_Common_Protocol _emGLAG_Common_Protocol_MIN = GLAG_Common_FindPlayer;
const _emGLAG_Common_Protocol _emGLAG_Common_Protocol_MAX = GLAG_Common_LocatePlayerAck;
const int _emGLAG_Common_Protocol_ARRAYSIZE = _emGLAG_Common_Protocol_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emGLAG_Common_Protocol_descriptor();
inline const ::std::string& _emGLAG_Common_Protocol_Name(_emGLAG_Common_Protocol value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emGLAG_Common_Protocol_descriptor(), value);
}
inline bool _emGLAG_Common_Protocol_Parse(
    const ::std::string& name, _emGLAG_Common_Protocol* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emGLAG_Common_Protocol>(
    _emGLAG_Common_Protocol_descriptor(), name, value);
}
enum _emG2GL_Player_Protocol {
  G2GL_Player_Syn = 0,
  G2GL_Player_Update = 1
};
bool _emG2GL_Player_Protocol_IsValid(int value);
const _emG2GL_Player_Protocol _emG2GL_Player_Protocol_MIN = G2GL_Player_Syn;
const _emG2GL_Player_Protocol _emG2GL_Player_Protocol_MAX = G2GL_Player_Update;
const int _emG2GL_Player_Protocol_ARRAYSIZE = _emG2GL_Player_Protocol_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emG2GL_Player_Protocol_descriptor();
inline const ::std::string& _emG2GL_Player_Protocol_Name(_emG2GL_Player_Protocol value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emG2GL_Player_Protocol_descriptor(), value);
}
inline bool _emG2GL_Player_Protocol_Parse(
    const ::std::string& name, _emG2GL_Player_Protocol* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emG2GL_Player_Protocol>(
    _emG2GL_Player_Protocol_descriptor(), name, value);
}
enum _emG2GL_Trade_Protocol {
  G2GL_Trade_apply = 0,
  G2GL_Trade_answer = 1,
  G2GL_Trade_addItem = 2,
  G2GL_Trade_delItem = 3,
  G2GL_Trade_setMoney = 4,
  G2GL_Trade_lock = 5,
  G2GL_Trade_unlock = 6,
  G2GL_Trade_confirm = 7,
  G2GL_Trade_cancel = 8,
  G2GL_Trade_interupt = 9,
  G2GL_Trade_check = 10,
  G2GL_Trade_delete = 11
};
bool _emG2GL_Trade_Protocol_IsValid(int value);
const _emG2GL_Trade_Protocol _emG2GL_Trade_Protocol_MIN = G2GL_Trade_apply;
const _emG2GL_Trade_Protocol _emG2GL_Trade_Protocol_MAX = G2GL_Trade_delete;
const int _emG2GL_Trade_Protocol_ARRAYSIZE = _emG2GL_Trade_Protocol_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emG2GL_Trade_Protocol_descriptor();
inline const ::std::string& _emG2GL_Trade_Protocol_Name(_emG2GL_Trade_Protocol value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emG2GL_Trade_Protocol_descriptor(), value);
}
inline bool _emG2GL_Trade_Protocol_Parse(
    const ::std::string& name, _emG2GL_Trade_Protocol* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emG2GL_Trade_Protocol>(
    _emG2GL_Trade_Protocol_descriptor(), name, value);
}
enum _emG2GL_SaleMoney_Protocol {
  G2GL_SaleMoney_sale = 0,
  G2GL_SaleMoney_salecancel = 1,
  G2GL_SaleMoney_list = 2,
  G2GL_SaleMoney_supply = 3,
  G2GL_SaleMoney_supplyextra = 4,
  G2GL_SaleMoney_tradelog = 5
};
bool _emG2GL_SaleMoney_Protocol_IsValid(int value);
const _emG2GL_SaleMoney_Protocol _emG2GL_SaleMoney_Protocol_MIN = G2GL_SaleMoney_sale;
const _emG2GL_SaleMoney_Protocol _emG2GL_SaleMoney_Protocol_MAX = G2GL_SaleMoney_tradelog;
const int _emG2GL_SaleMoney_Protocol_ARRAYSIZE = _emG2GL_SaleMoney_Protocol_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emG2GL_SaleMoney_Protocol_descriptor();
inline const ::std::string& _emG2GL_SaleMoney_Protocol_Name(_emG2GL_SaleMoney_Protocol value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emG2GL_SaleMoney_Protocol_descriptor(), value);
}
inline bool _emG2GL_SaleMoney_Protocol_Parse(
    const ::std::string& name, _emG2GL_SaleMoney_Protocol* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emG2GL_SaleMoney_Protocol>(
    _emG2GL_SaleMoney_Protocol_descriptor(), name, value);
}
enum _emG2GL_Auction_Protocol {
  G2GL_Auction_Sale = 0,
  G2GL_Auction_SaleCancel = 1,
  G2GL_Auction_Buy = 2,
  G2GL_Auction_List = 3,
  G2GL_Auction_Ave = 4
};
bool _emG2GL_Auction_Protocol_IsValid(int value);
const _emG2GL_Auction_Protocol _emG2GL_Auction_Protocol_MIN = G2GL_Auction_Sale;
const _emG2GL_Auction_Protocol _emG2GL_Auction_Protocol_MAX = G2GL_Auction_Ave;
const int _emG2GL_Auction_Protocol_ARRAYSIZE = _emG2GL_Auction_Protocol_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emG2GL_Auction_Protocol_descriptor();
inline const ::std::string& _emG2GL_Auction_Protocol_Name(_emG2GL_Auction_Protocol value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emG2GL_Auction_Protocol_descriptor(), value);
}
inline bool _emG2GL_Auction_Protocol_Parse(
    const ::std::string& name, _emG2GL_Auction_Protocol* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emG2GL_Auction_Protocol>(
    _emG2GL_Auction_Protocol_descriptor(), name, value);
}
enum _emG2GL_TopList_Protocol {
  G2GL_TopList_Update = 0,
  G2GL_TopList_List = 1,
  G2GL_TopList_Player = 2,
  G2GL_TopList_SetClearTime = 3
};
bool _emG2GL_TopList_Protocol_IsValid(int value);
const _emG2GL_TopList_Protocol _emG2GL_TopList_Protocol_MIN = G2GL_TopList_Update;
const _emG2GL_TopList_Protocol _emG2GL_TopList_Protocol_MAX = G2GL_TopList_SetClearTime;
const int _emG2GL_TopList_Protocol_ARRAYSIZE = _emG2GL_TopList_Protocol_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emG2GL_TopList_Protocol_descriptor();
inline const ::std::string& _emG2GL_TopList_Protocol_Name(_emG2GL_TopList_Protocol value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emG2GL_TopList_Protocol_descriptor(), value);
}
inline bool _emG2GL_TopList_Protocol_Parse(
    const ::std::string& name, _emG2GL_TopList_Protocol* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emG2GL_TopList_Protocol>(
    _emG2GL_TopList_Protocol_descriptor(), name, value);
}
enum _emG2GL_Friend_Protocol {
  G2GL_Friend_Add_Ask_Transfer = 0,
  G2GL_Friend_Answer_Transfer = 1,
  G2GL_Friend_Find_Transfer = 2,
  G2GL_Friend_Syn_ToGlobal = 3,
  G2GL_Friend_Add_ToGlobal = 4,
  G2GL_Friend_Del_ToGlobal = 5
};
bool _emG2GL_Friend_Protocol_IsValid(int value);
const _emG2GL_Friend_Protocol _emG2GL_Friend_Protocol_MIN = G2GL_Friend_Add_Ask_Transfer;
const _emG2GL_Friend_Protocol _emG2GL_Friend_Protocol_MAX = G2GL_Friend_Del_ToGlobal;
const int _emG2GL_Friend_Protocol_ARRAYSIZE = _emG2GL_Friend_Protocol_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emG2GL_Friend_Protocol_descriptor();
inline const ::std::string& _emG2GL_Friend_Protocol_Name(_emG2GL_Friend_Protocol value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emG2GL_Friend_Protocol_descriptor(), value);
}
inline bool _emG2GL_Friend_Protocol_Parse(
    const ::std::string& name, _emG2GL_Friend_Protocol* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emG2GL_Friend_Protocol>(
    _emG2GL_Friend_Protocol_descriptor(), name, value);
}
enum _emG2GL_WorldBoss_Protocol {
  G2GL_WorldBoss_Status_Report_Ask = 0,
  G2GL_WorldBoss_KillInfo_Ask = 1
};
bool _emG2GL_WorldBoss_Protocol_IsValid(int value);
const _emG2GL_WorldBoss_Protocol _emG2GL_WorldBoss_Protocol_MIN = G2GL_WorldBoss_Status_Report_Ask;
const _emG2GL_WorldBoss_Protocol _emG2GL_WorldBoss_Protocol_MAX = G2GL_WorldBoss_KillInfo_Ask;
const int _emG2GL_WorldBoss_Protocol_ARRAYSIZE = _emG2GL_WorldBoss_Protocol_MAX + 1;

const ::google::protobuf::EnumDescriptor* _emG2GL_WorldBoss_Protocol_descriptor();
inline const ::std::string& _emG2GL_WorldBoss_Protocol_Name(_emG2GL_WorldBoss_Protocol value) {
  return ::google::protobuf::internal::NameOfEnum(
    _emG2GL_WorldBoss_Protocol_descriptor(), value);
}
inline bool _emG2GL_WorldBoss_Protocol_Parse(
    const ::std::string& name, _emG2GL_WorldBoss_Protocol* value) {
  return ::google::protobuf::internal::ParseNamedEnum<_emG2GL_WorldBoss_Protocol>(
    _emG2GL_WorldBoss_Protocol_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< _emPGL2G_Protocol>() {
  return _emPGL2G_Protocol_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< _emGL2G_Protocol_Start>() {
  return _emGL2G_Protocol_Start_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< _emGL2G_Trade_Protocol>() {
  return _emGL2G_Trade_Protocol_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< _emGL2G_SaleMoney_Protocol>() {
  return _emGL2G_SaleMoney_Protocol_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< _emGL2G_Auction_Protocol>() {
  return _emGL2G_Auction_Protocol_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< _emGL2G_Friend_Protocol>() {
  return _emGL2G_Friend_Protocol_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< _emGL2G_WorldBoss_Protocol>() {
  return _emGL2G_WorldBoss_Protocol_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< _emPG2GL_Protocol>() {
  return _emPG2GL_Protocol_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< _emGLAG_Common_Protocol>() {
  return _emGLAG_Common_Protocol_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< _emG2GL_Player_Protocol>() {
  return _emG2GL_Player_Protocol_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< _emG2GL_Trade_Protocol>() {
  return _emG2GL_Trade_Protocol_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< _emG2GL_SaleMoney_Protocol>() {
  return _emG2GL_SaleMoney_Protocol_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< _emG2GL_Auction_Protocol>() {
  return _emG2GL_Auction_Protocol_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< _emG2GL_TopList_Protocol>() {
  return _emG2GL_TopList_Protocol_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< _emG2GL_Friend_Protocol>() {
  return _emG2GL_Friend_Protocol_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< _emG2GL_WorldBoss_Protocol>() {
  return _emG2GL_WorldBoss_Protocol_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_GLAGProtocol_2eproto__INCLUDED