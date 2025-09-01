// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <vector>
#include <random>
#include <math.h>


class PerlinNoise
{
public:
	PerlinNoise();
	
	float perlin(float x, float y);
	float perlin3D(float x, float y, float z);

private:

};
