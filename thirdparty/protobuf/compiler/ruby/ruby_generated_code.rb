# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: ruby_generated_code.proto

require 'google/protobuf'

Google::Protobuf::DescriptorPool.generated_pool.build do
  add_message "A.B.C.TestMessage" do
    optional :optional_int32, :int32, 1
    optional :optional_int64, :int64, 2
    optional :optional_uint32, :uint32, 3
    optional :optional_uint64, :uint64, 4
    optional :optional_bool, :bool, 5
    optional :optional_double, :double, 6
    optional :optional_float, :float, 7
    optional :optional_string, :string, 8
    optional :optional_bytes, :string, 9
    optional :optional_enum, :enum, 10, "A.B.C.TestEnum"
    optional :optional_msg, :message, 11, "A.B.C.TestMessage"
    repeated :repeated_int32, :int32, 21
    repeated :repeated_int64, :int64, 22
    repeated :repeated_uint32, :uint32, 23
    repeated :repeated_uint64, :uint64, 24
    repeated :repeated_bool, :bool, 25
    repeated :repeated_double, :double, 26
    repeated :repeated_float, :float, 27
    repeated :repeated_string, :string, 28
    repeated :repeated_bytes, :string, 29
    repeated :repeated_enum, :enum, 30, "A.B.C.TestEnum"
    repeated :repeated_msg, :message, 31, "A.B.C.TestMessage"
    map :map_int32_string, :int32, :string, 61
    map :map_int64_string, :int64, :string, 62
    map :map_uint32_string, :uint32, :string, 63
    map :map_uint64_string, :uint64, :string, 64
    map :map_bool_string, :bool, :string, 65
    map :map_string_string, :string, :string, 66
    map :map_string_msg, :string, :message, 67, "A.B.C.TestMessage"
    map :map_string_enum, :string, :enum, 68, "A.B.C.TestEnum"
    map :map_string_int32, :string, :int32, 69
    map :map_string_bool, :string, :bool, 70
    optional :nested_message, :message, 80, "A.B.C.TestMessage.NestedMessage"
    oneof :my_oneof do
      optional :oneof_int32, :int32, 41
      optional :oneof_int64, :int64, 42
      optional :oneof_uint32, :uint32, 43
      optional :oneof_uint64, :uint64, 44
      optional :oneof_bool, :bool, 45
      optional :oneof_double, :double, 46
      optional :oneof_float, :float, 47
      optional :oneof_string, :string, 48
      optional :oneof_bytes, :string, 49
      optional :oneof_enum, :enum, 50, "A.B.C.TestEnum"
      optional :oneof_msg, :message, 51, "A.B.C.TestMessage"
    end
  end
  add_message "A.B.C.TestMessage.NestedMessage" do
    optional :foo, :int32, 1
  end
  add_enum "A.B.C.TestEnum" do
    value :Default, 0
    value :A, 1
    value :B, 2
    value :C, 3
  end
end

module A
  module B
    module C
      TestMessage = Google::Protobuf::DescriptorPool.generated_pool.lookup("A.B.C.TestMessage").msgclass
      TestMessage::NestedMessage = Google::Protobuf::DescriptorPool.generated_pool.lookup("A.B.C.TestMessage.NestedMessage").msgclass
      TestEnum = Google::Protobuf::DescriptorPool.generated_pool.lookup("A.B.C.TestEnum").enummodule
    end
  end
end
