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
		V0 = pow(10, g / 20.0f);
		H0 = V0 - 1;
		setGain(g);
		cBoost = (tan(M_PI * fb / fs) - 1) / (tan(M_PI * fb/ fs) + 1);
		cCut = (V0 - tan(M_PI * fb/fs)) / (tan(M_PI * fb/fs) + V0);
		d = -cos(2*M_PI*fc / fs);
		xPrev[0] = xPrev[1] = yPrev[0] = yPrev[1] = 0;
	}

	void setGain(float g) { 
		this->g = g; 
		c = g >= 0 ? cBoost : cCut;
	}

	float A2(float xn) {
		float yn = c * yPrev[1] - d * (1 - c) * yPrev[0] - c * xn + d * (1 - c) * xPrev[0] + xPrev[1];
		xPrev[1] = xPrev[0];
		xPrev[0] = xn;
		yPrev[1] = yPrev[0];
		yPrev[0] = yn;
		return yn;
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

	float lpFilter(float xn) {
		c = (tan(M_PI * fc / fs) - 1) / (tan(M_PI * fc / fs) + 1);
		float xh = xn - c * xPrev[0];
		float ret = c * xh + xPrev[0];
		xPrev[0] = xh;
		return 0.5 * (xn - ret);
	}

	void process(juce::AudioBuffer<float>& buffer) {
		float xh[] = {0.f, 0.f}, xh_new;
		float y1;
		int kilo = 10000;
		//xPrev[0] = xPrev[1] = yPrev[0] = yPrev[1] = 0;
		for (auto ch = 0; ch < buffer.getNumChannels(); ++ch) {
			auto chSample = buffer.getWritePointer(ch);

			for (auto sample = 0; sample < buffer.getNumSamples(); ++sample) {
				/*const auto value = chSample[sample];
				xh_new = value - d * (1 - c) * xPrev[0] + c*xPrev[1];
				y1 = -c * xh_new + d * (1 - c) * xPrev[0] + xPrev[1];
				auto out = value + H0 / 2 * (value - y1);
				xPrev[1] = xPrev[0];
				xPrev[0] = xh_new;
				chSample[sample] = out;*/

				const auto value = chSample[sample] ;
				auto out = value + H0 / 2 * (value - A2(value));
				chSample[sample] = out ; 
				//chSample[sample] = lpFilter(chSample[sample]);
			}
		}


	}
};