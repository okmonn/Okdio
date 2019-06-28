#include "Okdio.h"
#include "XAudio2/XAudio2.h"
#include "Callback/VoiceCallback.h"
#include "Loader/Loader.h"
#include <ks.h>
#include <ksmedia.h>

// スピーカー設定用配列
const unsigned long spk[] = {
	KSAUDIO_SPEAKER_MONO,
	KSAUDIO_SPEAKER_STEREO,
	KSAUDIO_SPEAKER_STEREO | SPEAKER_LOW_FREQUENCY,
	KSAUDIO_SPEAKER_QUAD,
	0,
	KSAUDIO_SPEAKER_5POINT1,
	0,
	KSAUDIO_SPEAKER_7POINT1_SURROUND
};

// コンストラクタ
Okdio::Okdio()
{
	Init();
}

// コンストラクタ
Okdio::Okdio(const std::string& fileName, MixVoice* mix, const size_t& num)
{
	Init();
	Load(fileName, mix, num);
}

// コンストラクタ
Okdio::Okdio(const snd::Info& info, const std::vector<float>& data)
{
	Init();
	CustomWave(info, data);
}

// デストラクタ
Okdio::~Okdio()
{
	Stop();
	voice->DestroyVoice();
}

// 初期化
void Okdio::Init(void)
{
	back  = std::make_unique<VoiceCallback>(this);
	read  = 0;
	loop  = end = false;
}

// ソースボイス生成
void Okdio::CreateVoice(void)
{
	WAVEFORMATEXTENSIBLE desc{};
	desc.Format.cbSize          = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
	desc.Format.nChannels       = Loader::Get().Info(name).channel;
	desc.Format.nSamplesPerSec  = Loader::Get().Info(name).sample;
	desc.Format.nBlockAlign     = sizeof(float) * desc.Format.nChannels;
	desc.Format.wBitsPerSample  = sizeof(float) * 8;
	desc.Format.nAvgBytesPerSec = desc.Format.nSamplesPerSec * desc.Format.nBlockAlign;
	desc.Format.wFormatTag      = WAVE_FORMAT_EXTENSIBLE;

	desc.dwChannelMask = spk[desc.Format.nChannels - 1];
	desc.Samples.wValidBitsPerSample = desc.Format.wBitsPerSample;
	desc.SubFormat = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;

	auto hr = XAudio2::Get().Audio()->CreateSourceVoice(&voice, (WAVEFORMATEX*)(&desc), XAUDIO2_VOICE_USEFILTER, XAUDIO2_DEFAULT_FREQ_RATIO, &(*back));
	_ASSERT(hr == S_OK);
}

// 読み込み
void Okdio::Load(const std::string& fileName, MixVoice* mix, const size_t& num)
{
	auto hr = Loader::Get().Load(fileName);
	_ASSERT(hr == 0);

	name = fileName;

	CreateVoice();
	SetMixVoice(mix, num);
}

// カスタム波形登録
void Okdio::CustomWave(const snd::Info& info, const std::vector<float>& data)
{
	name = Loader::Get().Custom(info, data);
	CreateVoice();
}

// 再生
void Okdio::Play(const bool& loop)
{
	auto hr = voice->Start();
	_ASSERT(hr == S_OK);

	this->loop = loop;
	end        = false;
}

// 停止
void Okdio::Stop(void)
{
	auto hr = voice->Stop(XAUDIO2_PLAY_TAILS);
	_ASSERT(hr == S_OK);
}

// 波形データをボイスバッファに追加
void Okdio::Submit(void)
{
	XAUDIO2_VOICE_STATE st{};
	voice->GetState(&st, XAUDIO2_VOICE_NOSAMPLESPLAYED);
	if (st.BuffersQueued != 0)
	{
		return;
	}

	Loader& loader = Loader::Get();

	//波形サイズ
	size_t size = (read + loader.Size(name) > loader.DataSize(name))
		? (read + loader.Size(name)) - loader.DataSize(name)
		: loader.Size(name);

	XAUDIO2_BUFFER buf{};
	buf.AudioBytes = unsigned int(sizeof(float) * size);
	buf.pAudioData = (unsigned char*)&loader.Data(name).lock()->at(read);

	auto hr = voice->SubmitSourceBuffer(&buf);
	_ASSERT(hr == S_OK);

	read += size;
}

// 終了確認
void Okdio::CheckEnd(void)
{
	if (read < Loader::Get().DataSize(name))
	{
		return;
	}

	if (loop == false)
	{
		Stop();
		end = true;
	}

	read = 0;
}

// ミックスボイスセット
void Okdio::SetMixVoice(MixVoice* mix, const size_t& num)
{
	if (mix == nullptr)
	{
		return;
	}

	std::vector<XAUDIO2_SEND_DESCRIPTOR>desc(num);
	for (size_t i = 0; i < num; ++i)
	{
		desc[i] = mix[i].GetSend();
	}

	XAUDIO2_VOICE_SENDS send{ unsigned __int32(num), desc.data() };

	auto hr = voice->SetOutputVoices(&send);
	_ASSERT(hr == S_OK);
}

// 再生速度調節
bool Okdio::SetSpeed(const float& rate)
{
	if (!(rate > XAUDIO2_MIN_FREQ_RATIO && rate <= XAUDIO2_DEFAULT_FREQ_RATIO))
	{
		return false;
	}

	auto hr = voice->SetFrequencyRatio(rate);
	_ASSERT(hr == S_OK);

	return true;
}

// 登録名取得
std::string Okdio::Name(void) const
{
	return name;
}

// 読み込み位置取得
size_t& Okdio::Read(void)
{
	return read;
}

// 代入演算子オーバーロード
void Okdio::operator=(const Okdio& okdio)
{
	if (okdio.voice == nullptr)
	{
		return;
	}

	Init();
	name = okdio.name;
	CreateVoice();
}
