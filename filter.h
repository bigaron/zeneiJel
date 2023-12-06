#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <cmath>
#include <JuceHeader.h>

class Filter {
	float fs, fb, fc, V0, H0, g, c, d;
	float x1[2], x2[2], y1[2], y2[2];
	float prevVal;
public:
	Filter() {}
	Filter(float sample, float band, float cut, float gain): fs(sample), fb(band), fc(cut), g(gain) {
		d = -cos(2 * M_PI * fc / fs);
		setGain(g);
		resetCoeffs();
		prevVal = 0;
	}

	void resetCoeffs() {
		for (int i = 0; i < 2; ++i) {
			x1[i] = x2[i] = y1[i] = y2[i] = 0.f;
		}
	}

	void setGain(float g) {
		this->g = g;
		V0 = powf(10.0f, g / 20.0f);
		H0 = V0 - 1;
		if (g >= 0) c = (tanf(M_PI * fb / fs) - 1) / (tanf(M_PI * fb / fs) + 1);
		else c = (tanf(M_PI * fb / fs) - V0) / (tanf(M_PI * fb / fs) + V0);
	}

	float getGain() { return g; }

	float A2(float xn, int i) {
		float ret = -d * (1 - c) * y1[i] + c * y2[i] + (-c) * xn + d * (1 - c) * x1[i] + x2[i];
		x2[i] = x1[i];
		x1[i] = xn;
		y2[i] = y1[i];
		y1[i] = ret;
		return ret;
	}

	void lowPassBuffer(juce::AudioBuffer<float>& buffer) {
		float cutOff = fs / 2;
		const auto ta = std::tan(M_PI * cutOff / fs);
		const auto a1 = (ta - 1.f) / (ta + 1.f);

		for (int i = 0; i < buffer.getNumChannels(); ++i) {
			auto samples = buffer.getWritePointer(i);

			for (int n = 0; n < buffer.getNumSamples(); ++n) {
				const auto inputSample = samples[n];
				const auto allpassFilteredSample = a1 * inputSample + prevVal;
				prevVal = inputSample - a1 * allpassFilteredSample;

				const auto filterOut = 0.5f * (inputSample + allpassFilteredSample);
				samples[n] = filterOut;
			}
		}
	}

	void processBuffer(juce::AudioBuffer<float>& buffer) {
		for (int i = 0; i < buffer.getNumChannels(); ++i) {
			auto* samples = buffer.getWritePointer(i);

			for (int n = 0; n < buffer.getNumSamples(); ++n) {
				float val = samples[n];
				float yn;

				yn = val + H0 * 0.5f * (val - A2(val, i));
				samples[n] = yn;
			}
		}
	}
};