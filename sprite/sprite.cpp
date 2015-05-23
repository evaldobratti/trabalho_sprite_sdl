#include <stdio.h>
#include <vector>
using namespace std;

class Frame {
public:
	int getX(){
		return this->x;
	}
	int getY() {
		return this->y;
	}
	int getW() {
		return this->w;
	}
	int getH() {
		return this->h;
	}
	Frame(int x, int y, int w, int h){
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
private:
	int x, y, w, h;
};


class FrameSet {
public:
	FrameSet() {
		i = 0;
		nextFrameSet = NULL;
	}
	FrameSet(FrameSet *nextFrameSet) {
		vector<Frame> temp;
		this->frames = temp;
		this->nextFrameSet = nextFrameSet;
	}
	vector<Frame> getFrames(){
		return this->frames;
	}
	void addFrame(Frame frame) {
		this->frames.push_back(frame);
	}
	Frame getFrameAtual() {
		i += 1;
		if (i >= this->frames.size())
			i = 0;

		return frames.at(i);
	}
	void setNextFrameSet(FrameSet* nextFrameSet) {
		this->nextFrameSet = nextFrameSet;
	}
	FrameSet& getNextFrameSet() {
		if (nextFrameSet != NULL){
			return *nextFrameSet;
		}
		return *this;
	}
	void reset() {
		i = 0;
	}
private:
	int i;
	vector<Frame> frames;
	FrameSet *nextFrameSet;
	Frame *frameAtual;

	bool isUltimoFrame(int i) {
		return i >= frames.size();
	}
};

