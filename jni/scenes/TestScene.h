#pragma once

#include <defines.h>
#include <SceneManaging\BaseScene.h>
#include <Logger.h>
#include <Context.h>
#include <SceneGraph/Object.h>
#include <StarComponents.h>
#include <Objects/Sprite.h>
#include <Input/Gestures/TapGesture.h>

class TestScene : public star::BaseScene
{
public:
	TestScene(const tstring& name);
	virtual ~TestScene() {}

	status Initialize(const star::Context& context);
	status Update(const star::Context& context);
	status Draw();

private:
	star::TapGesture* m_TapGesture;
	int m_TotalFrames;
	int m_Step;
	int m_FPS;
	float m_PassedMiliseconds;

	float mRed, mGreen, mBlue;

	star::Sprite* m_pTestSprite;
	star::Object	*m_pObjectOne, 
			*m_pObjectTwo, 
			*m_pObjectThree,
			*m_pObjectFour, 
			*m_pObjectFive, 
			*m_pObjectSix;

	star::RectangleColliderComponent* m_pRectCompOne;
	star::CircleColliderComponent	*m_pRectCompTwo;

	star::PathFindNodeComponent	*m_pPathFindComp, 
								*m_pPathFindCompTwo, 
								*m_pPathFindCompThree, 
								*m_pPathFindCompFour, 
								*m_pPathFindCompFive, 
								*m_pPathFindCompSix;
		
	TestScene(const TestScene& t);
	TestScene(TestScene&& t);
	TestScene& operator=(const TestScene& t);
};