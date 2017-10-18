// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: db/DbDailyReward.proto

package com.mokylin.bleach.protobuf.db;

public final class DbDailyReward {
  private DbDailyReward() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistry registry) {
  }
  public interface DailyRewardOrBuilder
      extends com.google.protobuf.MessageOrBuilder {

    // required int32 rewardIndex = 1;
    /**
     * <code>required int32 rewardIndex = 1;</code>
     *
     * <pre>
     *当前将要领取的奖励的index，从0开始//
     * </pre>
     */
    boolean hasRewardIndex();
    /**
     * <code>required int32 rewardIndex = 1;</code>
     *
     * <pre>
     *当前将要领取的奖励的index，从0开始//
     * </pre>
     */
    int getRewardIndex();

    // required int64 lastRewardTime = 2;
    /**
     * <code>required int64 lastRewardTime = 2;</code>
     *
     * <pre>
     *最后一次领奖时间//
     * </pre>
     */
    boolean hasLastRewardTime();
    /**
     * <code>required int64 lastRewardTime = 2;</code>
     *
     * <pre>
     *最后一次领奖时间//
     * </pre>
     */
    long getLastRewardTime();

    // required bool canGetVipReward = 3;
    /**
     * <code>required bool canGetVipReward = 3;</code>
     *
     * <pre>
     *是否可以领取了VIP奖励//
     * </pre>
     */
    boolean hasCanGetVipReward();
    /**
     * <code>required bool canGetVipReward = 3;</code>
     *
     * <pre>
     *是否可以领取了VIP奖励//
     * </pre>
     */
    boolean getCanGetVipReward();
  }
  /**
   * Protobuf type {@code com.mokylin.DailyReward}
   *
   * <pre>
   ** 每日签到奖励存储 
   * </pre>
   */
  public static final class DailyReward extends
      com.google.protobuf.GeneratedMessage
      implements DailyRewardOrBuilder {
    // Use DailyReward.newBuilder() to construct.
    private DailyReward(com.google.protobuf.GeneratedMessage.Builder<?> builder) {
      super(builder);
      this.unknownFields = builder.getUnknownFields();
    }
    private DailyReward(boolean noInit) { this.unknownFields = com.google.protobuf.UnknownFieldSet.getDefaultInstance(); }

    private static final DailyReward defaultInstance;
    public static DailyReward getDefaultInstance() {
      return defaultInstance;
    }

    public DailyReward getDefaultInstanceForType() {
      return defaultInstance;
    }

    private final com.google.protobuf.UnknownFieldSet unknownFields;
    @java.lang.Override
    public final com.google.protobuf.UnknownFieldSet
        getUnknownFields() {
      return this.unknownFields;
    }
    private DailyReward(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      initFields();
      int mutable_bitField0_ = 0;
      com.google.protobuf.UnknownFieldSet.Builder unknownFields =
          com.google.protobuf.UnknownFieldSet.newBuilder();
      try {
        boolean done = false;
        while (!done) {
          int tag = input.readTag();
          switch (tag) {
            case 0:
              done = true;
              break;
            default: {
              if (!parseUnknownField(input, unknownFields,
                                     extensionRegistry, tag)) {
                done = true;
              }
              break;
            }
            case 8: {
              bitField0_ |= 0x00000001;
              rewardIndex_ = input.readInt32();
              break;
            }
            case 16: {
              bitField0_ |= 0x00000002;
              lastRewardTime_ = input.readInt64();
              break;
            }
            case 24: {
              bitField0_ |= 0x00000004;
              canGetVipReward_ = input.readBool();
              break;
            }
          }
        }
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        throw e.setUnfinishedMessage(this);
      } catch (java.io.IOException e) {
        throw new com.google.protobuf.InvalidProtocolBufferException(
            e.getMessage()).setUnfinishedMessage(this);
      } finally {
        this.unknownFields = unknownFields.build();
        makeExtensionsImmutable();
      }
    }
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.mokylin.bleach.protobuf.db.DbDailyReward.internal_static_com_mokylin_DailyReward_descriptor;
    }

    protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.mokylin.bleach.protobuf.db.DbDailyReward.internal_static_com_mokylin_DailyReward_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward.class, com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward.Builder.class);
    }

    public static com.google.protobuf.Parser<DailyReward> PARSER =
        new com.google.protobuf.AbstractParser<DailyReward>() {
      public DailyReward parsePartialFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws com.google.protobuf.InvalidProtocolBufferException {
        return new DailyReward(input, extensionRegistry);
      }
    };

    @java.lang.Override
    public com.google.protobuf.Parser<DailyReward> getParserForType() {
      return PARSER;
    }

    private int bitField0_;
    // required int32 rewardIndex = 1;
    public static final int REWARDINDEX_FIELD_NUMBER = 1;
    private int rewardIndex_;
    /**
     * <code>required int32 rewardIndex = 1;</code>
     *
     * <pre>
     *当前将要领取的奖励的index，从0开始//
     * </pre>
     */
    public boolean hasRewardIndex() {
      return ((bitField0_ & 0x00000001) == 0x00000001);
    }
    /**
     * <code>required int32 rewardIndex = 1;</code>
     *
     * <pre>
     *当前将要领取的奖励的index，从0开始//
     * </pre>
     */
    public int getRewardIndex() {
      return rewardIndex_;
    }

    // required int64 lastRewardTime = 2;
    public static final int LASTREWARDTIME_FIELD_NUMBER = 2;
    private long lastRewardTime_;
    /**
     * <code>required int64 lastRewardTime = 2;</code>
     *
     * <pre>
     *最后一次领奖时间//
     * </pre>
     */
    public boolean hasLastRewardTime() {
      return ((bitField0_ & 0x00000002) == 0x00000002);
    }
    /**
     * <code>required int64 lastRewardTime = 2;</code>
     *
     * <pre>
     *最后一次领奖时间//
     * </pre>
     */
    public long getLastRewardTime() {
      return lastRewardTime_;
    }

    // required bool canGetVipReward = 3;
    public static final int CANGETVIPREWARD_FIELD_NUMBER = 3;
    private boolean canGetVipReward_;
    /**
     * <code>required bool canGetVipReward = 3;</code>
     *
     * <pre>
     *是否可以领取了VIP奖励//
     * </pre>
     */
    public boolean hasCanGetVipReward() {
      return ((bitField0_ & 0x00000004) == 0x00000004);
    }
    /**
     * <code>required bool canGetVipReward = 3;</code>
     *
     * <pre>
     *是否可以领取了VIP奖励//
     * </pre>
     */
    public boolean getCanGetVipReward() {
      return canGetVipReward_;
    }

    private void initFields() {
      rewardIndex_ = 0;
      lastRewardTime_ = 0L;
      canGetVipReward_ = false;
    }
    private byte memoizedIsInitialized = -1;
    public final boolean isInitialized() {
      byte isInitialized = memoizedIsInitialized;
      if (isInitialized != -1) return isInitialized == 1;

      if (!hasRewardIndex()) {
        memoizedIsInitialized = 0;
        return false;
      }
      if (!hasLastRewardTime()) {
        memoizedIsInitialized = 0;
        return false;
      }
      if (!hasCanGetVipReward()) {
        memoizedIsInitialized = 0;
        return false;
      }
      memoizedIsInitialized = 1;
      return true;
    }

    public void writeTo(com.google.protobuf.CodedOutputStream output)
                        throws java.io.IOException {
      getSerializedSize();
      if (((bitField0_ & 0x00000001) == 0x00000001)) {
        output.writeInt32(1, rewardIndex_);
      }
      if (((bitField0_ & 0x00000002) == 0x00000002)) {
        output.writeInt64(2, lastRewardTime_);
      }
      if (((bitField0_ & 0x00000004) == 0x00000004)) {
        output.writeBool(3, canGetVipReward_);
      }
      getUnknownFields().writeTo(output);
    }

    private int memoizedSerializedSize = -1;
    public int getSerializedSize() {
      int size = memoizedSerializedSize;
      if (size != -1) return size;

      size = 0;
      if (((bitField0_ & 0x00000001) == 0x00000001)) {
        size += com.google.protobuf.CodedOutputStream
          .computeInt32Size(1, rewardIndex_);
      }
      if (((bitField0_ & 0x00000002) == 0x00000002)) {
        size += com.google.protobuf.CodedOutputStream
          .computeInt64Size(2, lastRewardTime_);
      }
      if (((bitField0_ & 0x00000004) == 0x00000004)) {
        size += com.google.protobuf.CodedOutputStream
          .computeBoolSize(3, canGetVipReward_);
      }
      size += getUnknownFields().getSerializedSize();
      memoizedSerializedSize = size;
      return size;
    }

    private static final long serialVersionUID = 0L;
    @java.lang.Override
    protected java.lang.Object writeReplace()
        throws java.io.ObjectStreamException {
      return super.writeReplace();
    }

    public static com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return PARSER.parseFrom(input);
    }
    public static com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseFrom(input, extensionRegistry);
    }
    public static com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      return PARSER.parseDelimitedFrom(input);
    }
    public static com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseDelimitedFrom(input, extensionRegistry);
    }
    public static com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return PARSER.parseFrom(input);
    }
    public static com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseFrom(input, extensionRegistry);
    }

    public static Builder newBuilder() { return Builder.create(); }
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder(com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward prototype) {
      return newBuilder().mergeFrom(prototype);
    }
    public Builder toBuilder() { return newBuilder(this); }

    @java.lang.Override
    protected Builder newBuilderForType(
        com.google.protobuf.GeneratedMessage.BuilderParent parent) {
      Builder builder = new Builder(parent);
      return builder;
    }
    /**
     * Protobuf type {@code com.mokylin.DailyReward}
     *
     * <pre>
     ** 每日签到奖励存储 
     * </pre>
     */
    public static final class Builder extends
        com.google.protobuf.GeneratedMessage.Builder<Builder>
       implements com.mokylin.bleach.protobuf.db.DbDailyReward.DailyRewardOrBuilder {
      public static final com.google.protobuf.Descriptors.Descriptor
          getDescriptor() {
        return com.mokylin.bleach.protobuf.db.DbDailyReward.internal_static_com_mokylin_DailyReward_descriptor;
      }

      protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
          internalGetFieldAccessorTable() {
        return com.mokylin.bleach.protobuf.db.DbDailyReward.internal_static_com_mokylin_DailyReward_fieldAccessorTable
            .ensureFieldAccessorsInitialized(
                com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward.class, com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward.Builder.class);
      }

      // Construct using com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward.newBuilder()
      private Builder() {
        maybeForceBuilderInitialization();
      }

      private Builder(
          com.google.protobuf.GeneratedMessage.BuilderParent parent) {
        super(parent);
        maybeForceBuilderInitialization();
      }
      private void maybeForceBuilderInitialization() {
        if (com.google.protobuf.GeneratedMessage.alwaysUseFieldBuilders) {
        }
      }
      private static Builder create() {
        return new Builder();
      }

      public Builder clear() {
        super.clear();
        rewardIndex_ = 0;
        bitField0_ = (bitField0_ & ~0x00000001);
        lastRewardTime_ = 0L;
        bitField0_ = (bitField0_ & ~0x00000002);
        canGetVipReward_ = false;
        bitField0_ = (bitField0_ & ~0x00000004);
        return this;
      }

      public Builder clone() {
        return create().mergeFrom(buildPartial());
      }

      public com.google.protobuf.Descriptors.Descriptor
          getDescriptorForType() {
        return com.mokylin.bleach.protobuf.db.DbDailyReward.internal_static_com_mokylin_DailyReward_descriptor;
      }

      public com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward getDefaultInstanceForType() {
        return com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward.getDefaultInstance();
      }

      public com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward build() {
        com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }

      public com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward buildPartial() {
        com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward result = new com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward(this);
        int from_bitField0_ = bitField0_;
        int to_bitField0_ = 0;
        if (((from_bitField0_ & 0x00000001) == 0x00000001)) {
          to_bitField0_ |= 0x00000001;
        }
        result.rewardIndex_ = rewardIndex_;
        if (((from_bitField0_ & 0x00000002) == 0x00000002)) {
          to_bitField0_ |= 0x00000002;
        }
        result.lastRewardTime_ = lastRewardTime_;
        if (((from_bitField0_ & 0x00000004) == 0x00000004)) {
          to_bitField0_ |= 0x00000004;
        }
        result.canGetVipReward_ = canGetVipReward_;
        result.bitField0_ = to_bitField0_;
        onBuilt();
        return result;
      }

      public Builder mergeFrom(com.google.protobuf.Message other) {
        if (other instanceof com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward) {
          return mergeFrom((com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward)other);
        } else {
          super.mergeFrom(other);
          return this;
        }
      }

      public Builder mergeFrom(com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward other) {
        if (other == com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward.getDefaultInstance()) return this;
        if (other.hasRewardIndex()) {
          setRewardIndex(other.getRewardIndex());
        }
        if (other.hasLastRewardTime()) {
          setLastRewardTime(other.getLastRewardTime());
        }
        if (other.hasCanGetVipReward()) {
          setCanGetVipReward(other.getCanGetVipReward());
        }
        this.mergeUnknownFields(other.getUnknownFields());
        return this;
      }

      public final boolean isInitialized() {
        if (!hasRewardIndex()) {
          
          return false;
        }
        if (!hasLastRewardTime()) {
          
          return false;
        }
        if (!hasCanGetVipReward()) {
          
          return false;
        }
        return true;
      }

      public Builder mergeFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws java.io.IOException {
        com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward parsedMessage = null;
        try {
          parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
        } catch (com.google.protobuf.InvalidProtocolBufferException e) {
          parsedMessage = (com.mokylin.bleach.protobuf.db.DbDailyReward.DailyReward) e.getUnfinishedMessage();
          throw e;
        } finally {
          if (parsedMessage != null) {
            mergeFrom(parsedMessage);
          }
        }
        return this;
      }
      private int bitField0_;

      // required int32 rewardIndex = 1;
      private int rewardIndex_ ;
      /**
       * <code>required int32 rewardIndex = 1;</code>
       *
       * <pre>
       *当前将要领取的奖励的index，从0开始//
       * </pre>
       */
      public boolean hasRewardIndex() {
        return ((bitField0_ & 0x00000001) == 0x00000001);
      }
      /**
       * <code>required int32 rewardIndex = 1;</code>
       *
       * <pre>
       *当前将要领取的奖励的index，从0开始//
       * </pre>
       */
      public int getRewardIndex() {
        return rewardIndex_;
      }
      /**
       * <code>required int32 rewardIndex = 1;</code>
       *
       * <pre>
       *当前将要领取的奖励的index，从0开始//
       * </pre>
       */
      public Builder setRewardIndex(int value) {
        bitField0_ |= 0x00000001;
        rewardIndex_ = value;
        onChanged();
        return this;
      }
      /**
       * <code>required int32 rewardIndex = 1;</code>
       *
       * <pre>
       *当前将要领取的奖励的index，从0开始//
       * </pre>
       */
      public Builder clearRewardIndex() {
        bitField0_ = (bitField0_ & ~0x00000001);
        rewardIndex_ = 0;
        onChanged();
        return this;
      }

      // required int64 lastRewardTime = 2;
      private long lastRewardTime_ ;
      /**
       * <code>required int64 lastRewardTime = 2;</code>
       *
       * <pre>
       *最后一次领奖时间//
       * </pre>
       */
      public boolean hasLastRewardTime() {
        return ((bitField0_ & 0x00000002) == 0x00000002);
      }
      /**
       * <code>required int64 lastRewardTime = 2;</code>
       *
       * <pre>
       *最后一次领奖时间//
       * </pre>
       */
      public long getLastRewardTime() {
        return lastRewardTime_;
      }
      /**
       * <code>required int64 lastRewardTime = 2;</code>
       *
       * <pre>
       *最后一次领奖时间//
       * </pre>
       */
      public Builder setLastRewardTime(long value) {
        bitField0_ |= 0x00000002;
        lastRewardTime_ = value;
        onChanged();
        return this;
      }
      /**
       * <code>required int64 lastRewardTime = 2;</code>
       *
       * <pre>
       *最后一次领奖时间//
       * </pre>
       */
      public Builder clearLastRewardTime() {
        bitField0_ = (bitField0_ & ~0x00000002);
        lastRewardTime_ = 0L;
        onChanged();
        return this;
      }

      // required bool canGetVipReward = 3;
      private boolean canGetVipReward_ ;
      /**
       * <code>required bool canGetVipReward = 3;</code>
       *
       * <pre>
       *是否可以领取了VIP奖励//
       * </pre>
       */
      public boolean hasCanGetVipReward() {
        return ((bitField0_ & 0x00000004) == 0x00000004);
      }
      /**
       * <code>required bool canGetVipReward = 3;</code>
       *
       * <pre>
       *是否可以领取了VIP奖励//
       * </pre>
       */
      public boolean getCanGetVipReward() {
        return canGetVipReward_;
      }
      /**
       * <code>required bool canGetVipReward = 3;</code>
       *
       * <pre>
       *是否可以领取了VIP奖励//
       * </pre>
       */
      public Builder setCanGetVipReward(boolean value) {
        bitField0_ |= 0x00000004;
        canGetVipReward_ = value;
        onChanged();
        return this;
      }
      /**
       * <code>required bool canGetVipReward = 3;</code>
       *
       * <pre>
       *是否可以领取了VIP奖励//
       * </pre>
       */
      public Builder clearCanGetVipReward() {
        bitField0_ = (bitField0_ & ~0x00000004);
        canGetVipReward_ = false;
        onChanged();
        return this;
      }

      // @@protoc_insertion_point(builder_scope:com.mokylin.DailyReward)
    }

    static {
      defaultInstance = new DailyReward(true);
      defaultInstance.initFields();
    }

    // @@protoc_insertion_point(class_scope:com.mokylin.DailyReward)
  }

  private static com.google.protobuf.Descriptors.Descriptor
    internal_static_com_mokylin_DailyReward_descriptor;
  private static
    com.google.protobuf.GeneratedMessage.FieldAccessorTable
      internal_static_com_mokylin_DailyReward_fieldAccessorTable;

  public static com.google.protobuf.Descriptors.FileDescriptor
      getDescriptor() {
    return descriptor;
  }
  private static com.google.protobuf.Descriptors.FileDescriptor
      descriptor;
  static {
    java.lang.String[] descriptorData = {
      "\n\026db/DbDailyReward.proto\022\013com.mokylin\"S\n" +
      "\013DailyReward\022\023\n\013rewardIndex\030\001 \002(\005\022\026\n\016las" +
      "tRewardTime\030\002 \002(\003\022\027\n\017canGetVipReward\030\003 \002" +
      "(\010B/\n\036com.mokylin.bleach.protobuf.dbB\rDb" +
      "DailyReward"
    };
    com.google.protobuf.Descriptors.FileDescriptor.InternalDescriptorAssigner assigner =
      new com.google.protobuf.Descriptors.FileDescriptor.InternalDescriptorAssigner() {
        public com.google.protobuf.ExtensionRegistry assignDescriptors(
            com.google.protobuf.Descriptors.FileDescriptor root) {
          descriptor = root;
          internal_static_com_mokylin_DailyReward_descriptor =
            getDescriptor().getMessageTypes().get(0);
          internal_static_com_mokylin_DailyReward_fieldAccessorTable = new
            com.google.protobuf.GeneratedMessage.FieldAccessorTable(
              internal_static_com_mokylin_DailyReward_descriptor,
              new java.lang.String[] { "RewardIndex", "LastRewardTime", "CanGetVipReward", });
          return null;
        }
      };
    com.google.protobuf.Descriptors.FileDescriptor
      .internalBuildGeneratedFileFrom(descriptorData,
        new com.google.protobuf.Descriptors.FileDescriptor[] {
        }, assigner);
  }

  // @@protoc_insertion_point(outer_class_scope)
}
