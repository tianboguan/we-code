#!/bin/bash
# Generates C# source files from .proto files.
# You first need to make sure protoc has been built (see instructions on
# building protoc in root of this repository)

# This script performs a few fix-ups as part of generation. These are:
# - descriptor.proto is renamed to descriptor_proto_file.proto before
#   generation, to avoid the naming collision between the class for the file
#   descriptor and its Descriptor property
# - This change also impacts UnittestCustomOptions, which expects to
#   use a class of Descriptor when it's actually been renamed to
#   DescriptorProtoFile.
# - Issue 307 (codegen for double-nested types) breaks Unittest.proto and
#   its lite equivalents.

set -ex

# cd to repository root
cd $(dirname $0)/..

# Protocol buffer compiler to use. If the PROTOC variable is set,
# use that. Otherwise, probe for expected locations under both
# Windows and Unix.
if [ -z "$PROTOC" ]; then
  # TODO(jonskeet): Use an array and a for loop instead?
  if [ -x cmake/build/Debug/protoc.exe ]; then
    PROTOC=cmake/build/Debug/protoc.exe
  elif [ -x cmake/build/Release/protoc.exe ]; then
    PROTOC=cmake/build/Release/protoc.exe
  elif [ -x src/protoc ]; then
    PROTOC=src/protoc
  else
    echo "Unable to find protocol buffer compiler."
    exit 1
  fi
fi

# Descriptor proto
$PROTOC -Isrc --csharp_out=csharp/src/Google.Protobuf/Reflection \
    src/google/protobuf/descriptor.proto

$PROTOC -Isrc --csharp_out=csharp/src/Google.Protobuf/WellKnownTypes \
    src/google/protobuf/any.proto \
    src/google/protobuf/api.proto \
    src/google/protobuf/duration.proto \
    src/google/protobuf/empty.proto \
    src/google/protobuf/field_mask.proto \
    src/google/protobuf/source_context.proto \
    src/google/protobuf/struct.proto \
    src/google/protobuf/timestamp.proto \
    src/google/protobuf/type.proto \
    src/google/protobuf/wrappers.proto

$PROTOC -Isrc --csharp_out=csharp/src/Google.Protobuf.Test/TestProtos \
    src/google/protobuf/map_unittest_proto3.proto \
    src/google/protobuf/unittest_proto3.proto \
    src/google/protobuf/unittest_import_proto3.proto \
    src/google/protobuf/unittest_import_public_proto3.proto \
    src/google/protobuf/unittest_well_known_types.proto


$PROTOC -Icsharp/protos --csharp_out=csharp/src/Google.Protobuf.Test/TestProtos \
    csharp/protos/unittest_issues.proto

# AddressBook sample protos
$PROTOC -Iexamples --csharp_out=csharp/src/AddressBook \
    examples/addressbook.proto

$PROTOC -Iconformance --csharp_out=csharp/src/Google.Protobuf.Conformance \
    conformance/conformance.proto
