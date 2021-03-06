#include "CWall.h"
#include "Utils.h"
#include "CPlayer.h"
#include <iostream>

namespace hg
{
	void movePoint(Vector2f &mVector, const Vector2f mCenter, const float mSpeed)
	{
		Vector2f m { mCenter - mVector };
		m = getNormalized(m);
		m *= mSpeed;
		mVector += m;
	}

	CWall::CWall(HexagonGame *mHgPtr, Vector2f mCenterPos, int mSide, float mThickness, float mDistance, float mSpeed) :
			Component{"wall"}, hgPtr{mHgPtr}, centerPos{mCenterPos}, speed{mSpeed}
	{
		float div { 360.f / hgPtr->getSides() };
		float angle { div * mSide };

		p1 = getOrbit(centerPos, angle - div * 0.5f, mDistance);
		p2 = getOrbit(centerPos, angle + div * 0.5f, mDistance);
		p3 = getOrbit(centerPos, angle + div * 0.5f, mDistance + mThickness);
		p4 = getOrbit(centerPos, angle - div * 0.5f, mDistance + mThickness);
	}

	bool CWall::isOverlapping(Vector2f mPoint) { return isPointInPolygon(pointPtrs, mPoint); }

	void CWall::draw()
	{
		Color color { hgPtr->getColorMain() };

		vertices[0].position = p1;
		vertices[1].position = p2;
		vertices[2].position = p3;
		vertices[3].position = p4;

		vertices[0].color = color;
		vertices[1].color = color;
		vertices[2].color = color;
		vertices[3].color = color;

		hgPtr->drawOnTexture(vertices);
	}

	void CWall::update(float mFrameTime)
	{
		float radius { hgPtr->getRadius() * 0.65f };
		int pointsOnCenter { 0 };

		for(auto pointPtr : pointPtrs)
		{
			float distanceX { abs(pointPtr->x - centerPos.x) };
			float distanceY { abs(pointPtr->y - centerPos.y) };

			if(distanceX < radius && distanceY < radius) pointsOnCenter++;
			else movePoint(*pointPtr, centerPos, speed * mFrameTime);
		}

		if(pointsOnCenter > 3) getEntity().destroy();
	}
}

