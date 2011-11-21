#ifndef _STRING_TO_POINTS_
#define _STRING_TO_POINTS_

#include "ofMain.h"

struct messageData{
	vector<ofPoint> points;
	string text;
	ofRectangle boundingbox;
};

class stringToPoinsWrapper{
public:
	stringToPoinsWrapper(){	}
	void setup(string fontStr, int fontSize){
		font.loadFont(fontStr, fontSize, true, true, true);
		font.setLineHeight(34.0f);
		font.setLetterSpacing(1.035);
	}

	messageData stringToPoints(string & input){
		messageData data;
		vector<ofTTFCharacter> paths =   font.getStringAsPoints(text);

		data.text = input;
		data.boundingbox = font.getStringBoundingBox(text,0,0);
		data.points.reserve(3000);
		for(int pi=0;pi<paths.size();++pi){
			vector<ofPolyline> & lines = paths[pi].getOutline();
			for (int li = 0; li < lines.size(); ++li) {
				vector<ofPoint> & line = lines[li].getVertices();

				//TODO lückenfüllen - siehe Q3
				data.points.insert(data.points.end(),line.begin(),line.end());
			}

			//alternative:
	//		ofMesh & mesh = paths[pi].getTessellation();
	//		vector<ofPoint> & line = mesh.getVertices();
	//		stringPoints.insert(stringPoints.end(),line.begin(),line.end());
		}

		return data;
	}
protected:
	ofTrueTypeFont font;
};

#endif
