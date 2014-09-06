//
//  EffectParticles.h
//  LaserShow
//
//  Created by Seb Lee-Delisle on 16/03/2014.
//
//

#pragma once
#include "ParticleSystemManager.h"
#include "ParticleRendererLaser.h"
#include "ParticleRendererLaserLine.h"
#include "ParticleRendererGlitchLineLaser.h"
#include "PipeOrganData.h"
#include "ColumnData.h"


class EffectParticles {

	public : 
	EffectParticles() : particleSystemManager(*ParticleSystemManager::instance()){
		mode = 0;
	};

	void update();
	
	void draw();

	void makeParticleForPipe(int pipeindex);
	
	void makeRainbowBurst();
	void makeStarBurst(float timespeed = 0.5);
	
	void makeParticle(ofPoint pos, int colindex);
	
	void setObjects(PipeOrganData* pipeorgandata, ColumnData* coldata);


	ParticleSystemManager& particleSystemManager;
	PipeOrganData* pipeOrganData = NULL;
	ColumnData* colData = NULL;
	
	int mode;

};