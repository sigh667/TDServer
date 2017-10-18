// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Common.proto

package com.mokylin.bleach.protobuf;

public final class CommonMessage {
  private CommonMessage() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistry registry) {
  }
  public interface CurrencyChangeInfoOrBuilder
      extends com.google.protobuf.MessageOrBuilder {

    // required int32 currencyTypeId = 1;
    /**
     * <code>required int32 currencyTypeId = 1;</code>
     *
     * <pre>
     ** 货币类型Id 
     * </pre>
     */
    boolean hasCurrencyTypeId();
    /**
     * <code>required int32 currencyTypeId = 1;</code>
     *
     * <pre>
     ** 货币类型Id 
     * </pre>
     */
    int getCurrencyTypeId();

    // required int64 changeValue = 2;
    /**
     * <code>required int64 changeValue = 2;</code>
     *
     * <pre>
     ** 改变的值 
     * </pre>
     */
    boolean hasChangeValue();
    /**
     * <code>required int64 changeValue = 2;</code>
     *
     * <pre>
     ** 改变的值 
     * </pre>
     */
    long getChangeValue();
  }
  /**
   * Protobuf type {@code com.mokylin.CurrencyChangeInfo}
   *
   * <pre>
   **
   * 货币改变信息
   * </pre>
   */
  public static final class CurrencyChangeInfo extends
      com.google.protobuf.GeneratedMessage
      implements CurrencyChangeInfoOrBuilder {
    // Use CurrencyChangeInfo.newBuilder() to construct.
    private CurrencyChangeInfo(com.google.protobuf.GeneratedMessage.Builder<?> builder) {
      super(builder);
      this.unknownFields = builder.getUnknownFields();
    }
    private CurrencyChangeInfo(boolean noInit) { this.unknownFields = com.google.protobuf.UnknownFieldSet.getDefaultInstance(); }

    private static final CurrencyChangeInfo defaultInstance;
    public static CurrencyChangeInfo getDefaultInstance() {
      return defaultInstance;
    }

    public CurrencyChangeInfo getDefaultInstanceForType() {
      return defaultInstance;
    }

    private final com.google.protobuf.UnknownFieldSet unknownFields;
    @java.lang.Override
    public final com.google.protobuf.UnknownFieldSet
        getUnknownFields() {
      return this.unknownFields;
    }
    private CurrencyChangeInfo(
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
              currencyTypeId_ = input.readInt32();
              break;
            }
            case 16: {
              bitField0_ |= 0x00000002;
              changeValue_ = input.readInt64();
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
      return com.mokylin.bleach.protobuf.CommonMessage.internal_static_com_mokylin_CurrencyChangeInfo_descriptor;
    }

    protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.mokylin.bleach.protobuf.CommonMessage.internal_static_com_mokylin_CurrencyChangeInfo_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo.class, com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo.Builder.class);
    }

    public static com.google.protobuf.Parser<CurrencyChangeInfo> PARSER =
        new com.google.protobuf.AbstractParser<CurrencyChangeInfo>() {
      public CurrencyChangeInfo parsePartialFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws com.google.protobuf.InvalidProtocolBufferException {
        return new CurrencyChangeInfo(input, extensionRegistry);
      }
    };

    @java.lang.Override
    public com.google.protobuf.Parser<CurrencyChangeInfo> getParserForType() {
      return PARSER;
    }

    private int bitField0_;
    // required int32 currencyTypeId = 1;
    public static final int CURRENCYTYPEID_FIELD_NUMBER = 1;
    private int currencyTypeId_;
    /**
     * <code>required int32 currencyTypeId = 1;</code>
     *
     * <pre>
     ** 货币类型Id 
     * </pre>
     */
    public boolean hasCurrencyTypeId() {
      return ((bitField0_ & 0x00000001) == 0x00000001);
    }
    /**
     * <code>required int32 currencyTypeId = 1;</code>
     *
     * <pre>
     ** 货币类型Id 
     * </pre>
     */
    public int getCurrencyTypeId() {
      return currencyTypeId_;
    }

    // required int64 changeValue = 2;
    public static final int CHANGEVALUE_FIELD_NUMBER = 2;
    private long changeValue_;
    /**
     * <code>required int64 changeValue = 2;</code>
     *
     * <pre>
     ** 改变的值 
     * </pre>
     */
    public boolean hasChangeValue() {
      return ((bitField0_ & 0x00000002) == 0x00000002);
    }
    /**
     * <code>required int64 changeValue = 2;</code>
     *
     * <pre>
     ** 改变的值 
     * </pre>
     */
    public long getChangeValue() {
      return changeValue_;
    }

    private void initFields() {
      currencyTypeId_ = 0;
      changeValue_ = 0L;
    }
    private byte memoizedIsInitialized = -1;
    public final boolean isInitialized() {
      byte isInitialized = memoizedIsInitialized;
      if (isInitialized != -1) return isInitialized == 1;

      if (!hasCurrencyTypeId()) {
        memoizedIsInitialized = 0;
        return false;
      }
      if (!hasChangeValue()) {
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
        output.writeInt32(1, currencyTypeId_);
      }
      if (((bitField0_ & 0x00000002) == 0x00000002)) {
        output.writeInt64(2, changeValue_);
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
          .computeInt32Size(1, currencyTypeId_);
      }
      if (((bitField0_ & 0x00000002) == 0x00000002)) {
        size += com.google.protobuf.CodedOutputStream
          .computeInt64Size(2, changeValue_);
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

    public static com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return PARSER.parseFrom(input);
    }
    public static com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseFrom(input, extensionRegistry);
    }
    public static com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      return PARSER.parseDelimitedFrom(input);
    }
    public static com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseDelimitedFrom(input, extensionRegistry);
    }
    public static com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return PARSER.parseFrom(input);
    }
    public static com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseFrom(input, extensionRegistry);
    }

    public static Builder newBuilder() { return Builder.create(); }
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder(com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo prototype) {
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
     * Protobuf type {@code com.mokylin.CurrencyChangeInfo}
     *
     * <pre>
     **
     * 货币改变信息
     * </pre>
     */
    public static final class Builder extends
        com.google.protobuf.GeneratedMessage.Builder<Builder>
       implements com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfoOrBuilder {
      public static final com.google.protobuf.Descriptors.Descriptor
          getDescriptor() {
        return com.mokylin.bleach.protobuf.CommonMessage.internal_static_com_mokylin_CurrencyChangeInfo_descriptor;
      }

      protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
          internalGetFieldAccessorTable() {
        return com.mokylin.bleach.protobuf.CommonMessage.internal_static_com_mokylin_CurrencyChangeInfo_fieldAccessorTable
            .ensureFieldAccessorsInitialized(
                com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo.class, com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo.Builder.class);
      }

      // Construct using com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo.newBuilder()
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
        currencyTypeId_ = 0;
        bitField0_ = (bitField0_ & ~0x00000001);
        changeValue_ = 0L;
        bitField0_ = (bitField0_ & ~0x00000002);
        return this;
      }

      public Builder clone() {
        return create().mergeFrom(buildPartial());
      }

      public com.google.protobuf.Descriptors.Descriptor
          getDescriptorForType() {
        return com.mokylin.bleach.protobuf.CommonMessage.internal_static_com_mokylin_CurrencyChangeInfo_descriptor;
      }

      public com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo getDefaultInstanceForType() {
        return com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo.getDefaultInstance();
      }

      public com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo build() {
        com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }

      public com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo buildPartial() {
        com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo result = new com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo(this);
        int from_bitField0_ = bitField0_;
        int to_bitField0_ = 0;
        if (((from_bitField0_ & 0x00000001) == 0x00000001)) {
          to_bitField0_ |= 0x00000001;
        }
        result.currencyTypeId_ = currencyTypeId_;
        if (((from_bitField0_ & 0x00000002) == 0x00000002)) {
          to_bitField0_ |= 0x00000002;
        }
        result.changeValue_ = changeValue_;
        result.bitField0_ = to_bitField0_;
        onBuilt();
        return result;
      }

      public Builder mergeFrom(com.google.protobuf.Message other) {
        if (other instanceof com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo) {
          return mergeFrom((com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo)other);
        } else {
          super.mergeFrom(other);
          return this;
        }
      }

      public Builder mergeFrom(com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo other) {
        if (other == com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo.getDefaultInstance()) return this;
        if (other.hasCurrencyTypeId()) {
          setCurrencyTypeId(other.getCurrencyTypeId());
        }
        if (other.hasChangeValue()) {
          setChangeValue(other.getChangeValue());
        }
        this.mergeUnknownFields(other.getUnknownFields());
        return this;
      }

      public final boolean isInitialized() {
        if (!hasCurrencyTypeId()) {
          
          return false;
        }
        if (!hasChangeValue()) {
          
          return false;
        }
        return true;
      }

      public Builder mergeFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws java.io.IOException {
        com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo parsedMessage = null;
        try {
          parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
        } catch (com.google.protobuf.InvalidProtocolBufferException e) {
          parsedMessage = (com.mokylin.bleach.protobuf.CommonMessage.CurrencyChangeInfo) e.getUnfinishedMessage();
          throw e;
        } finally {
          if (parsedMessage != null) {
            mergeFrom(parsedMessage);
          }
        }
        return this;
      }
      private int bitField0_;

      // required int32 currencyTypeId = 1;
      private int currencyTypeId_ ;
      /**
       * <code>required int32 currencyTypeId = 1;</code>
       *
       * <pre>
       ** 货币类型Id 
       * </pre>
       */
      public boolean hasCurrencyTypeId() {
        return ((bitField0_ & 0x00000001) == 0x00000001);
      }
      /**
       * <code>required int32 currencyTypeId = 1;</code>
       *
       * <pre>
       ** 货币类型Id 
       * </pre>
       */
      public int getCurrencyTypeId() {
        return currencyTypeId_;
      }
      /**
       * <code>required int32 currencyTypeId = 1;</code>
       *
       * <pre>
       ** 货币类型Id 
       * </pre>
       */
      public Builder setCurrencyTypeId(int value) {
        bitField0_ |= 0x00000001;
        currencyTypeId_ = value;
        onChanged();
        return this;
      }
      /**
       * <code>required int32 currencyTypeId = 1;</code>
       *
       * <pre>
       ** 货币类型Id 
       * </pre>
       */
      public Builder clearCurrencyTypeId() {
        bitField0_ = (bitField0_ & ~0x00000001);
        currencyTypeId_ = 0;
        onChanged();
        return this;
      }

      // required int64 changeValue = 2;
      private long changeValue_ ;
      /**
       * <code>required int64 changeValue = 2;</code>
       *
       * <pre>
       ** 改变的值 
       * </pre>
       */
      public boolean hasChangeValue() {
        return ((bitField0_ & 0x00000002) == 0x00000002);
      }
      /**
       * <code>required int64 changeValue = 2;</code>
       *
       * <pre>
       ** 改变的值 
       * </pre>
       */
      public long getChangeValue() {
        return changeValue_;
      }
      /**
       * <code>required int64 changeValue = 2;</code>
       *
       * <pre>
       ** 改变的值 
       * </pre>
       */
      public Builder setChangeValue(long value) {
        bitField0_ |= 0x00000002;
        changeValue_ = value;
        onChanged();
        return this;
      }
      /**
       * <code>required int64 changeValue = 2;</code>
       *
       * <pre>
       ** 改变的值 
       * </pre>
       */
      public Builder clearChangeValue() {
        bitField0_ = (bitField0_ & ~0x00000002);
        changeValue_ = 0L;
        onChanged();
        return this;
      }

      // @@protoc_insertion_point(builder_scope:com.mokylin.CurrencyChangeInfo)
    }

    static {
      defaultInstance = new CurrencyChangeInfo(true);
      defaultInstance.initFields();
    }

    // @@protoc_insertion_point(class_scope:com.mokylin.CurrencyChangeInfo)
  }

  private static com.google.protobuf.Descriptors.Descriptor
    internal_static_com_mokylin_CurrencyChangeInfo_descriptor;
  private static
    com.google.protobuf.GeneratedMessage.FieldAccessorTable
      internal_static_com_mokylin_CurrencyChangeInfo_fieldAccessorTable;

  public static com.google.protobuf.Descriptors.FileDescriptor
      getDescriptor() {
    return descriptor;
  }
  private static com.google.protobuf.Descriptors.FileDescriptor
      descriptor;
  static {
    java.lang.String[] descriptorData = {
      "\n\014Common.proto\022\013com.mokylin\032\021MessageType" +
      ".proto\"A\n\022CurrencyChangeInfo\022\026\n\016currency" +
      "TypeId\030\001 \002(\005\022\023\n\013changeValue\030\002 \002(\003B,\n\033com" +
      ".mokylin.bleach.protobufB\rCommonMessage"
    };
    com.google.protobuf.Descriptors.FileDescriptor.InternalDescriptorAssigner assigner =
      new com.google.protobuf.Descriptors.FileDescriptor.InternalDescriptorAssigner() {
        public com.google.protobuf.ExtensionRegistry assignDescriptors(
            com.google.protobuf.Descriptors.FileDescriptor root) {
          descriptor = root;
          internal_static_com_mokylin_CurrencyChangeInfo_descriptor =
            getDescriptor().getMessageTypes().get(0);
          internal_static_com_mokylin_CurrencyChangeInfo_fieldAccessorTable = new
            com.google.protobuf.GeneratedMessage.FieldAccessorTable(
              internal_static_com_mokylin_CurrencyChangeInfo_descriptor,
              new java.lang.String[] { "CurrencyTypeId", "ChangeValue", });
          return null;
        }
      };
    com.google.protobuf.Descriptors.FileDescriptor
      .internalBuildGeneratedFileFrom(descriptorData,
        new com.google.protobuf.Descriptors.FileDescriptor[] {
          com.mokylin.bleach.protobuf.MessageType.getDescriptor(),
        }, assigner);
  }

  // @@protoc_insertion_point(outer_class_scope)
}
