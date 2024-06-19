#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofSetColor(255);
	ofSetLineWidth(1.5);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	this->font_size = 30;
	ofTrueTypeFontSettings font_settings("fonts/msgothic.ttc", this->font_size);
	font_settings.antialiased = true;
	font_settings.addRanges(ofAlphabet::Japanese);
	this->font.load(font_settings);

	this->words = {

		//u8"!", u8"#", u8"$", u8"%", u8"&",
		//u8"'", u8"(", u8")", u8"*", u8"+",
		//u8",", u8"-", u8".", u8"/", u8"0",
		//u8"1", u8"2", u8"3", u8"4", u8"5",
		//u8"6", u8"7", u8"8", u8"9", u8"0",
		//u8":", u8";", u8"<", u8"=", u8">",
		//u8"?", u8"@",

		//u8"A", u8"B", u8"C", u8"D", u8"E",
		//u8"F", u8"G", u8"H", u8"I", u8"J",
		//u8"K", u8"L", u8"M", u8"N", u8"O",
		//u8"P", u8"Q", u8"R", u8"S", u8"T",
		//u8"U", u8"V", u8"W", u8"X", u8"Y", u8"Z",

		//u8"a", u8"b", u8"c", u8"d", u8"e",
		//u8"f", u8"g", u8"h", u8"i", u8"j",
		//u8"k", u8"l", u8"m", u8"n", u8"o",
		//u8"p", u8"q", u8"r", u8"s", u8"t",
		//u8"u", u8"v", u8"w", u8"x", u8"y", u8"z",

		u8"±", u8"²", u8"³", u8"´", u8"µ",
		u8"¶", u8"·", u8"¸", u8"¹", u8"º",
		u8"»", u8"¼", u8"½", u8"¾", u8"¿",
		u8"À", u8"Á", u8"Â", u8"Ã", u8"Ä",
		u8"Å", u8"Æ", u8"Ç", u8"È", u8"É",
		u8"Ê", u8"Ë", u8"Ì", u8"Í", u8"Î",
		u8"Ï", u8"Ð", u8"Ñ", u8"Ò", u8"Ó",
		u8"Ô", u8"Õ", u8"Ö",
		u8"×", u8"Ø", u8"Ù", u8"Ú", u8"Û",
		u8"Ü", u8"¦", u8"Ý",
	};
}

//--------------------------------------------------------------
void ofApp::update() {

	int index = 0;
	auto radius = ofMap(sin(ofGetFrameNum() * 0.02), -1, 1, -300, 300) + 1;
	for (int i = 0; i < 2; i++) {

		auto deg = ofGetFrameNum() * 3 + i * 180;
		auto next_deg = deg + 1;

		auto location = glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
		auto next = glm::vec2(radius * cos(next_deg * DEG_TO_RAD), radius * sin(next_deg * DEG_TO_RAD));

		auto distance = location - next;
		distance *= 1;

		for (int k = 0; k < 2; k++) {

			auto future = location + distance * 15;
			auto random_deg = ofRandom(360);
			future += glm::vec2(30 * cos(random_deg * DEG_TO_RAD), 30 * sin(random_deg * DEG_TO_RAD));
			auto future_distance = future - location;

			this->location_list.push_back(location);
			this->velocity_list.push_back(glm::normalize(future_distance) * glm::length(distance));
			this->word_index_list.push_back(ofRandom(this->words.size()));
		}

		index++;
	}

	for (int i = this->location_list.size() - 1; i > -1; i--) {

		this->location_list[i] += this->velocity_list[i];
		this->velocity_list[i] *= 1.1;

		if (glm::distance(glm::vec2(), this->location_list[i]) > 720) {

			this->location_list.erase(this->location_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->word_index_list.erase(this->word_index_list.begin() + i);
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);

	int index = 0;
	for (auto& location : this->location_list) {

		ofPushMatrix();
		ofTranslate(location);

		auto angle = std::atan2(location.y - (location.y + this->velocity_list[index].y),
			location.x - (location.x + this->velocity_list[index].x));
		ofRotate(angle * RAD_TO_DEG + 90);

		this->font.drawString(this->words[this->word_index_list[index]], 0, 0);

		ofPopMatrix();

		index++;
	}

	/*
	int start = 150;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}