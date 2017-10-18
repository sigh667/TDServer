// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: WAGLMsg.proto

#ifndef PROTOBUF_WAGLMsg_2eproto__INCLUDED
#define PROTOBUF_WAGLMsg_2eproto__INCLUDED

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
#include "PlayerStruct.pb.h"
#include "GameStruct.pb.h"
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_WAGLMsg_2eproto();
void protobuf_AssignDesc_WAGLMsg_2eproto();
void protobuf_ShutdownFile_WAGLMsg_2eproto();

class PBW2GLPlayerLogin;
class PBW2GLPlayerLogout;
class PBW2GLPlayerTransfer;

// ===================================================================

class PBW2GLPlayerLogin : public ::google::protobuf::Message {
 public:
  PBW2GLPlayerLogin();
  virtual ~PBW2GLPlayerLogin();
  
  PBW2GLPlayerLogin(const PBW2GLPlayerLogin& from);
  
  inline PBW2GLPlayerLogin& operator=(const PBW2GLPlayerLogin& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const PBW2GLPlayerLogin& default_instance();
  
  void Swap(PBW2GLPlayerLogin* other);
  
  // implements Message ----------------------------------------------
  
  PBW2GLPlayerLogin* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PBW2GLPlayerLogin& from);
  void MergeFrom(const PBW2GLPlayerLogin& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // optional .PBPlayerGlobalBase info = 1;
  inline bool has_info() const;
  inline void clear_info();
  static const int kInfoFieldNumber = 1;
  inline const ::PBPlayerGlobalBase& info() const;
  inline ::PBPlayerGlobalBase* mutable_info();
  inline ::PBPlayerGlobalBase* release_info();
  
  // optional uint32 logicworldid = 2;
  inline bool has_logicworldid() const;
  inline void clear_logicworldid();
  static const int kLogicworldidFieldNumber = 2;
  inline ::google::protobuf::uint32 logicworldid() const;
  inline void set_logicworldid(::google::protobuf::uint32 value);
  
  // optional uint32 servertype = 3;
  inline bool has_servertype() const;
  inline void clear_servertype();
  static const int kServertypeFieldNumber = 3;
  inline ::google::protobuf::uint32 servertype() const;
  inline void set_servertype(::google::protobuf::uint32 value);
  
  // optional uint32 serverid = 4;
  inline bool has_serverid() const;
  inline void clear_serverid();
  static const int kServeridFieldNumber = 4;
  inline ::google::protobuf::uint32 serverid() const;
  inline void set_serverid(::google::protobuf::uint32 value);
  
  // @@protoc_insertion_point(class_scope:PBW2GLPlayerLogin)
 private:
  inline void set_has_info();
  inline void clear_has_info();
  inline void set_has_logicworldid();
  inline void clear_has_logicworldid();
  inline void set_has_servertype();
  inline void clear_has_servertype();
  inline void set_has_serverid();
  inline void clear_has_serverid();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::PBPlayerGlobalBase* info_;
  ::google::protobuf::uint32 logicworldid_;
  ::google::protobuf::uint32 servertype_;
  ::google::protobuf::uint32 serverid_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void  protobuf_AddDesc_WAGLMsg_2eproto();
  friend void protobuf_AssignDesc_WAGLMsg_2eproto();
  friend void protobuf_ShutdownFile_WAGLMsg_2eproto();
  
  void InitAsDefaultInstance();
  static PBW2GLPlayerLogin* default_instance_;
};
// -------------------------------------------------------------------

class PBW2GLPlayerLogout : public ::google::protobuf::Message {
 public:
  PBW2GLPlayerLogout();
  virtual ~PBW2GLPlayerLogout();
  
  PBW2GLPlayerLogout(const PBW2GLPlayerLogout& from);
  
  inline PBW2GLPlayerLogout& operator=(const PBW2GLPlayerLogout& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const PBW2GLPlayerLogout& default_instance();
  
  void Swap(PBW2GLPlayerLogout* other);
  
  // implements Message ----------------------------------------------
  
  PBW2GLPlayerLogout* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PBW2GLPlayerLogout& from);
  void MergeFrom(const PBW2GLPlayerLogout& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // optional uint32 playerid = 1;
  inline bool has_playerid() const;
  inline void clear_playerid();
  static const int kPlayeridFieldNumber = 1;
  inline ::google::protobuf::uint32 playerid() const;
  inline void set_playerid(::google::protobuf::uint32 value);
  
  // @@protoc_insertion_point(class_scope:PBW2GLPlayerLogout)
 private:
  inline void set_has_playerid();
  inline void clear_has_playerid();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::uint32 playerid_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_WAGLMsg_2eproto();
  friend void protobuf_AssignDesc_WAGLMsg_2eproto();
  friend void protobuf_ShutdownFile_WAGLMsg_2eproto();
  
  void InitAsDefaultInstance();
  static PBW2GLPlayerLogout* default_instance_;
};
// -------------------------------------------------------------------

class PBW2GLPlayerTransfer : public ::google::protobuf::Message {
 public:
  PBW2GLPlayerTransfer();
  virtual ~PBW2GLPlayerTransfer();
  
  PBW2GLPlayerTransfer(const PBW2GLPlayerTransfer& from);
  
  inline PBW2GLPlayerTransfer& operator=(const PBW2GLPlayerTransfer& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const PBW2GLPlayerTransfer& default_instance();
  
  void Swap(PBW2GLPlayerTransfer* other);
  
  // implements Message ----------------------------------------------
  
  PBW2GLPlayerTransfer* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PBW2GLPlayerTransfer& from);
  void MergeFrom(const PBW2GLPlayerTransfer& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // optional uint32 playerid = 1;
  inline bool has_playerid() const;
  inline void clear_playerid();
  static const int kPlayeridFieldNumber = 1;
  inline ::google::protobuf::uint32 playerid() const;
  inline void set_playerid(::google::protobuf::uint32 value);
  
  // optional uint32 logicworldid = 2;
  inline bool has_logicworldid() const;
  inline void clear_logicworldid();
  static const int kLogicworldidFieldNumber = 2;
  inline ::google::protobuf::uint32 logicworldid() const;
  inline void set_logicworldid(::google::protobuf::uint32 value);
  
  // optional uint32 servertype = 3;
  inline bool has_servertype() const;
  inline void clear_servertype();
  static const int kServertypeFieldNumber = 3;
  inline ::google::protobuf::uint32 servertype() const;
  inline void set_servertype(::google::protobuf::uint32 value);
  
  // optional uint32 serverid = 4;
  inline bool has_serverid() const;
  inline void clear_serverid();
  static const int kServeridFieldNumber = 4;
  inline ::google::protobuf::uint32 serverid() const;
  inline void set_serverid(::google::protobuf::uint32 value);
  
  // @@protoc_insertion_point(class_scope:PBW2GLPlayerTransfer)
 private:
  inline void set_has_playerid();
  inline void clear_has_playerid();
  inline void set_has_logicworldid();
  inline void clear_has_logicworldid();
  inline void set_has_servertype();
  inline void clear_has_servertype();
  inline void set_has_serverid();
  inline void clear_has_serverid();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::uint32 playerid_;
  ::google::protobuf::uint32 logicworldid_;
  ::google::protobuf::uint32 servertype_;
  ::google::protobuf::uint32 serverid_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void  protobuf_AddDesc_WAGLMsg_2eproto();
  friend void protobuf_AssignDesc_WAGLMsg_2eproto();
  friend void protobuf_ShutdownFile_WAGLMsg_2eproto();
  
  void InitAsDefaultInstance();
  static PBW2GLPlayerTransfer* default_instance_;
};
// ===================================================================


// ===================================================================

// PBW2GLPlayerLogin

// optional .PBPlayerGlobalBase info = 1;
inline bool PBW2GLPlayerLogin::has_info() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void PBW2GLPlayerLogin::set_has_info() {
  _has_bits_[0] |= 0x00000001u;
}
inline void PBW2GLPlayerLogin::clear_has_info() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void PBW2GLPlayerLogin::clear_info() {
  if (info_ != NULL) info_->::PBPlayerGlobalBase::Clear();
  clear_has_info();
}
inline const ::PBPlayerGlobalBase& PBW2GLPlayerLogin::info() const {
  return info_ != NULL ? *info_ : *default_instance_->info_;
}
inline ::PBPlayerGlobalBase* PBW2GLPlayerLogin::mutable_info() {
  set_has_info();
  if (info_ == NULL) info_ = new ::PBPlayerGlobalBase;
  return info_;
}
inline ::PBPlayerGlobalBase* PBW2GLPlayerLogin::release_info() {
  clear_has_info();
  ::PBPlayerGlobalBase* temp = info_;
  info_ = NULL;
  return temp;
}

// optional uint32 logicworldid = 2;
inline bool PBW2GLPlayerLogin::has_logicworldid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void PBW2GLPlayerLogin::set_has_logicworldid() {
  _has_bits_[0] |= 0x00000002u;
}
inline void PBW2GLPlayerLogin::clear_has_logicworldid() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void PBW2GLPlayerLogin::clear_logicworldid() {
  logicworldid_ = 0u;
  clear_has_logicworldid();
}
inline ::google::protobuf::uint32 PBW2GLPlayerLogin::logicworldid() const {
  return logicworldid_;
}
inline void PBW2GLPlayerLogin::set_logicworldid(::google::protobuf::uint32 value) {
  set_has_logicworldid();
  logicworldid_ = value;
}

// optional uint32 servertype = 3;
inline bool PBW2GLPlayerLogin::has_servertype() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void PBW2GLPlayerLogin::set_has_servertype() {
  _has_bits_[0] |= 0x00000004u;
}
inline void PBW2GLPlayerLogin::clear_has_servertype() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void PBW2GLPlayerLogin::clear_servertype() {
  servertype_ = 0u;
  clear_has_servertype();
}
inline ::google::protobuf::uint32 PBW2GLPlayerLogin::servertype() const {
  return servertype_;
}
inline void PBW2GLPlayerLogin::set_servertype(::google::protobuf::uint32 value) {
  set_has_servertype();
  servertype_ = value;
}

// optional uint32 serverid = 4;
inline bool PBW2GLPlayerLogin::has_serverid() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void PBW2GLPlayerLogin::set_has_serverid() {
  _has_bits_[0] |= 0x00000008u;
}
inline void PBW2GLPlayerLogin::clear_has_serverid() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void PBW2GLPlayerLogin::clear_serverid() {
  serverid_ = 0u;
  clear_has_serverid();
}
inline ::google::protobuf::uint32 PBW2GLPlayerLogin::serverid() const {
  return serverid_;
}
inline void PBW2GLPlayerLogin::set_serverid(::google::protobuf::uint32 value) {
  set_has_serverid();
  serverid_ = value;
}

// -------------------------------------------------------------------

// PBW2GLPlayerLogout

// optional uint32 playerid = 1;
inline bool PBW2GLPlayerLogout::has_playerid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void PBW2GLPlayerLogout::set_has_playerid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void PBW2GLPlayerLogout::clear_has_playerid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void PBW2GLPlayerLogout::clear_playerid() {
  playerid_ = 0u;
  clear_has_playerid();
}
inline ::google::protobuf::uint32 PBW2GLPlayerLogout::playerid() const {
  return playerid_;
}
inline void PBW2GLPlayerLogout::set_playerid(::google::protobuf::uint32 value) {
  set_has_playerid();
  playerid_ = value;
}

// -------------------------------------------------------------------

// PBW2GLPlayerTransfer

// optional uint32 playerid = 1;
inline bool PBW2GLPlayerTransfer::has_playerid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void PBW2GLPlayerTransfer::set_has_playerid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void PBW2GLPlayerTransfer::clear_has_playerid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void PBW2GLPlayerTransfer::clear_playerid() {
  playerid_ = 0u;
  clear_has_playerid();
}
inline ::google::protobuf::uint32 PBW2GLPlayerTransfer::playerid() const {
  return playerid_;
}
inline void PBW2GLPlayerTransfer::set_playerid(::google::protobuf::uint32 value) {
  set_has_playerid();
  playerid_ = value;
}

// optional uint32 logicworldid = 2;
inline bool PBW2GLPlayerTransfer::has_logicworldid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void PBW2GLPlayerTransfer::set_has_logicworldid() {
  _has_bits_[0] |= 0x00000002u;
}
inline void PBW2GLPlayerTransfer::clear_has_logicworldid() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void PBW2GLPlayerTransfer::clear_logicworldid() {
  logicworldid_ = 0u;
  clear_has_logicworldid();
}
inline ::google::protobuf::uint32 PBW2GLPlayerTransfer::logicworldid() const {
  return logicworldid_;
}
inline void PBW2GLPlayerTransfer::set_logicworldid(::google::protobuf::uint32 value) {
  set_has_logicworldid();
  logicworldid_ = value;
}

// optional uint32 servertype = 3;
inline bool PBW2GLPlayerTransfer::has_servertype() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void PBW2GLPlayerTransfer::set_has_servertype() {
  _has_bits_[0] |= 0x00000004u;
}
inline void PBW2GLPlayerTransfer::clear_has_servertype() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void PBW2GLPlayerTransfer::clear_servertype() {
  servertype_ = 0u;
  clear_has_servertype();
}
inline ::google::protobuf::uint32 PBW2GLPlayerTransfer::servertype() const {
  return servertype_;
}
inline void PBW2GLPlayerTransfer::set_servertype(::google::protobuf::uint32 value) {
  set_has_servertype();
  servertype_ = value;
}

// optional uint32 serverid = 4;
inline bool PBW2GLPlayerTransfer::has_serverid() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void PBW2GLPlayerTransfer::set_has_serverid() {
  _has_bits_[0] |= 0x00000008u;
}
inline void PBW2GLPlayerTransfer::clear_has_serverid() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void PBW2GLPlayerTransfer::clear_serverid() {
  serverid_ = 0u;
  clear_has_serverid();
}
inline ::google::protobuf::uint32 PBW2GLPlayerTransfer::serverid() const {
  return serverid_;
}
inline void PBW2GLPlayerTransfer::set_serverid(::google::protobuf::uint32 value) {
  set_has_serverid();
  serverid_ = value;
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_WAGLMsg_2eproto__INCLUDED