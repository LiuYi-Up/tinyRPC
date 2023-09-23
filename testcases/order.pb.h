// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: order.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_order_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_order_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3019000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3019004 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/service.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_order_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_order_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_order_2eproto;
class makeOrderRequest;
struct makeOrderRequestDefaultTypeInternal;
extern makeOrderRequestDefaultTypeInternal _makeOrderRequest_default_instance_;
class makeOrderResponse;
struct makeOrderResponseDefaultTypeInternal;
extern makeOrderResponseDefaultTypeInternal _makeOrderResponse_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::makeOrderRequest* Arena::CreateMaybeMessage<::makeOrderRequest>(Arena*);
template<> ::makeOrderResponse* Arena::CreateMaybeMessage<::makeOrderResponse>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

// ===================================================================

class makeOrderRequest final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:makeOrderRequest) */ {
 public:
  inline makeOrderRequest() : makeOrderRequest(nullptr) {}
  ~makeOrderRequest() override;
  explicit constexpr makeOrderRequest(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  makeOrderRequest(const makeOrderRequest& from);
  makeOrderRequest(makeOrderRequest&& from) noexcept
    : makeOrderRequest() {
    *this = ::std::move(from);
  }

  inline makeOrderRequest& operator=(const makeOrderRequest& from) {
    CopyFrom(from);
    return *this;
  }
  inline makeOrderRequest& operator=(makeOrderRequest&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const makeOrderRequest& default_instance() {
    return *internal_default_instance();
  }
  static inline const makeOrderRequest* internal_default_instance() {
    return reinterpret_cast<const makeOrderRequest*>(
               &_makeOrderRequest_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(makeOrderRequest& a, makeOrderRequest& b) {
    a.Swap(&b);
  }
  inline void Swap(makeOrderRequest* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(makeOrderRequest* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  makeOrderRequest* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<makeOrderRequest>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const makeOrderRequest& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const makeOrderRequest& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(makeOrderRequest* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "makeOrderRequest";
  }
  protected:
  explicit makeOrderRequest(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kGoodFieldNumber = 2,
    kPriceFieldNumber = 1,
  };
  // string good = 2;
  void clear_good();
  const std::string& good() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_good(ArgT0&& arg0, ArgT... args);
  std::string* mutable_good();
  PROTOBUF_NODISCARD std::string* release_good();
  void set_allocated_good(std::string* good);
  private:
  const std::string& _internal_good() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_good(const std::string& value);
  std::string* _internal_mutable_good();
  public:

  // int32 price = 1;
  void clear_price();
  int32_t price() const;
  void set_price(int32_t value);
  private:
  int32_t _internal_price() const;
  void _internal_set_price(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:makeOrderRequest)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr good_;
  int32_t price_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_order_2eproto;
};
// -------------------------------------------------------------------

class makeOrderResponse final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:makeOrderResponse) */ {
 public:
  inline makeOrderResponse() : makeOrderResponse(nullptr) {}
  ~makeOrderResponse() override;
  explicit constexpr makeOrderResponse(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  makeOrderResponse(const makeOrderResponse& from);
  makeOrderResponse(makeOrderResponse&& from) noexcept
    : makeOrderResponse() {
    *this = ::std::move(from);
  }

  inline makeOrderResponse& operator=(const makeOrderResponse& from) {
    CopyFrom(from);
    return *this;
  }
  inline makeOrderResponse& operator=(makeOrderResponse&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const makeOrderResponse& default_instance() {
    return *internal_default_instance();
  }
  static inline const makeOrderResponse* internal_default_instance() {
    return reinterpret_cast<const makeOrderResponse*>(
               &_makeOrderResponse_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(makeOrderResponse& a, makeOrderResponse& b) {
    a.Swap(&b);
  }
  inline void Swap(makeOrderResponse* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(makeOrderResponse* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  makeOrderResponse* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<makeOrderResponse>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const makeOrderResponse& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const makeOrderResponse& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(makeOrderResponse* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "makeOrderResponse";
  }
  protected:
  explicit makeOrderResponse(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kResInfoFieldNumber = 2,
    kOrderIdFieldNumber = 3,
    kResCodeFieldNumber = 1,
  };
  // string res_info = 2;
  void clear_res_info();
  const std::string& res_info() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_res_info(ArgT0&& arg0, ArgT... args);
  std::string* mutable_res_info();
  PROTOBUF_NODISCARD std::string* release_res_info();
  void set_allocated_res_info(std::string* res_info);
  private:
  const std::string& _internal_res_info() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_res_info(const std::string& value);
  std::string* _internal_mutable_res_info();
  public:

  // string order_id = 3;
  void clear_order_id();
  const std::string& order_id() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_order_id(ArgT0&& arg0, ArgT... args);
  std::string* mutable_order_id();
  PROTOBUF_NODISCARD std::string* release_order_id();
  void set_allocated_order_id(std::string* order_id);
  private:
  const std::string& _internal_order_id() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_order_id(const std::string& value);
  std::string* _internal_mutable_order_id();
  public:

  // int32 res_code = 1;
  void clear_res_code();
  int32_t res_code() const;
  void set_res_code(int32_t value);
  private:
  int32_t _internal_res_code() const;
  void _internal_set_res_code(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:makeOrderResponse)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr res_info_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr order_id_;
  int32_t res_code_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_order_2eproto;
};
// ===================================================================

class Order_Stub;

class Order : public ::PROTOBUF_NAMESPACE_ID::Service {
 protected:
  // This class should be treated as an abstract interface.
  inline Order() {};
 public:
  virtual ~Order();

  typedef Order_Stub Stub;

  static const ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor* descriptor();

  virtual void makeOrder(::PROTOBUF_NAMESPACE_ID::RpcController* controller,
                       const ::makeOrderRequest* request,
                       ::makeOrderResponse* response,
                       ::google::protobuf::Closure* done);

  // implements Service ----------------------------------------------

  const ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor* GetDescriptor();
  void CallMethod(const ::PROTOBUF_NAMESPACE_ID::MethodDescriptor* method,
                  ::PROTOBUF_NAMESPACE_ID::RpcController* controller,
                  const ::PROTOBUF_NAMESPACE_ID::Message* request,
                  ::PROTOBUF_NAMESPACE_ID::Message* response,
                  ::google::protobuf::Closure* done);
  const ::PROTOBUF_NAMESPACE_ID::Message& GetRequestPrototype(
    const ::PROTOBUF_NAMESPACE_ID::MethodDescriptor* method) const;
  const ::PROTOBUF_NAMESPACE_ID::Message& GetResponsePrototype(
    const ::PROTOBUF_NAMESPACE_ID::MethodDescriptor* method) const;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(Order);
};

class Order_Stub : public Order {
 public:
  Order_Stub(::PROTOBUF_NAMESPACE_ID::RpcChannel* channel);
  Order_Stub(::PROTOBUF_NAMESPACE_ID::RpcChannel* channel,
                   ::PROTOBUF_NAMESPACE_ID::Service::ChannelOwnership ownership);
  ~Order_Stub();

  inline ::PROTOBUF_NAMESPACE_ID::RpcChannel* channel() { return channel_; }

  // implements Order ------------------------------------------

  void makeOrder(::PROTOBUF_NAMESPACE_ID::RpcController* controller,
                       const ::makeOrderRequest* request,
                       ::makeOrderResponse* response,
                       ::google::protobuf::Closure* done);
 private:
  ::PROTOBUF_NAMESPACE_ID::RpcChannel* channel_;
  bool owns_channel_;
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(Order_Stub);
};


// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// makeOrderRequest

// int32 price = 1;
inline void makeOrderRequest::clear_price() {
  price_ = 0;
}
inline int32_t makeOrderRequest::_internal_price() const {
  return price_;
}
inline int32_t makeOrderRequest::price() const {
  // @@protoc_insertion_point(field_get:makeOrderRequest.price)
  return _internal_price();
}
inline void makeOrderRequest::_internal_set_price(int32_t value) {
  
  price_ = value;
}
inline void makeOrderRequest::set_price(int32_t value) {
  _internal_set_price(value);
  // @@protoc_insertion_point(field_set:makeOrderRequest.price)
}

// string good = 2;
inline void makeOrderRequest::clear_good() {
  good_.ClearToEmpty();
}
inline const std::string& makeOrderRequest::good() const {
  // @@protoc_insertion_point(field_get:makeOrderRequest.good)
  return _internal_good();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void makeOrderRequest::set_good(ArgT0&& arg0, ArgT... args) {
 
 good_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:makeOrderRequest.good)
}
inline std::string* makeOrderRequest::mutable_good() {
  std::string* _s = _internal_mutable_good();
  // @@protoc_insertion_point(field_mutable:makeOrderRequest.good)
  return _s;
}
inline const std::string& makeOrderRequest::_internal_good() const {
  return good_.Get();
}
inline void makeOrderRequest::_internal_set_good(const std::string& value) {
  
  good_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* makeOrderRequest::_internal_mutable_good() {
  
  return good_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* makeOrderRequest::release_good() {
  // @@protoc_insertion_point(field_release:makeOrderRequest.good)
  return good_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void makeOrderRequest::set_allocated_good(std::string* good) {
  if (good != nullptr) {
    
  } else {
    
  }
  good_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), good,
      GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (good_.IsDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited())) {
    good_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:makeOrderRequest.good)
}

// -------------------------------------------------------------------

// makeOrderResponse

// int32 res_code = 1;
inline void makeOrderResponse::clear_res_code() {
  res_code_ = 0;
}
inline int32_t makeOrderResponse::_internal_res_code() const {
  return res_code_;
}
inline int32_t makeOrderResponse::res_code() const {
  // @@protoc_insertion_point(field_get:makeOrderResponse.res_code)
  return _internal_res_code();
}
inline void makeOrderResponse::_internal_set_res_code(int32_t value) {
  
  res_code_ = value;
}
inline void makeOrderResponse::set_res_code(int32_t value) {
  _internal_set_res_code(value);
  // @@protoc_insertion_point(field_set:makeOrderResponse.res_code)
}

// string res_info = 2;
inline void makeOrderResponse::clear_res_info() {
  res_info_.ClearToEmpty();
}
inline const std::string& makeOrderResponse::res_info() const {
  // @@protoc_insertion_point(field_get:makeOrderResponse.res_info)
  return _internal_res_info();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void makeOrderResponse::set_res_info(ArgT0&& arg0, ArgT... args) {
 
 res_info_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:makeOrderResponse.res_info)
}
inline std::string* makeOrderResponse::mutable_res_info() {
  std::string* _s = _internal_mutable_res_info();
  // @@protoc_insertion_point(field_mutable:makeOrderResponse.res_info)
  return _s;
}
inline const std::string& makeOrderResponse::_internal_res_info() const {
  return res_info_.Get();
}
inline void makeOrderResponse::_internal_set_res_info(const std::string& value) {
  
  res_info_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* makeOrderResponse::_internal_mutable_res_info() {
  
  return res_info_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* makeOrderResponse::release_res_info() {
  // @@protoc_insertion_point(field_release:makeOrderResponse.res_info)
  return res_info_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void makeOrderResponse::set_allocated_res_info(std::string* res_info) {
  if (res_info != nullptr) {
    
  } else {
    
  }
  res_info_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), res_info,
      GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (res_info_.IsDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited())) {
    res_info_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:makeOrderResponse.res_info)
}

// string order_id = 3;
inline void makeOrderResponse::clear_order_id() {
  order_id_.ClearToEmpty();
}
inline const std::string& makeOrderResponse::order_id() const {
  // @@protoc_insertion_point(field_get:makeOrderResponse.order_id)
  return _internal_order_id();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void makeOrderResponse::set_order_id(ArgT0&& arg0, ArgT... args) {
 
 order_id_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:makeOrderResponse.order_id)
}
inline std::string* makeOrderResponse::mutable_order_id() {
  std::string* _s = _internal_mutable_order_id();
  // @@protoc_insertion_point(field_mutable:makeOrderResponse.order_id)
  return _s;
}
inline const std::string& makeOrderResponse::_internal_order_id() const {
  return order_id_.Get();
}
inline void makeOrderResponse::_internal_set_order_id(const std::string& value) {
  
  order_id_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* makeOrderResponse::_internal_mutable_order_id() {
  
  return order_id_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* makeOrderResponse::release_order_id() {
  // @@protoc_insertion_point(field_release:makeOrderResponse.order_id)
  return order_id_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void makeOrderResponse::set_allocated_order_id(std::string* order_id) {
  if (order_id != nullptr) {
    
  } else {
    
  }
  order_id_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), order_id,
      GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (order_id_.IsDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited())) {
    order_id_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:makeOrderResponse.order_id)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_order_2eproto
