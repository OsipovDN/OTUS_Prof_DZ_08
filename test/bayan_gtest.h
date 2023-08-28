#pragma once
#include <gtest/gtest.h>
#include "version.h"

TEST(Version, TestVersionCount) {
    // Arrange

    // Assert
    EXPECT_TRUE(PROJECT_VERSION_PATCH > 0);
}