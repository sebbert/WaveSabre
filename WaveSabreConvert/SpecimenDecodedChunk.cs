using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Runtime.Remoting.Messaging;

namespace WaveSabreConvert
{
    public struct SpecimenChunkHeader
    {
        public int CompressedSize;
        public int UncompressedSize;

        public void Read(BinaryReader reader)
        {
            CompressedSize = reader.ReadInt32();
            UncompressedSize = reader.ReadInt32();
        }

        public void Write(BinaryWriter writer)
        {
            writer.Write(CompressedSize);
            writer.Write(UncompressedSize);
        }
    }

    public struct SpecimenSampleFormat
    {
        public ushort Tag;
        public ushort Version;
        public int Size;
        public SpecimenSampleFormatType Type;

        public bool Read(BinaryReader reader)
        {
            Tag = reader.ReadUInt16();
            if (Tag != 0)
            {
                return false;
            }

            Version = reader.ReadUInt16();
            Size = reader.ReadInt32();
            Type = (SpecimenSampleFormatType)reader.ReadUInt32();

            return true;
        }

        public void Write(BinaryWriter writer)
        {
            writer.Write(Tag);
            writer.Write(Version);
            writer.Write(Size);
            writer.Write((uint)Type);
        }
    }

    public enum SpecimenSampleFormatType : uint
    {
        Blob = 0,
    }

    public class SpecimenDecodedChunk
    {
        public SpecimenChunkHeader Header;
        public SpecimenSampleFormat SampleFormat;

        public byte[] CompressedData;
        public float[] Params;

        public int ReadChunkSize;

        public bool SetChunk(byte[] chunk, ILog logger)
        {
            using (var stream = new MemoryStream(chunk))
            using (var reader = new BinaryReader(stream))
            {
                Header.Read(reader);
                if (!SampleFormat.Read(reader))
                {
                    return false;
                }

                CompressedData = reader.ReadBytes(Header.CompressedSize);

                var remainingSize = chunk.Length - stream.Position;
                var chunkSizeSize = sizeof(int);
                var paramsSize = remainingSize - chunkSizeSize;
                var numParams = paramsSize / sizeof(float);

                Params = new float[numParams];
                for (var i = 0; i < numParams; ++i)
                {
                    Params[i] = reader.ReadSingle();
                }

                var chunkSize = reader.ReadInt32();
                if (chunkSize != chunk.Length)
                {
                    throw new Exception("Chunk size footer does not match actual chunk size");
                }

                if (stream.Position != chunk.Length)
                {
                    throw new Exception("Did not read entire chunk");
                }

                ReadChunkSize = chunkSize;
            }

            return true;
        }

        public byte[] GetChunk(ILog logger)
        {

            using (var stream = new MemoryStream())
            using (var writer = new BinaryWriter(stream))
            {
                Header.Write(writer);
                SampleFormat.Write(writer);

                writer.Write(CompressedData);

                for (var i = 0; i < Params.Length; ++i)
                {
                    writer.Write(Params[i]);
                }

                var chunkSize = stream.Position + sizeof(int);
                writer.Write((int)chunkSize);

                return stream.ToArray();
            }
        }

        public void SetCompressedData(byte[] compressedData)
        {
            CompressedData = compressedData;
            Header.CompressedSize = compressedData.Length;
        }
    }
}
