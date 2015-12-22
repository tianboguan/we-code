#include "thirdparty/gtest/gtest.h"
#include <string>
#include "common/app/FollowApi.h"
#include "common/app/CgiCode.h"
#include "thirdparty/plog/Log.h"

using namespace std;

TEST(FollowApi, IsFollowTest) {
  FollowApi api("1");
  bool followed;
  ASSERT_EQ(api.UnFollow("2"), kCgiCodeOk);
  ASSERT_EQ(api.IsFollowed("2", &followed), kCgiCodeOk);
  EXPECT_EQ(followed, false); 
  ASSERT_EQ(api.Follow("2"), kCgiCodeOk);
  ASSERT_EQ(api.IsFollowed("2", &followed), kCgiCodeOk);
  EXPECT_EQ(followed, true); 
  ASSERT_EQ(api.UnFollow("2"), kCgiCodeOk);
  ASSERT_EQ(api.IsFollowed("2", &followed), kCgiCodeOk);
  EXPECT_EQ(followed, false); 
}

TEST(FollowApi, FollowTest) {
  FollowApi api("1");
  vector<string> follow_to;
  ASSERT_EQ(api.GetFollowTo("1", &follow_to), kCgiCodeOk);
  for (vector<string>::iterator iter = follow_to.begin();
      iter != follow_to.end(); ++iter) {
    ASSERT_EQ(api.UnFollow(*iter), kCgiCodeOk);
  }
  ASSERT_EQ(api.Follow("1"), kCgiCodeFollowOperatorSelf);
  ASSERT_EQ(api.Follow("2"), kCgiCodeOk);
  ASSERT_EQ(api.Follow("2"), kCgiCodeOk);
  ASSERT_EQ(api.Follow("3"), kCgiCodeOk);

  follow_to.clear();
  ASSERT_EQ(api.GetFollowTo("1", &follow_to), kCgiCodeOk);
  EXPECT_EQ(follow_to.size(), 2u);
  EXPECT_EQ(follow_to[0], "2");
  EXPECT_EQ(follow_to[1], "3");

  vector<string> follow_from;
  ASSERT_EQ(api.GetFollowFrom("2", &follow_from), kCgiCodeOk);
  ASSERT_EQ(follow_from.size(), 1u);
  EXPECT_EQ(follow_from[0], "1");
  follow_from.clear();
  ASSERT_EQ(api.GetFollowFrom("3", &follow_from), kCgiCodeOk);
  ASSERT_EQ(follow_from.size(), 1u);
  EXPECT_EQ(follow_from[0], "1");
  ASSERT_EQ(api.UnFollow("2"), kCgiCodeOk);
  ASSERT_EQ(api.UnFollow("2"), kCgiCodeOk);
}

TEST(FollowApi, UnFollowTest) {
  FollowApi api("1");
  vector<string> follow_to;
  ASSERT_EQ(api.GetFollowTo("1", &follow_to), kCgiCodeOk);
  for (vector<string>::iterator iter = follow_to.begin();
      iter != follow_to.end(); ++iter) {
    ASSERT_EQ(api.UnFollow(*iter), kCgiCodeOk);
  }
  follow_to.clear();
  ASSERT_EQ(api.GetFollowTo("1", &follow_to), kCgiCodeOk);
  EXPECT_EQ(follow_to.size(), 0u);

  ASSERT_EQ(api.Follow("2"), kCgiCodeOk);
  ASSERT_EQ(api.Follow("3"), kCgiCodeOk);
  follow_to.clear();
  ASSERT_EQ(api.GetFollowTo("1", &follow_to), kCgiCodeOk);
  EXPECT_EQ(follow_to.size(), 2u);
  vector<string> follow_from;
  ASSERT_EQ(api.GetFollowFrom("2", &follow_from), kCgiCodeOk);
  EXPECT_EQ(follow_from.size(), 1u);
  follow_from.clear();
  ASSERT_EQ(api.GetFollowFrom("3", &follow_from), kCgiCodeOk);
  EXPECT_EQ(follow_from.size(), 1u);

  ASSERT_EQ(api.UnFollow("2"), kCgiCodeOk);
  ASSERT_EQ(api.UnFollow("3"), kCgiCodeOk);
  follow_to.clear();
  ASSERT_EQ(api.GetFollowTo("1", &follow_to), kCgiCodeOk);
  EXPECT_EQ(follow_to.size(), 0u);
  follow_from.clear();
  ASSERT_EQ(api.GetFollowFrom("2", &follow_from), kCgiCodeOk);
  EXPECT_EQ(follow_from.size(), 0u);
  follow_from.clear();
  ASSERT_EQ(api.GetFollowFrom("3", &follow_from), kCgiCodeOk);
  EXPECT_EQ(follow_from.size(), 0u);
}

TEST(FollowApi, BlockTest) {
  FollowApi api("1");
  ASSERT_EQ(api.UnBlock("2"), kCgiCodeOk);
  bool blocked;
  ASSERT_EQ(api.IsBlocked("1", "2", &blocked), kCgiCodeOk);
  EXPECT_EQ(blocked, false);

  ASSERT_EQ(api.Block("2"), kCgiCodeOk);
  ASSERT_EQ(api.IsBlocked("1", "2", &blocked), kCgiCodeOk);
  EXPECT_EQ(blocked, true);

  FollowApi api2("2");
  ASSERT_EQ(api2.Follow("1"), kCgiCodeBlocked);
  bool followed;
  ASSERT_EQ(api2.IsFollowed("1", &followed), kCgiCodeOk);
  EXPECT_EQ(followed, false); 
}

int main(int argc, char **argv) {  
  ::testing::InitGoogleTest(&argc, argv);  
  plog::init(plog::debug, "./FollowApi_UT.log");
  return RUN_ALL_TESTS();  
}
