
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <fstream>
#include<wrl.h>
#include"Audio.h"
#include <cassert>
using namespace Microsoft::WRL;
void Audio::soundData()
{
	struct SoundData
	{
		WAVEFORMATEX wfex;
		BYTE* pBuffer;
		unsigned int bufferSize;
	};
	return;
}
struct SoundData
{
	WAVEFORMATEX wfex;
	BYTE* pBuffer;
	unsigned int bufferSize;
};

bool Audio::Initialize()
{
	HRESULT result;

	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if FAILED(result) {
		assert(0);
		return false;
	}

	// マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	if FAILED(result) {
		assert(0);
		return false;
	}

	return false;
}
void Audio::SoundLoadWave(const char* filename)
{
	// ファイル入力ストリームのインスタンス
	std::ifstream file;
	// .wavファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);
	// ファイルオープン失敗を検出する
	assert(file.is_open());

	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		//assert(0);
	}
	// タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		//assert(0);
	}

	// Formatチャンクの読み込み

	// チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		//assert(0);
	}
	// チャンク本体の読み込み

	file.read((char*)&format.fmt, format.chunk.size);

	// Dataチャンクの読み込み
	//ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	// JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK ", 4) == 0) {
		// 読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data ", 4) != 0) {
		//assert(0);
	}

	// Dataチャンクのデータ部（波形データ）の読み込み
	pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// Waveファイルを閉じる
	file.close();

	// returnする為の音声データ


	SSoundData.wfex = format.fmt;
	SSoundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	SSoundData.bufferSize = data.size;


}

void Audio::SoundPlayWave() {

	HRESULT result;

	// 波形フォーマットを元にSourceVoiceの生成

	result = xAudio2->CreateSourceVoice(&pSourceVoice, &SSoundData.wfex);
	assert(SUCCEEDED(result));

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = SSoundData.pBuffer;
	buf.AudioBytes = SSoundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.LoopCount = XAUDIO2_LOOP_INFINITE;

	// 波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	if FAILED(result) {
		delete[] pBuffer;
		assert(0);
		return;
	}

	result = pSourceVoice->Start();
	if FAILED(result) {
		delete[] pBuffer;
		assert(0);
		return;
	}
}

void Audio::stopSound()
{
	HRESULT result;

	result = pSourceVoice->Stop();


}
void Audio::ReStart()
{
	HRESULT result;

	result = pSourceVoice->Start();

}
Audio::Audio()
{

}

Audio::~Audio()
{
}

void Audio::SEPlayWave()
{
	HRESULT result;

	// 波形フォーマットを元にSourceVoiceの生成

	result = xAudio2->CreateSourceVoice(&pSourceVoice, &SSoundData.wfex);
	assert(SUCCEEDED(result));

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = SSoundData.pBuffer;
	buf.AudioBytes = SSoundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	result = pSourceVoice->Stop();
	result = pSourceVoice->FlushSourceBuffers();

	// 波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	if FAILED(result) {
		delete[] pBuffer;
		assert(0);
		return;
	}

	result = pSourceVoice->Start();
	if FAILED(result) {
		delete[] pBuffer;
		assert(0);
		return;
	}



}

void Audio::Reset()
{
	HRESULT result;
	result = pSourceVoice->FlushSourceBuffers();

}
//void Audio::SEResetWave()
//{
//	HRESULT result;
//
//	
//}



void Audio::SoundUnload()
{
	// バッファのメモリを解放
	delete[] SSoundData.pBuffer;

	SSoundData.pBuffer = 0;
	SSoundData.bufferSize = 0;
	SSoundData.wfex = {};
}

//////////////////////////////////////////
/*
gameScene.cppに書いてね
//////////////////////////
			//	各エンド処理の中のタイトルに戻るためのif文の中にいれる
		for (int i = 0; i < 11; i++)
		{
			AUDIO[i]->Reset();
			AUDIOFlag[i] = 0;
		}
//////////////////////////


*/