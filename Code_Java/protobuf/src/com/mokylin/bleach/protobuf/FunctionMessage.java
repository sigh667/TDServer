// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Function.proto

package com.mokylin.bleach.protobuf;

public final class FunctionMessage {
  private FunctionMessage() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistry registry) {
  }
  public interface GCFunctionOpenListOrBuilder
      extends com.google.protobuf.MessageOrBuilder {

    // repeated int32 funcTypes = 1;
    /**
     * <code>repeated int32 funcTypes = 1;</code>
     *
     * <pre>
     * 开放的功能列表
     * </pre>
     */
    java.util.List<java.lang.Integer> getFuncTypesList();
    /**
     * <code>repeated int32 funcTypes = 1;</code>
     *
     * <pre>
     * 开放的功能列表
     * </pre>
     */
    int getFuncTypesCount();
    /**
     * <code>repeated int32 funcTypes = 1;</code>
     *
     * <pre>
     * 开放的功能列表
     * </pre>
     */
    int getFuncTypes(int index);
  }
  /**
   * Protobuf type {@code com.mokylin.GCFunctionOpenList}
   *
   * <pre>
   **
   * 功能开放消息
   * </pre>
   */
  public static final class GCFunctionOpenList extends
      com.google.protobuf.GeneratedMessage
      implements GCFunctionOpenListOrBuilder {
    // Use GCFunctionOpenList.newBuilder() to construct.
    private GCFunctionOpenList(com.google.protobuf.GeneratedMessage.Builder<?> builder) {
      super(builder);
      this.unknownFields = builder.getUnknownFields();
    }
    private GCFunctionOpenList(boolean noInit) { this.unknownFields = com.google.protobuf.UnknownFieldSet.getDefaultInstance(); }

    private static final GCFunctionOpenList defaultInstance;
    public static GCFunctionOpenList getDefaultInstance() {
      return defaultInstance;
    }

    public GCFunctionOpenList getDefaultInstanceForType() {
      return defaultInstance;
    }

    private final com.google.protobuf.UnknownFieldSet unknownFields;
    @java.lang.Override
    public final com.google.protobuf.UnknownFieldSet
        getUnknownFields() {
      return this.unknownFields;
    }
    private GCFunctionOpenList(
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
              if (!((mutable_bitField0_ & 0x00000001) == 0x00000001)) {
                funcTypes_ = new java.util.ArrayList<java.lang.Integer>();
                mutable_bitField0_ |= 0x00000001;
              }
              funcTypes_.add(input.readInt32());
              break;
            }
            case 10: {
              int length = input.readRawVarint32();
              int limit = input.pushLimit(length);
              if (!((mutable_bitField0_ & 0x00000001) == 0x00000001) && input.getBytesUntilLimit() > 0) {
                funcTypes_ = new java.util.ArrayList<java.lang.Integer>();
                mutable_bitField0_ |= 0x00000001;
              }
              while (input.getBytesUntilLimit() > 0) {
                funcTypes_.add(input.readInt32());
              }
              input.popLimit(limit);
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
        if (((mutable_bitField0_ & 0x00000001) == 0x00000001)) {
          funcTypes_ = java.util.Collections.unmodifiableList(funcTypes_);
        }
        this.unknownFields = unknownFields.build();
        makeExtensionsImmutable();
      }
    }
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.mokylin.bleach.protobuf.FunctionMessage.internal_static_com_mokylin_GCFunctionOpenList_descriptor;
    }

    protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.mokylin.bleach.protobuf.FunctionMessage.internal_static_com_mokylin_GCFunctionOpenList_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList.class, com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList.Builder.class);
    }

    public static com.google.protobuf.Parser<GCFunctionOpenList> PARSER =
        new com.google.protobuf.AbstractParser<GCFunctionOpenList>() {
      public GCFunctionOpenList parsePartialFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws com.google.protobuf.InvalidProtocolBufferException {
        return new GCFunctionOpenList(input, extensionRegistry);
      }
    };

    @java.lang.Override
    public com.google.protobuf.Parser<GCFunctionOpenList> getParserForType() {
      return PARSER;
    }

    // repeated int32 funcTypes = 1;
    public static final int FUNCTYPES_FIELD_NUMBER = 1;
    private java.util.List<java.lang.Integer> funcTypes_;
    /**
     * <code>repeated int32 funcTypes = 1;</code>
     *
     * <pre>
     * 开放的功能列表
     * </pre>
     */
    public java.util.List<java.lang.Integer>
        getFuncTypesList() {
      return funcTypes_;
    }
    /**
     * <code>repeated int32 funcTypes = 1;</code>
     *
     * <pre>
     * 开放的功能列表
     * </pre>
     */
    public int getFuncTypesCount() {
      return funcTypes_.size();
    }
    /**
     * <code>repeated int32 funcTypes = 1;</code>
     *
     * <pre>
     * 开放的功能列表
     * </pre>
     */
    public int getFuncTypes(int index) {
      return funcTypes_.get(index);
    }

    private void initFields() {
      funcTypes_ = java.util.Collections.emptyList();
    }
    private byte memoizedIsInitialized = -1;
    public final boolean isInitialized() {
      byte isInitialized = memoizedIsInitialized;
      if (isInitialized != -1) return isInitialized == 1;

      memoizedIsInitialized = 1;
      return true;
    }

    public void writeTo(com.google.protobuf.CodedOutputStream output)
                        throws java.io.IOException {
      getSerializedSize();
      for (int i = 0; i < funcTypes_.size(); i++) {
        output.writeInt32(1, funcTypes_.get(i));
      }
      getUnknownFields().writeTo(output);
    }

    private int memoizedSerializedSize = -1;
    public int getSerializedSize() {
      int size = memoizedSerializedSize;
      if (size != -1) return size;

      size = 0;
      {
        int dataSize = 0;
        for (int i = 0; i < funcTypes_.size(); i++) {
          dataSize += com.google.protobuf.CodedOutputStream
            .computeInt32SizeNoTag(funcTypes_.get(i));
        }
        size += dataSize;
        size += 1 * getFuncTypesList().size();
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

    public static com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return PARSER.parseFrom(input);
    }
    public static com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseFrom(input, extensionRegistry);
    }
    public static com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      return PARSER.parseDelimitedFrom(input);
    }
    public static com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseDelimitedFrom(input, extensionRegistry);
    }
    public static com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return PARSER.parseFrom(input);
    }
    public static com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseFrom(input, extensionRegistry);
    }

    public static Builder newBuilder() { return Builder.create(); }
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder(com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList prototype) {
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
     * Protobuf type {@code com.mokylin.GCFunctionOpenList}
     *
     * <pre>
     **
     * 功能开放消息
     * </pre>
     */
    public static final class Builder extends
        com.google.protobuf.GeneratedMessage.Builder<Builder>
       implements com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenListOrBuilder {
      public static final com.google.protobuf.Descriptors.Descriptor
          getDescriptor() {
        return com.mokylin.bleach.protobuf.FunctionMessage.internal_static_com_mokylin_GCFunctionOpenList_descriptor;
      }

      protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
          internalGetFieldAccessorTable() {
        return com.mokylin.bleach.protobuf.FunctionMessage.internal_static_com_mokylin_GCFunctionOpenList_fieldAccessorTable
            .ensureFieldAccessorsInitialized(
                com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList.class, com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList.Builder.class);
      }

      // Construct using com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList.newBuilder()
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
        funcTypes_ = java.util.Collections.emptyList();
        bitField0_ = (bitField0_ & ~0x00000001);
        return this;
      }

      public Builder clone() {
        return create().mergeFrom(buildPartial());
      }

      public com.google.protobuf.Descriptors.Descriptor
          getDescriptorForType() {
        return com.mokylin.bleach.protobuf.FunctionMessage.internal_static_com_mokylin_GCFunctionOpenList_descriptor;
      }

      public com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList getDefaultInstanceForType() {
        return com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList.getDefaultInstance();
      }

      public com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList build() {
        com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }

      public com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList buildPartial() {
        com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList result = new com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList(this);
        int from_bitField0_ = bitField0_;
        if (((bitField0_ & 0x00000001) == 0x00000001)) {
          funcTypes_ = java.util.Collections.unmodifiableList(funcTypes_);
          bitField0_ = (bitField0_ & ~0x00000001);
        }
        result.funcTypes_ = funcTypes_;
        onBuilt();
        return result;
      }

      public Builder mergeFrom(com.google.protobuf.Message other) {
        if (other instanceof com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList) {
          return mergeFrom((com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList)other);
        } else {
          super.mergeFrom(other);
          return this;
        }
      }

      public Builder mergeFrom(com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList other) {
        if (other == com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList.getDefaultInstance()) return this;
        if (!other.funcTypes_.isEmpty()) {
          if (funcTypes_.isEmpty()) {
            funcTypes_ = other.funcTypes_;
            bitField0_ = (bitField0_ & ~0x00000001);
          } else {
            ensureFuncTypesIsMutable();
            funcTypes_.addAll(other.funcTypes_);
          }
          onChanged();
        }
        this.mergeUnknownFields(other.getUnknownFields());
        return this;
      }

      public final boolean isInitialized() {
        return true;
      }

      public Builder mergeFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws java.io.IOException {
        com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList parsedMessage = null;
        try {
          parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
        } catch (com.google.protobuf.InvalidProtocolBufferException e) {
          parsedMessage = (com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenList) e.getUnfinishedMessage();
          throw e;
        } finally {
          if (parsedMessage != null) {
            mergeFrom(parsedMessage);
          }
        }
        return this;
      }
      private int bitField0_;

      // repeated int32 funcTypes = 1;
      private java.util.List<java.lang.Integer> funcTypes_ = java.util.Collections.emptyList();
      private void ensureFuncTypesIsMutable() {
        if (!((bitField0_ & 0x00000001) == 0x00000001)) {
          funcTypes_ = new java.util.ArrayList<java.lang.Integer>(funcTypes_);
          bitField0_ |= 0x00000001;
         }
      }
      /**
       * <code>repeated int32 funcTypes = 1;</code>
       *
       * <pre>
       * 开放的功能列表
       * </pre>
       */
      public java.util.List<java.lang.Integer>
          getFuncTypesList() {
        return java.util.Collections.unmodifiableList(funcTypes_);
      }
      /**
       * <code>repeated int32 funcTypes = 1;</code>
       *
       * <pre>
       * 开放的功能列表
       * </pre>
       */
      public int getFuncTypesCount() {
        return funcTypes_.size();
      }
      /**
       * <code>repeated int32 funcTypes = 1;</code>
       *
       * <pre>
       * 开放的功能列表
       * </pre>
       */
      public int getFuncTypes(int index) {
        return funcTypes_.get(index);
      }
      /**
       * <code>repeated int32 funcTypes = 1;</code>
       *
       * <pre>
       * 开放的功能列表
       * </pre>
       */
      public Builder setFuncTypes(
          int index, int value) {
        ensureFuncTypesIsMutable();
        funcTypes_.set(index, value);
        onChanged();
        return this;
      }
      /**
       * <code>repeated int32 funcTypes = 1;</code>
       *
       * <pre>
       * 开放的功能列表
       * </pre>
       */
      public Builder addFuncTypes(int value) {
        ensureFuncTypesIsMutable();
        funcTypes_.add(value);
        onChanged();
        return this;
      }
      /**
       * <code>repeated int32 funcTypes = 1;</code>
       *
       * <pre>
       * 开放的功能列表
       * </pre>
       */
      public Builder addAllFuncTypes(
          java.lang.Iterable<? extends java.lang.Integer> values) {
        ensureFuncTypesIsMutable();
        super.addAll(values, funcTypes_);
        onChanged();
        return this;
      }
      /**
       * <code>repeated int32 funcTypes = 1;</code>
       *
       * <pre>
       * 开放的功能列表
       * </pre>
       */
      public Builder clearFuncTypes() {
        funcTypes_ = java.util.Collections.emptyList();
        bitField0_ = (bitField0_ & ~0x00000001);
        onChanged();
        return this;
      }

      // @@protoc_insertion_point(builder_scope:com.mokylin.GCFunctionOpenList)
    }

    static {
      defaultInstance = new GCFunctionOpenList(true);
      defaultInstance.initFields();
    }

    // @@protoc_insertion_point(class_scope:com.mokylin.GCFunctionOpenList)
  }

  public interface GCFunctionOpenOrBuilder
      extends com.google.protobuf.MessageOrBuilder {

    // required int32 funcType = 1;
    /**
     * <code>required int32 funcType = 1;</code>
     *
     * <pre>
     * 新开放的功能
     * </pre>
     */
    boolean hasFuncType();
    /**
     * <code>required int32 funcType = 1;</code>
     *
     * <pre>
     * 新开放的功能
     * </pre>
     */
    int getFuncType();
  }
  /**
   * Protobuf type {@code com.mokylin.GCFunctionOpen}
   *
   * <pre>
   **
   * 新开放功能消息
   * </pre>
   */
  public static final class GCFunctionOpen extends
      com.google.protobuf.GeneratedMessage
      implements GCFunctionOpenOrBuilder {
    // Use GCFunctionOpen.newBuilder() to construct.
    private GCFunctionOpen(com.google.protobuf.GeneratedMessage.Builder<?> builder) {
      super(builder);
      this.unknownFields = builder.getUnknownFields();
    }
    private GCFunctionOpen(boolean noInit) { this.unknownFields = com.google.protobuf.UnknownFieldSet.getDefaultInstance(); }

    private static final GCFunctionOpen defaultInstance;
    public static GCFunctionOpen getDefaultInstance() {
      return defaultInstance;
    }

    public GCFunctionOpen getDefaultInstanceForType() {
      return defaultInstance;
    }

    private final com.google.protobuf.UnknownFieldSet unknownFields;
    @java.lang.Override
    public final com.google.protobuf.UnknownFieldSet
        getUnknownFields() {
      return this.unknownFields;
    }
    private GCFunctionOpen(
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
              funcType_ = input.readInt32();
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
      return com.mokylin.bleach.protobuf.FunctionMessage.internal_static_com_mokylin_GCFunctionOpen_descriptor;
    }

    protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.mokylin.bleach.protobuf.FunctionMessage.internal_static_com_mokylin_GCFunctionOpen_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen.class, com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen.Builder.class);
    }

    public static com.google.protobuf.Parser<GCFunctionOpen> PARSER =
        new com.google.protobuf.AbstractParser<GCFunctionOpen>() {
      public GCFunctionOpen parsePartialFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws com.google.protobuf.InvalidProtocolBufferException {
        return new GCFunctionOpen(input, extensionRegistry);
      }
    };

    @java.lang.Override
    public com.google.protobuf.Parser<GCFunctionOpen> getParserForType() {
      return PARSER;
    }

    private int bitField0_;
    // required int32 funcType = 1;
    public static final int FUNCTYPE_FIELD_NUMBER = 1;
    private int funcType_;
    /**
     * <code>required int32 funcType = 1;</code>
     *
     * <pre>
     * 新开放的功能
     * </pre>
     */
    public boolean hasFuncType() {
      return ((bitField0_ & 0x00000001) == 0x00000001);
    }
    /**
     * <code>required int32 funcType = 1;</code>
     *
     * <pre>
     * 新开放的功能
     * </pre>
     */
    public int getFuncType() {
      return funcType_;
    }

    private void initFields() {
      funcType_ = 0;
    }
    private byte memoizedIsInitialized = -1;
    public final boolean isInitialized() {
      byte isInitialized = memoizedIsInitialized;
      if (isInitialized != -1) return isInitialized == 1;

      if (!hasFuncType()) {
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
        output.writeInt32(1, funcType_);
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
          .computeInt32Size(1, funcType_);
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

    public static com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return PARSER.parseFrom(input);
    }
    public static com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseFrom(input, extensionRegistry);
    }
    public static com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      return PARSER.parseDelimitedFrom(input);
    }
    public static com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseDelimitedFrom(input, extensionRegistry);
    }
    public static com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return PARSER.parseFrom(input);
    }
    public static com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return PARSER.parseFrom(input, extensionRegistry);
    }

    public static Builder newBuilder() { return Builder.create(); }
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder(com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen prototype) {
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
     * Protobuf type {@code com.mokylin.GCFunctionOpen}
     *
     * <pre>
     **
     * 新开放功能消息
     * </pre>
     */
    public static final class Builder extends
        com.google.protobuf.GeneratedMessage.Builder<Builder>
       implements com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpenOrBuilder {
      public static final com.google.protobuf.Descriptors.Descriptor
          getDescriptor() {
        return com.mokylin.bleach.protobuf.FunctionMessage.internal_static_com_mokylin_GCFunctionOpen_descriptor;
      }

      protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
          internalGetFieldAccessorTable() {
        return com.mokylin.bleach.protobuf.FunctionMessage.internal_static_com_mokylin_GCFunctionOpen_fieldAccessorTable
            .ensureFieldAccessorsInitialized(
                com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen.class, com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen.Builder.class);
      }

      // Construct using com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen.newBuilder()
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
        funcType_ = 0;
        bitField0_ = (bitField0_ & ~0x00000001);
        return this;
      }

      public Builder clone() {
        return create().mergeFrom(buildPartial());
      }

      public com.google.protobuf.Descriptors.Descriptor
          getDescriptorForType() {
        return com.mokylin.bleach.protobuf.FunctionMessage.internal_static_com_mokylin_GCFunctionOpen_descriptor;
      }

      public com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen getDefaultInstanceForType() {
        return com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen.getDefaultInstance();
      }

      public com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen build() {
        com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }

      public com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen buildPartial() {
        com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen result = new com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen(this);
        int from_bitField0_ = bitField0_;
        int to_bitField0_ = 0;
        if (((from_bitField0_ & 0x00000001) == 0x00000001)) {
          to_bitField0_ |= 0x00000001;
        }
        result.funcType_ = funcType_;
        result.bitField0_ = to_bitField0_;
        onBuilt();
        return result;
      }

      public Builder mergeFrom(com.google.protobuf.Message other) {
        if (other instanceof com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen) {
          return mergeFrom((com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen)other);
        } else {
          super.mergeFrom(other);
          return this;
        }
      }

      public Builder mergeFrom(com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen other) {
        if (other == com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen.getDefaultInstance()) return this;
        if (other.hasFuncType()) {
          setFuncType(other.getFuncType());
        }
        this.mergeUnknownFields(other.getUnknownFields());
        return this;
      }

      public final boolean isInitialized() {
        if (!hasFuncType()) {
          
          return false;
        }
        return true;
      }

      public Builder mergeFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws java.io.IOException {
        com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen parsedMessage = null;
        try {
          parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
        } catch (com.google.protobuf.InvalidProtocolBufferException e) {
          parsedMessage = (com.mokylin.bleach.protobuf.FunctionMessage.GCFunctionOpen) e.getUnfinishedMessage();
          throw e;
        } finally {
          if (parsedMessage != null) {
            mergeFrom(parsedMessage);
          }
        }
        return this;
      }
      private int bitField0_;

      // required int32 funcType = 1;
      private int funcType_ ;
      /**
       * <code>required int32 funcType = 1;</code>
       *
       * <pre>
       * 新开放的功能
       * </pre>
       */
      public boolean hasFuncType() {
        return ((bitField0_ & 0x00000001) == 0x00000001);
      }
      /**
       * <code>required int32 funcType = 1;</code>
       *
       * <pre>
       * 新开放的功能
       * </pre>
       */
      public int getFuncType() {
        return funcType_;
      }
      /**
       * <code>required int32 funcType = 1;</code>
       *
       * <pre>
       * 新开放的功能
       * </pre>
       */
      public Builder setFuncType(int value) {
        bitField0_ |= 0x00000001;
        funcType_ = value;
        onChanged();
        return this;
      }
      /**
       * <code>required int32 funcType = 1;</code>
       *
       * <pre>
       * 新开放的功能
       * </pre>
       */
      public Builder clearFuncType() {
        bitField0_ = (bitField0_ & ~0x00000001);
        funcType_ = 0;
        onChanged();
        return this;
      }

      // @@protoc_insertion_point(builder_scope:com.mokylin.GCFunctionOpen)
    }

    static {
      defaultInstance = new GCFunctionOpen(true);
      defaultInstance.initFields();
    }

    // @@protoc_insertion_point(class_scope:com.mokylin.GCFunctionOpen)
  }

  private static com.google.protobuf.Descriptors.Descriptor
    internal_static_com_mokylin_GCFunctionOpenList_descriptor;
  private static
    com.google.protobuf.GeneratedMessage.FieldAccessorTable
      internal_static_com_mokylin_GCFunctionOpenList_fieldAccessorTable;
  private static com.google.protobuf.Descriptors.Descriptor
    internal_static_com_mokylin_GCFunctionOpen_descriptor;
  private static
    com.google.protobuf.GeneratedMessage.FieldAccessorTable
      internal_static_com_mokylin_GCFunctionOpen_fieldAccessorTable;

  public static com.google.protobuf.Descriptors.FileDescriptor
      getDescriptor() {
    return descriptor;
  }
  private static com.google.protobuf.Descriptors.FileDescriptor
      descriptor;
  static {
    java.lang.String[] descriptorData = {
      "\n\016Function.proto\022\013com.mokylin\032\021MessageTy" +
      "pe.proto\".\n\022GCFunctionOpenList\022\021\n\tfuncTy" +
      "pes\030\001 \003(\005:\005\220\265\030\365\003\")\n\016GCFunctionOpen\022\020\n\010fu" +
      "ncType\030\001 \002(\005:\005\220\265\030\366\003B.\n\033com.mokylin.bleac" +
      "h.protobufB\017FunctionMessage"
    };
    com.google.protobuf.Descriptors.FileDescriptor.InternalDescriptorAssigner assigner =
      new com.google.protobuf.Descriptors.FileDescriptor.InternalDescriptorAssigner() {
        public com.google.protobuf.ExtensionRegistry assignDescriptors(
            com.google.protobuf.Descriptors.FileDescriptor root) {
          descriptor = root;
          internal_static_com_mokylin_GCFunctionOpenList_descriptor =
            getDescriptor().getMessageTypes().get(0);
          internal_static_com_mokylin_GCFunctionOpenList_fieldAccessorTable = new
            com.google.protobuf.GeneratedMessage.FieldAccessorTable(
              internal_static_com_mokylin_GCFunctionOpenList_descriptor,
              new java.lang.String[] { "FuncTypes", });
          internal_static_com_mokylin_GCFunctionOpen_descriptor =
            getDescriptor().getMessageTypes().get(1);
          internal_static_com_mokylin_GCFunctionOpen_fieldAccessorTable = new
            com.google.protobuf.GeneratedMessage.FieldAccessorTable(
              internal_static_com_mokylin_GCFunctionOpen_descriptor,
              new java.lang.String[] { "FuncType", });
          com.google.protobuf.ExtensionRegistry registry =
            com.google.protobuf.ExtensionRegistry.newInstance();
          registry.add(com.mokylin.bleach.protobuf.MessageType.gcMessageType);
          registry.add(com.mokylin.bleach.protobuf.MessageType.gcMessageType);
          return registry;
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
