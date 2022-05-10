#pragma once
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <fstream>
#include<wrl.h>

using namespace Microsoft::WRL;



class Audio
{
private:
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	IXAudio2SourceVoice* pSourceVoice;

public:
	Audio();
	~Audio();

	struct ChunkHeader
	{
		char id[4]; // チャンク毎のID
		int32_t size;  // チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader
	{
		ChunkHeader chunk;   // "RIFF"
		char type[4]; // "WAVE"
	};

	// FMTチャンク
	struct FormatChunk
	{
		ChunkHeader chunk; // "fmt "
		WAVEFORMATEX fmt; // 波形フォーマット
	};
	struct SoundData
	{
		WAVEFORMATEX wfex;
		BYTE* pBuffer;
		unsigned int bufferSize;
	};
private:
	FormatChunk format = {};
	ChunkHeader data;
	char* pBuffer;
	SoundData SSoundData;
public:

	void soundData();
	bool Initialize();
	void SoundLoadWave(const char* filename);
	void SoundPlayWave();
	void stopSound();
	void ReStart();
	void SoundUnload();
	void Reset();
	//SE用
	void SEPlayWave();
	//void SEResetWave();





};

