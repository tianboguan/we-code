// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/unittest_import_public_proto3.proto
#pragma warning disable 1591, 0612, 3021
#region Designer generated code

using pb = global::Google.Protobuf;
using pbc = global::Google.Protobuf.Collections;
using pbr = global::Google.Protobuf.Reflection;
using scg = global::System.Collections.Generic;
namespace Google.Protobuf.TestProtos {

  [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
  public static partial class UnittestImportPublicProto3 {

    #region Descriptor
    public static pbr::FileDescriptor Descriptor {
      get { return descriptor; }
    }
    private static pbr::FileDescriptor descriptor;

    static UnittestImportPublicProto3() {
      byte[] descriptorData = global::System.Convert.FromBase64String(
          string.Concat(
            "CjNnb29nbGUvcHJvdG9idWYvdW5pdHRlc3RfaW1wb3J0X3B1YmxpY19wcm90", 
            "bzMucHJvdG8SGHByb3RvYnVmX3VuaXR0ZXN0X2ltcG9ydCIgChNQdWJsaWNJ", 
            "bXBvcnRNZXNzYWdlEgkKAWUYASABKAVCNwoYY29tLmdvb2dsZS5wcm90b2J1", 
            "Zi50ZXN0qgIaR29vZ2xlLlByb3RvYnVmLlRlc3RQcm90b3NiBnByb3RvMw=="));
      descriptor = pbr::FileDescriptor.InternalBuildGeneratedFileFrom(descriptorData,
          new pbr::FileDescriptor[] { },
          new pbr::GeneratedCodeInfo(null, new pbr::GeneratedCodeInfo[] {
            new pbr::GeneratedCodeInfo(typeof(global::Google.Protobuf.TestProtos.PublicImportMessage), new[]{ "E" }, null, null, null)
          }));
    }
    #endregion

  }
  #region Messages
  [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
  public sealed partial class PublicImportMessage : pb::IMessage<PublicImportMessage> {
    private static readonly pb::MessageParser<PublicImportMessage> _parser = new pb::MessageParser<PublicImportMessage>(() => new PublicImportMessage());
    public static pb::MessageParser<PublicImportMessage> Parser { get { return _parser; } }

    public static pbr::MessageDescriptor Descriptor {
      get { return global::Google.Protobuf.TestProtos.UnittestImportPublicProto3.Descriptor.MessageTypes[0]; }
    }

    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    public PublicImportMessage() {
      OnConstruction();
    }

    partial void OnConstruction();

    public PublicImportMessage(PublicImportMessage other) : this() {
      e_ = other.e_;
    }

    public PublicImportMessage Clone() {
      return new PublicImportMessage(this);
    }

    public const int EFieldNumber = 1;
    private int e_;
    public int E {
      get { return e_; }
      set {
        e_ = value;
      }
    }

    public override bool Equals(object other) {
      return Equals(other as PublicImportMessage);
    }

    public bool Equals(PublicImportMessage other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (E != other.E) return false;
      return true;
    }

    public override int GetHashCode() {
      int hash = 1;
      if (E != 0) hash ^= E.GetHashCode();
      return hash;
    }

    public override string ToString() {
      return pb::JsonFormatter.Default.Format(this);
    }

    public void WriteTo(pb::CodedOutputStream output) {
      if (E != 0) {
        output.WriteRawTag(8);
        output.WriteInt32(E);
      }
    }

    public int CalculateSize() {
      int size = 0;
      if (E != 0) {
        size += 1 + pb::CodedOutputStream.ComputeInt32Size(E);
      }
      return size;
    }

    public void MergeFrom(PublicImportMessage other) {
      if (other == null) {
        return;
      }
      if (other.E != 0) {
        E = other.E;
      }
    }

    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            input.SkipLastField();
            break;
          case 8: {
            E = input.ReadInt32();
            break;
          }
        }
      }
    }

  }

  #endregion

}

#endregion Designer generated code
