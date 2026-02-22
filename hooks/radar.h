#pragma once
#define _USE_MATH_DEFINES
#include "pch.h"

class Base_Radar
{
public:
	void SetSize(const float& Size);
	void SetPos(const Vector2& Pos);
	void SetRange(const float& Range);
	void SetCrossColor(const ImColor& Color);
	void SetProportion(const float& Proportion);
	void SetDrawList(ImDrawList* DrawList);
	float GetSize();
	Vector2 GetPos();
	
	void AddPoint(const Vector3& LocalPos,const float& LocalYaw, const Vector3& Pos, ImColor Color, int Type = 0, float Yaw = 0.0f);

	void Render();
public:
	ImDrawList* DrawList = nullptr;
	
	bool ShowCrossLine = true;
	
	ImColor CrossColor = ImColor(255, 255, 255, 255);
	
	float Proportion = 2680;
	
	float CircleSize = 4;
	
	float ArrowSize = 11;
	
	float ArcArrowSize = 7;
	
	float RenderRange = 250;
	
	float LocalYaw = 0.0f;
	
	bool ShowRadar = true;
	
	int  PointType = 0;
private:
	Vector2  Pos{ 0,0 };
	float Width = 200;
	std::vector<std::tuple<Vector2, ImColor, int, float>> Points;
};