#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <cmath>
#include <JuceHeader.h>

class PeakEQ {
	float fs, g, fc, fb, V0;
	float Wc, Wb;
	float d, H0;
	float cBoost, cCut, c;
	float x1, x2, xH, yH;
	float yPrev[3];
	float xPrev[3];
	int channelN;
public:
	PeakEQ() {
		fs = g = fc = fb = V0 = 0;
		d = H0 = cBoost = cCut = c = 0;
		x2 = x1 = xH = yH = 0;
		channelN = 0;
	}
	PeakEQ(float fs, float g, float fc, float fb, int channelN) : fs(fs), g(g), fc(fc), fb(fb), channelN(channelN){
		Wc = 2 * fc / fs;
		Wb = 2 * fb / fs;
		V0 = pow(10, g / 20);
		H0 = V0 - 1;
		setGain(g);
		cBoost = (tan(M_PI * Wb / 2) - 1) / (tan(M_PI * Wb / 2) + 1);
		cCut = (tan(M_PI * Wb / 2) - V0) / (tan(M_PI * Wb / 2) + V0);
		d = -cos(M_PI * Wc);
		x1 = x2 = 0;
	}

	void setGain(float g) { 
		this->g = g; 
		c = g >= 0 ? cBoost : cCut;
	}

	float A2(float xn) {
		float p1 = c * yPrev[1] - d * (1 - c) * yPrev[0];
		float p2 = -c * xn + d * (1 - c) * xPrev[0] + xPrev[1];
		xPrev[1] = xPrev[0];
		xPrev[0] = xn;
		yPrev[1] = yPrev[0];
		yPrev[0] = p1 + p2;

		return p1 + p2;
	}
	void resetXN() {
		x1 = x2 = 0;
	}

	void transferFunction(float* xn) {
		float tmp = *xn;
		float xh_new = tmp - d * (1 - c) * x1 + c * x2;
		float ap_y = -c * xh_new + d * (1 - c) * x1 + x2;
		x2 = x1;
		x1 = xh_new;
		tmp = 0.5 * H0 * (tmp - ap_y) + tmp;
		*xn = tmp;
	}

	void process(juce::AudioBuffer<float>& buffer) {
		float xh[] = {0.f, 0.f}, xh_new;
		float y1;
		for (auto ch = 0; ch < buffer.getNumChannels(); ++ch) {
			auto chSample = buffer.getWritePointer(ch);

			for (auto sample = 0; sample < buffer.getNumSamples(); ++sample) {
				const auto value = chSample[sample];
				xh_new = value - d * (1 - c) * xh[0] + c * xh[1];
				y1 = -c * xh_new + d * (1 - c) * xh[0] + xh[1];
				auto out = value + H0 / 2 * (value - y1);
				xh[1] = xh[0];
				xh[0] = xh_new;
				chSample[sample] = out;
			}
		}


	}
};