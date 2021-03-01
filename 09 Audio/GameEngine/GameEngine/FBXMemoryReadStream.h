#pragma once

#include <fbxsdk.h>

class FBXMemoryReadStream : public fbxsdk::FbxStream
{
public:
	FBXMemoryReadStream (int size, fbxsdk::FbxManager* fbxManager);
	virtual ~FBXMemoryReadStream ();

	virtual fbxsdk::FbxStream::EState GetState () override;

	virtual bool Open (void* pStreamData) override;
	virtual bool Close () override;
	virtual bool Flush () override;

	virtual int Write (const void* pData, int pSize) override;
	virtual int Read (void* pData, int pSize) const override;
	//virtual char* ReadString (char* pBuffer, int pMaxSize, bool pStopAtFirstWhiteSpace = false) override;

	virtual int GetReaderID () const override;
	virtual int GetWriterID () const override;

	virtual void Seek (const fbxsdk::FbxInt64& pOffset, const fbxsdk::FbxFile::ESeekPos& pSeekPos) override;
	virtual long GetPosition () const override;
	virtual void SetPosition (long pPosition) override;

	virtual int GetError () const override;
	virtual void ClearError () override;

private:
	char* m_streamData;
	int m_streamDataSize;
	int m_streamPosition;

	fbxsdk::FbxStream::EState m_state;
	int m_readerId;
};
