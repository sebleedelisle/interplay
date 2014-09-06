//
//  EffectParticles.cpp
//  LaserShow
//
//  Created by Seb Lee-Delisle on 16/03/2014.
//
//

#include "EffectParticles.h"


void EffectParticles:: makeRainbowBurst() {
	if(pipeOrganData == NULL) return; 
	
	float numpipes = pipeOrganData->pipes.size();
	
	
	for(int i =0; i<numpipes; i+=1) {
		makeParticleForPipe(i) ;
		
	}
	
	
}


void EffectParticles:: makeStarBurst(float timespeed ) {
	
	if(colData == NULL) return;
	
	
	int numcols = colData->colPositions.size();
	
	for(int i =0; i<numcols; i+=1) {
		
		ParticleSystem &ps = *particleSystemManager.getParticleSystem();
		
		ParticleSystemSettings pss;
		pss.emitLifeTime = 0.1;
		
		pss.emitCount = 20;
		pss.renderer = new ParticleRendererLaser();
		pss.speedMin = 2000 ;
		pss.speedMax = 2450;
		pss.drag = 0.90;
		pss.gravity.set(0,600,0);
		
		pss.sizeStartMin = pss.sizeStartMax = 1;
		pss.sizeChangeRatio = 0.5;
		
		pss.directionYVar = pss.directionZVar = 0;
		pss.directionX = colData->rotation->x;
		pss.directionXVar = 2;
		pss.directionZVar = 2;
		
		pss.hueStartMin = pss.hueStartMax = 0;
		pss.hueChange = 0;
		pss.saturationMin = pss.saturationMax = 0;
		pss.saturationEnd = 0;
		pss.brightnessStartMin = pss.brightnessStartMax =pss.brightnessEnd = 255;
		pss.lifeMin = 2;
		pss.lifeMax = 3;
		pss.shimmerMin = 0;
		pss.timeSpeed = timespeed;
	
		pss.emitDelay = ofRandom(0.1);

		ps.pos = colData->getColumnBasePos(i);
		ps.init(pss);
	}
	
	
}

void EffectParticles ::  makeParticle(ofVec3f pos, int columnNum){
	
	
	
	if(mode ==0 ) return;

	
	ParticleSystemSettings pss;
	
	if(mode ==1 ) {
		ParticleSystem &ps = *particleSystemManager.getParticleSystem();
		
		pss.emitLifeTime = 0.1;
		pss.emitMode = PARTICLE_EMIT_BURST;
		pss.emitCount = 1;
		pss.renderer = new ParticleRendererLaser();
		pss.speedMin = 1500 ;
		pss.speedMax = 200;
		pss.drag = 0.90;
		pss.gravity.set(0,600,0);
		
		pss.sizeStartMin = pss.sizeStartMax = 1;
		pss.sizeChangeRatio = 0.5;
		//pss.emitShape = &explodeMesh;
		pss.directionYVar = pss.directionZVar = 0;
		pss.directionX = colData->rotation->x;
		pss.directionXVar = 0;
		pss.directionZVar = 0;
		
		pss.hueStartMin = pss.hueStartMax = 0;
		pss.hueChange = 0;
		pss.saturationMin = pss.saturationMax = 0;
		pss.saturationEnd = 0;
		pss.brightnessStartMin = pss.brightnessStartMax =pss.brightnessEnd = 255;
		pss.lifeMin = 0.5;
		pss.lifeMax = 1;
		pss.shimmerMin = 0;
		pss.timeSpeed = 0.5;
		
		pss.emitDelay = 0;//ofRandom(0.1);
		
		ps.pos = pos;
		ps.init(pss);

	} else if(mode ==2) {
		ParticleSystem &ps = *particleSystemManager.getParticleSystem();
		
		pss.emitLifeTime = 0.1;
		pss.emitMode = PARTICLE_EMIT_BURST;
		pss.emitCount = 1;
		pss.renderer = new ParticleRendererLaserLine(6);
		pss.speedMin = 600 ;
		pss.speedMax = 650;
		pss.drag = 1;
		pss.gravity.set(0,500,0);
		
		pss.sizeStartMin = pss.sizeStartMax = 1;
		pss.sizeChangeRatio = 0.1;
		//pss.emitShape = &explodeMesh;
		pss.directionYVar = pss.directionZVar = 0;
		pss.directionY = 0;
		pss.directionX = colData->rotation->x;
		
		pss.hueStartMin = pss.hueStartMax = 0;
		pss.hueChange = 360;
		pss.saturationMin = pss.saturationMax = 255;
		pss.saturationEnd = 255;
		pss.brightnessStartMin = pss.brightnessStartMax =pss.brightnessEnd = 255;
		pss.lifeMin = pss.lifeMax = 1.2;
		pss.shimmerMin = 0;
		pss.timeSpeed = 0.8;
		//pss.doNotScale = true;
		
		ps.pos = pos;
		ps.init(pss);
		
		
	} else if(mode ==3) {
		
		
		// PHYSICS
		pss.speedMin = 0;
		pss.speedMax = 30;
		pss.directionZ = -90;
		pss.directionZVar = 90;
		//ps.directionZVar = 0;
		pss.directionYVar = 0;
		
		pss.drag = 0.90;
		pss.gravity.set(0,0);
		
		//LIFE
		pss.lifeMin = 0.5;
		pss.lifeMax = 0.7;
		
		//APPEARANCE
		
		pss.sizeStartMin = 4;
		pss.sizeStartMax = 6;
		pss.sizeChangeRatio = 1;
		
		float hueStartOffset = (columnNum%2 == 0)? 130 : 235;
		float hueChange = 10;
		
		pss.hueStartMin = -5+hueStartOffset;
		pss.hueStartMax = 0+hueStartOffset;
		pss.hueChange = hueChange;
		
		pss.brightnessStartMin = 230;
		pss.brightnessStartMax = 255;
		pss.brightnessEnd = 0;
		
		pss.saturationMin = 150;
		pss.saturationMax = 255;
		pss.saturationEnd = 255;
		
		pss.emitMode = PARTICLE_EMIT_CONTINUOUS;
		pss.emitCount = 20;
		pss.emitDelay = 0;
		pss.emitLifeTime= ofRandom(0.2, 1.4);
		
		pss.emitStartSizeModifier = 0;
		pss.emitSpeedModifier = 10;
		pss.emitHueModifierOffset = 0;
		pss.emitInheritVelocity = 0;
		
		
		pss.renderer = new ParticleRendererGlitchLineLaser();

		
		RocketSettings* rs = new RocketSettings();
		
		rs->startSpeedMin = 800 * 3;
		rs->startSpeedMax = 950 *3 ;
		rs->direction = -90;
		rs->directionVar = 0;
		rs->gravity.y = 0;
		rs->setLifeTime(pss.emitLifeTime);
		rs->drag = 0.9;
		
		rs->addParticleSystemSetting(pss);
		
		
		
		//ps.init(pss);
		
		PhysicsObject*rocket = particleSystemManager.addRocket(*rs, pos);
		
		
	}
	
	
	
}


void EffectParticles :: makeParticleForPipe(int pipeindex) {
	if(pipeOrganData == NULL) return;
	
	
	
		
	ParticleSystem &ps = *particleSystemManager.getParticleSystem();
	
	ParticleSystemSettings pss;
	
		
		pss.emitLifeTime = 0.1;
		pss.emitMode = PARTICLE_EMIT_BURST;
		pss.emitCount = 1;
		pss.renderer = new ParticleRendererLaserLine(6);
		pss.speedMin = 600 ;
		pss.speedMax = 650;
		pss.drag = 1;
		pss.gravity.set(0,500,0);
		
		pss.sizeStartMin = pss.sizeStartMax = 1;
		pss.sizeChangeRatio = 0.1;
		//pss.emitShape = &explodeMesh;
		pss.directionYVar = pss.directionZVar = 0;
		pss.directionY = 50;
		pss.directionX = -35;
		
		pss.hueStartMin = pss.hueStartMax = 0;
		pss.hueChange = 360;
		pss.saturationMin = pss.saturationMax = 255;
		pss.saturationEnd = 255;
		pss.brightnessStartMin = pss.brightnessStartMax =pss.brightnessEnd = 255;
		pss.lifeMin = pss.lifeMax = 1.7;
		pss.shimmerMin = 0;
		pss.timeSpeed = 0.7;
		//pss.doNotScale = true;
		
		ps.pos = pipeOrganData->pipes[pipeindex].top;
		ps.init(pss);
		
	
	
}


void EffectParticles ::  setObjects(PipeOrganData* pipeorgandata, ColumnData* coldata) {
	pipeOrganData = pipeorgandata;
	colData = coldata;
	
}

