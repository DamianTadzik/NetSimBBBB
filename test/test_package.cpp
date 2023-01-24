//#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "package.hpp"
#include "types.hpp"

TEST(PackageTest, IsAssignedIdLowest) {
    // przydzielanie ID o jeden większych -- utworzenie dwóch obiektów pod rząd

    Package p1;
    Package p2;

    EXPECT_EQ(p1.get_id(), 1);
    EXPECT_EQ(p2.get_id(), 2);
}

TEST(PackageTest, IsIdReused) {
    // przydzielanie ID po zwolnionym obiekcie

    {
        Package p1;
    }
    Package p2;

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(PackageTest, IsMoveConstructorCorrect) {
    Package p1;
    Package p2(std::move(p1));

    EXPECT_EQ(p2.get_id(), 1);
}


TEST(PackageTest, IsAssignmentOperatorCorrect) {
    Package p1;
    Package p2;
    p2 = std::move(p1);

    EXPECT_EQ(p2.get_id(), 1);
}


TEST(PackageTest, ThrowAnException) {
    try {
        Package p1(1);
        Package p2(2);
        Package p3(2);
        FAIL();
    }
    catch (std::logic_error& already_assigned) {
        SUCCEED();
    }
}
