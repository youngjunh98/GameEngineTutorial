#include <memory>

#include "FBXMemoryReadStream.h"

FBXMemoryReadStream::FBXMemoryReadStream (int size, fbxsdk::FbxManager* fbxManager) :
	m_streamData (nullptr),
	m_streamDataSize (size),
	m_state (EState::eEmpty),
	m_streamPosition (0),
	m_readerId (fbxManager->GetIOPluginRegistry ()->FindReaderIDByExtension ("fbx"))
{
}

FBXMemoryReadStream::~FBXMemoryReadStream ()
{
}

fbxsdk::FbxStream::EState FBXMemoryReadStream::GetState ()
{
	return m_state;
}

bool FBXMemoryReadStream::Open (void* pStreamData)
{
	m_streamData = static_cast<char*> (pStreamData);
	m_streamPosition = 0;
	m_state = EState::eOpen;

	return true;
}

bool FBXMemoryReadStream::Close ()
{
	m_streamData = nullptr;
	m_streamPosition = 0;
	m_state = EState::eClosed;

	return true;
}

bool FBXMemoryReadStream::Flush ()
{
	return true;
}

int FBXMemoryReadStream::Write (const void* pData, int pSize)
{
	return 0;
}

int FBXMemoryReadStream::Read (void* pData, int pSize) const
{
	if (m_streamData == nullptr)
	{
		return 0;
	}

	int maxReadSize = m_streamDataSize - m_streamPosition;
	int readSize = maxReadSize;
	
	if (pSize < maxReadSize)
	{
		readSize = pSize;
	}
	
	std::memcpy (pData, &m_streamData[m_streamPosition], readSize);
	const_cast<FBXMemoryReadStream*> (this)->m_streamPosition += readSize;

	return readSize;
}

int FBXMemoryReadStream::GetReaderID () const
{
	return m_readerId;
}

int FBXMemoryReadStream::GetWriterID () const
{
	return -1;
}

void FBXMemoryReadStream::Seek (const fbxsdk::FbxInt64& pOffset, const fbxsdk::FbxFile::ESeekPos& pSeekPos)
{
	int offset = static_cast<int> (pOffset);

	if (pSeekPos == FbxFile::ESeekPos::eBegin)
	{
		m_streamPosition = offset;
	}
	else if (pSeekPos == FbxFile::ESeekPos::eEnd)
	{
		m_streamPosition = m_streamDataSize - offset;
	}
	else
	{
		m_streamPosition += offset;
	}
}

long FBXMemoryReadStream::GetPosition () const
{
	return static_cast<long> (m_streamPosition);
}

void FBXMemoryReadStream::SetPosition (long pPosition)
{
	m_streamPosition = static_cast<int> (pPosition);
}

int FBXMemoryReadStream::GetError () const
{
	return 0;
}

void FBXMemoryReadStream::ClearError ()
{
	// Empty
}
