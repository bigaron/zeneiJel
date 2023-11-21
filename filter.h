#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <cmath>
#include <JuceHeader.h>

class Filter {
	float fs, fb, fc, V0, H0, g, c, d;
	float x1, x2, y1, y2;
public:
	Filter() {}
	Filter(float sample, float band, float cut, float gain): fs(sample), fb(band), fc(cut), g(gain) {
		d = -cos(2 * M_PI * fc / fs);
		setGain(g);
		x1 = x2 = y1 = y2 = 0.f;
	}

	void setGain(float g) {
		this->g = g;
		if (g >= 0) c = (tanf(M_PI * fb / fs) - 1) / (tanf(M_PI * fb / fs) + 1);
		else c = (tanf(M_PI * fb / fs) - V0) / (tanf(M_PI * fb / fs) + V0);
		x1 = x2 = y1 = y2 = 0.f;
		V0 = powf(10, g / 20.0f);
		H0 = V0 - 1;
	}

	float getGain() { return g; }

	float A2(float xn) {
		float ret = -d * (1 - c) * y1 + c * y2 - c * xn + d * (1 - c) * x1 + x2;
		x2 = x1;
		x1 = xn;
		y2 = y1;
		y1 = ret;
		return ret;
	}

	void processBuffer(juce::AudioBuffer<float>& buffer) {
		for (int i = 0; i < buffer.getNumChannels(); ++i) {
			auto samples = buffer.getWritePointer(i);

			for (int n = 0; n < buffer.getNumSamples(); ++n) {
				float val = samples[n];
				float yn = val + H0 * 0.5 * (val - A2(val));
				samples[n] = yn;
			}
		}
	}
};