#pragma once
#include <vector>

namespace snd
{
	// ”CˆÓ‚ÌŒ…‚©‚çØ‚èÌ‚Ä
	float Floor(const float& input, const int& digit);

	// ‰~ü—¦æ“¾
	inline constexpr float PI(void);

	// —£Uƒt[ƒŠƒG•ÏŠ·
	void DFT(const std::vector<float>& data, std::vector<float>& real, std::vector<float>& imag);

	// ‹t—£Uƒt[ƒŠƒG•ÏŠ·
	std::vector<float> IDFT(const std::vector<float>& real, const std::vector<float>& imag);
}
