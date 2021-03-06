#include <gtest/gtest.h>

#include <DescentEngine/src/EntityEngine/TimeSliceAction.h>

TEST(TimeSlice, activate) {
	TimeSliceAction tsa;

	ASSERT_TRUE(tsa.canActivate());
	tsa.activate(1.0f);
	ASSERT_FALSE(tsa.canActivate());
	tsa.validate(0.5f, 0.0f);
	ASSERT_FALSE(tsa.wasJustDisabled());
	ASSERT_FALSE(tsa.canActivate());
	ASSERT_TRUE(tsa.isActive());
	tsa.validate(0.6f, 0.0f);
	ASSERT_TRUE(tsa.wasJustDisabled());
	ASSERT_FALSE(tsa.wasJustDisabled());
	ASSERT_TRUE(tsa.canActivate());
	ASSERT_FALSE(tsa.isActive());
}

