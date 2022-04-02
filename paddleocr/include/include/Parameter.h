// Copyright (c) 2021 raoyutian Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#pragma pack(push,1)
#include <vector>
using namespace std;
struct OCRParameter
{
	//ͨ�ò���
	bool use_gpu;
	int gpu_id;
	int gpu_mem;
	int numThread;
	bool enable_mkldnn;
	
	//���ģ�����
	int    Padding;
	int    MaxSideLen;
	float  BoxScoreThresh;
	float   BoxThresh;
	float   UnClipRatio;
	bool use_polygon_score;
	bool visualize;
	bool    DoAngle;
	bool   MostAngle;
	
	//������������
	bool use_angle_cls;
	float   cls_thresh;

	OCRParameter()
	{
		//ͨ�ò���
		use_gpu = false;
		gpu_id = 0;
		gpu_mem = 4000;
		numThread = 2;
		enable_mkldnn = true;
		
		//���ģ�����
		Padding = 50;
		MaxSideLen = 2048;
		BoxThresh = 0.3f;
		BoxScoreThresh = 0.618f;
		UnClipRatio = 1.6f;
		use_polygon_score = false;
		visualize = false;

		DoAngle = true;
		MostAngle = true;

		//������������
		use_angle_cls = false;
		cls_thresh = 0.9f;
	}
};

struct Textblock {
	
	std::wstring textblock;
	std::vector<std::vector<int>> box;
	float score;
	Textblock(wstring textblock, std::vector<std::vector<int>> box,float score) {
		this->textblock = textblock;
		this->box = box;
		this->score = score;
	}
};

//textblock�ı��ĸ��ǵĵ�
struct _OCRTextPoint {
	int x;
	int y;
	_OCRTextPoint() :x(0), y(0) {
	}
};

struct _OCRText {
	//textblock�ı�
	int textLen;
	char* ptext;
	//һ��textblock�ĸ���
	_OCRTextPoint points[4];
	//�÷�
	float score;
	_OCRText() {
		textLen = 0;
		ptext = nullptr;
		score = 0.0f;
	}
};

typedef struct _OCRResult {
	//textblock�ı�����
	int textCount;
	_OCRText* pOCRText;
}OCRResult, * LpOCRResult;
#pragma pack(pop) 
