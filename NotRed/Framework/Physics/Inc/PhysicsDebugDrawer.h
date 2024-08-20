#pragma once

namespace NotRed::Physics
{
	class PhysicsDebugDrawer final : public btIDebugDraw
	{
	public:
		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
		void drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
		void reportErrorWarning(const char* warningString) override;
		void draw3dText(const btVector3& location, const char* textString) override;
		void setDebugMode(int debugMode) override;
		int getDebugMode() const override;


	private:
		int mDebugMode = 0;
	};
}