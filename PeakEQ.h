#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <JuceHeader.h>

class PeakEQ {
	float fs, g, fc, fb, V0;
	float d, P;
	float cBoost, cCut, c;
	float x0, x1, xI, yI;
	int channelN;
public:
	PeakEQ() {
		fs = g = fc = fb = V0 = 0;
		d = P = cBoost = cCut = c = 0;
		x0 = x1 = xI = yI = 0;
		channelN = 0;
	}
	PeakEQ(float fs, float g, float fc, float fb, int channelN) : fs(fs), g(g), fc(fc), fb(fb), channelN(channelN){
		d = -cos(fc / fs);
		V0 = pow(10, g / 20);
		P = V0 - 1;
		float coEff =  std::tanf(M_PI * fb / fs);
		cBoost = (coEff - 1) / (coEff + 1);
		cCut = (coEff - V0) / (coEff + V0);
		c = g > 0 ? cBoost : cCut;
		x0 = x1 = yI = xI = 0;
	}

	void setGain(float g) { 
		this->g = g; 
		c = g > 0 ? cBoost : cCut;
	}

	void transferFunction(float* xn) {
		/*for (int chIdx = 0; chIdx < channelN; ++chIdx) {
			for (int i = 0; i < in.getNumSamples(); ++i) {
				xI = in.getSample(chIdx, i) - d*(1-c)*x0 + c*x1;
				y1 = -c * xI + d * (1 - c) * x0 + x1;
				x0 = xI;
				x1 = x0;
				out.setSample(chIdx,i, 0.5 * P * (in.getSample(chIdx, i) - y1) + in.getSample(chIdx, i));
			}
		}

		return out;*/

		xI = *xn - d * (1 - c) * x0 + c * x1;
		yI = -c * xI + d * (1 - c) * x0 + x1;
		x0 = xI;
		x1 = x0;
		*xn *= 0.5 * P * (*xn - yI) + *xn;
	}
};