#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\Reaction_Game\Player.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_ReactionGame
{		
	TEST_CLASS(Player_test)
	{
	public:
		
		TEST_METHOD(Test_construction)
		{
			Player p{ "Marlene" };
			Assert::AreEqual("Marlene", p.get_name().c_str(), "Player names are not equal");
			Assert::AreEqual(0.0f, p.get_points(),1E-6f);
		}

		TEST_METHOD(Test_Add_Points)
		{
			Player p{ "Michael" };
			p.add_win();
			Assert::AreEqual(1.0f, p.get_points(), 1E-6f);
			p.add_draw();
			Assert::AreEqual(1.5f, p.get_points(), 1E-6f);
			p.add_draw();
			Assert::AreEqual(2.0f, p.get_points(), 1E-6f);
			p.add_win();
			Assert::AreEqual(3.0f, p.get_points(), 1E-6f);
		}

		TEST_METHOD(Test_Reset_Points)
		{
			Player p{ "Jane Doe" };
			p.add_win();
			p.add_draw();
			Assert::AreEqual(1.5f, p.get_points(), 1E-6f);
			p.reset_points();
			Assert::AreEqual(0.0f, p.get_points(), 1E-6f);
		}

		TEST_METHOD(Test_Restart_Points)
		{
			Player p{ "John Doe" };
			p.add_win();
			Assert::AreEqual(1.0f, p.get_points(), 1E-6f);
			p.reset_points();
			Assert::AreEqual(0.0f, p.get_points(), 1E-6f);
			p.add_draw();
			Assert::AreEqual(0.5f, p.get_points(), 1E-6f);
		}
	};
}