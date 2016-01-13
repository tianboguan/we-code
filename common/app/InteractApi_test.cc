#include "thirdparty/gtest/gtest.h"
#include <string>
#include "common/app/InteractApi.h"
#include "common/app/RecordApi.h"
#include "common/app/ProfileApi.h"
#include "common/app/CgiCode.h"
#include "common/utils/StringUtils.h"
#include "thirdparty/plog/Log.h"


using namespace std;

class InteractApiTest: public testing::Test {
  public:
    void SetUp() {
      // create 2 user;
      ProfileApi profile_api;
      UserProfile profile;
      profile.set_user("1");
      profile.set_nickname("nickname1");
      profile.set_head("head1");
      ASSERT_EQ(profile_api.Create(profile, false), kCgiCodeOk);
      profile.set_user("2");
      profile.set_nickname("nickname2");
      profile.set_head("head2");
      ASSERT_EQ(profile_api.Create(profile, false), kCgiCodeOk);

      // create 1 record;
      RecordApi record_api;
      RoughRecord record;
      record.set_id("1");
      record.set_user("2");
      record.set_text("text");
      ASSERT_EQ(record_api.Set(record), kCgiCodeOk);
    }

    void TearDown() {
    }
};

TEST_F(InteractApiTest, LikeTest) {
  InteractApi api("1");

  vector<ExtInteract> interacts;
  ASSERT_EQ(api.ClearRecordInteracts("1"), kCgiCodeOk);
  ASSERT_EQ(api.GetRecordInteracts("1", &interacts), kCgiCodeOk);
  ASSERT_EQ(interacts.size(), 0u);

  interacts.clear();
  ASSERT_EQ(api.Like("1"), kCgiCodeOk);
  ASSERT_EQ(api.GetRecordInteracts("1", &interacts), kCgiCodeOk);
  ASSERT_EQ(interacts.size(), 1u);
  ExtInteract interact = interacts[0];
  EXPECT_EQ(interact.interact().type(),  INTER_TYPE_LIKE);
  EXPECT_NEAR(interact.interact().time(), time(NULL), 1);
  EXPECT_EQ(interact.user().user(), "1");
  EXPECT_EQ(interact.user().nickname(), "nickname1");
  EXPECT_EQ(interact.user().head(), "head1");
  // EXPECT_EQ(interact.record().id(), "1");
  // EXPECT_EQ(interact.record().text(), "text");

  interacts.clear();
  ASSERT_EQ(api.Unlike("1"), kCgiCodeOk);
  ASSERT_EQ(api.GetRecordInteracts("1", &interacts), kCgiCodeOk);
  ASSERT_EQ(interacts.size(), 0u);
}

TEST_F(InteractApiTest, CommentTest) {
  InteractApi api("1");

  // clear exist interact
  vector<ExtInteract> interacts;
  ASSERT_EQ(api.ClearRecordInteracts("1"), kCgiCodeOk);
  ASSERT_EQ(api.GetRecordInteracts("1", &interacts), kCgiCodeOk);
  ASSERT_EQ(interacts.size(), 0u);

  interacts.clear();
  ASSERT_EQ(api.Comment("1", "comment"), kCgiCodeOk);
  ASSERT_EQ(api.GetRecordInteracts("1", &interacts), kCgiCodeOk);
  ASSERT_EQ(interacts.size(), 1u);
  ExtInteract interact = interacts[0];
  EXPECT_EQ(interact.interact().type(), INTER_TYPE_COMMENT);
  EXPECT_EQ(interact.interact().text(), "comment");
  EXPECT_EQ(interact.interact().target_nickname(), "");
  EXPECT_NEAR(interact.interact().time(), time(NULL), 1);
  EXPECT_EQ(interact.user().user(), "1");
  EXPECT_EQ(interact.user().nickname(), "nickname1");
  EXPECT_EQ(interact.user().head(), "head1");

  interacts.clear();
  ASSERT_EQ(api.Uncomment(interact.interact().id(), "1"), kCgiCodeOk);
  ASSERT_EQ(api.GetRecordInteracts("1", &interacts), kCgiCodeOk);
  ASSERT_EQ(interacts.size(), 0u);
}

TEST_F(InteractApiTest, CommentCommentTest) {
  InteractApi api("1");

  // clear exist interact
  vector<ExtInteract> interacts;
  ASSERT_EQ(api.ClearRecordInteracts("1"), kCgiCodeOk);
  ASSERT_EQ(api.GetRecordInteracts("1", &interacts), kCgiCodeOk);
  ASSERT_EQ(interacts.size(), 0u);

  interacts.clear();
  ASSERT_EQ(api.Comment("1", "comment1"), kCgiCodeOk);
  ASSERT_EQ(api.GetRecordInteracts("1", &interacts), kCgiCodeOk);
  ASSERT_EQ(interacts.size(), 1u);
  ExtInteract interact = interacts[0];
  InteractApi api2("2");
  ASSERT_EQ(api2.Comment("1", "comment2", interact.interact().id()), kCgiCodeOk);
  interacts.clear();
  ASSERT_EQ(api.GetRecordInteracts("1", &interacts), kCgiCodeOk);
  ASSERT_EQ(interacts.size(), 2u);

  interact = interacts[0];
  EXPECT_EQ(interact.interact().type(), INTER_TYPE_COMMENT);
  EXPECT_EQ(interact.interact().text(), "comment2");
  EXPECT_EQ(interact.interact().target_nickname(), "nickname1");
  EXPECT_NEAR(interact.interact().time(), time(NULL), 1);
  EXPECT_EQ(interact.user().user(), "2");
  EXPECT_EQ(interact.user().nickname(), "nickname2");
  EXPECT_EQ(interact.user().head(), "head2");

  interact = interacts[1];
  EXPECT_EQ(interact.interact().type(), INTER_TYPE_COMMENT);
  EXPECT_EQ(interact.interact().text(), "comment1");
  EXPECT_EQ(interact.interact().target_nickname(), "");
  EXPECT_NEAR(interact.interact().time(), time(NULL), 1);
  EXPECT_EQ(interact.user().user(), "1");
  EXPECT_EQ(interact.user().nickname(), "nickname1");
  EXPECT_EQ(interact.user().head(), "head1");

  ASSERT_EQ(api.Uncomment(interacts[1].interact().id(), "1"), kCgiCodeOk);
  ASSERT_EQ(api.Uncomment(interacts[0].interact().id(), "1"), kCgiCodeOk);
  interacts.clear();
  ASSERT_EQ(api.GetRecordInteracts("1", &interacts), kCgiCodeOk);
  ASSERT_EQ(interacts.size(), 0u);
}

TEST_F(InteractApiTest, GetRecordInteractsTest) {
  InteractApi api("1");

  vector<ExtInteract> interacts;
  ASSERT_EQ(api.ClearRecordInteracts("1"), kCgiCodeOk);
  ASSERT_EQ(api.GetRecordInteracts("1", &interacts), kCgiCodeOk);
  ASSERT_EQ(interacts.size(), 0u);

  ASSERT_EQ(api.Comment("1", "comment1"), kCgiCodeOk);
  ASSERT_EQ(api.GetRecordInteracts("1", &interacts), kCgiCodeOk);
  ASSERT_EQ(interacts.size(), 1u);
}

TEST_F(InteractApiTest, GetUserNoticeInteractsTest) {
  InteractApi api("1");

  vector<ExtInteract> interacts;
  InteractApi api2("2");
  ASSERT_EQ(api2.ClearUserNoticeInteracts(), kCgiCodeOk);
  ASSERT_EQ(api2.GetUserNoticeInteracts(1, &interacts), kCgiCodeNoMoreData);
  ASSERT_EQ(interacts.size(), 0u);

  ASSERT_EQ(api.Comment("1", "comment1"), kCgiCodeOk);
  ASSERT_EQ(api2.GetUserNoticeInteracts(1, &interacts), kCgiCodeOk);
  ASSERT_EQ(interacts.size(), 1u);
  ExtInteract interact = interacts[0];
  EXPECT_EQ(interact.interact().type(),  INTER_TYPE_COMMENT);
  EXPECT_NEAR(interact.interact().time(), time(NULL), 1);
  EXPECT_EQ(interact.interact().text(), "comment1");
  EXPECT_EQ(interact.user().user(), "1");
  EXPECT_EQ(interact.user().nickname(), "nickname1");
  EXPECT_EQ(interact.user().head(), "head1");
  EXPECT_EQ(interact.record().id(), "1");
  EXPECT_EQ(interact.record().text(), "text");
  ASSERT_EQ(api2.GetUserNoticeInteracts(2, &interacts), kCgiCodeNoMoreData);

  interacts.clear();
  ASSERT_EQ(api2.GetUserNoticeInteracts(1, &interacts), kCgiCodeNoMoreData);
  ASSERT_EQ(interacts.size(), 0u);

  for (int i = 0; i < 30; i++) {
    ASSERT_EQ(api.Comment("1", "comment1"), kCgiCodeOk);
  }
  interacts.clear();
  ASSERT_EQ(api2.GetUserNoticeInteracts(1, &interacts), kCgiCodeOk);
  ASSERT_EQ(interacts.size(), 28u);
  interacts.clear();
  ASSERT_EQ(api2.GetUserNoticeInteracts(1, &interacts), kCgiCodeOk);
  ASSERT_EQ(interacts.size(), 2u);
}

TEST_F(InteractApiTest, GetUserNoticeInteractsSelfTest) {
  InteractApi api("1");

  vector<ExtInteract> interacts;
  InteractApi api2("2");
  ASSERT_EQ(api2.ClearUserNoticeInteracts(), kCgiCodeOk);
  ASSERT_EQ(api2.GetUserNoticeInteracts(1, &interacts), kCgiCodeNoMoreData);
  ASSERT_EQ(interacts.size(), 0u);

  ASSERT_EQ(api2.Comment("1", "comment1"), kCgiCodeOk);
  ASSERT_EQ(api2.GetUserNoticeInteracts(1, &interacts), kCgiCodeNoMoreData);
  ASSERT_EQ(interacts.size(), 0u);
}

TEST_F(InteractApiTest, GetUserReceivedInteractsTest) {
  InteractApi api("1");

  vector<ExtInteract> interacts;
  InteractApi api2("2");
  ASSERT_EQ(api2.ClearUserReceivedInteracts(), kCgiCodeOk);
  ASSERT_EQ(api2.GetUserReceivedInteracts(1, &interacts), kCgiCodeNoMoreData);
  ASSERT_EQ(interacts.size(), 0u);

  ASSERT_EQ(api.Comment("1", "comment2"), kCgiCodeOk);
  ASSERT_EQ(api2.GetUserReceivedInteracts(1, &interacts), kCgiCodeOk);
  ASSERT_EQ(interacts.size(), 1u);
  ExtInteract interact = interacts[0];
  EXPECT_EQ(interact.interact().type(),  INTER_TYPE_COMMENT);
  EXPECT_NEAR(interact.interact().time(), time(NULL), 1);
  EXPECT_EQ(interact.interact().text(), "comment2");
  EXPECT_EQ(interact.user().user(), "1");
  EXPECT_EQ(interact.user().nickname(), "nickname1");
  EXPECT_EQ(interact.user().head(), "head1");
  EXPECT_EQ(interact.record().id(), "1");
  EXPECT_EQ(interact.record().text(), "text");
  ASSERT_EQ(api2.GetUserReceivedInteracts(2, &interacts), kCgiCodeNoMoreData);
}

TEST_F(InteractApiTest, GetUserReceivedInteractsSelfTest) {
  InteractApi api("1");

  vector<ExtInteract> interacts;
  InteractApi api2("2");
  ASSERT_EQ(api2.ClearUserReceivedInteracts(), kCgiCodeOk);
  ASSERT_EQ(api2.GetUserReceivedInteracts(1, &interacts), kCgiCodeNoMoreData);
  ASSERT_EQ(interacts.size(), 0u);

  ASSERT_EQ(api2.Comment("1", "comment2"), kCgiCodeOk);
  ASSERT_EQ(api2.GetUserReceivedInteracts(1, &interacts), kCgiCodeNoMoreData);
  ASSERT_EQ(interacts.size(), 0u);
}

TEST_F(InteractApiTest, GetUserSendedInteractsTest) {
  InteractApi api("1");

  vector<ExtInteract> interacts;
  InteractApi api2("2");
  ASSERT_EQ(api2.ClearUserSendedInteracts(), kCgiCodeOk);
  ASSERT_EQ(api2.GetUserSendedInteracts(1, &interacts), kCgiCodeNoMoreData);
  ASSERT_EQ(interacts.size(), 0u);

  ASSERT_EQ(api2.Comment("1", "comment2"), kCgiCodeOk);
  ASSERT_EQ(api2.GetUserSendedInteracts(1, &interacts), kCgiCodeOk);
  ASSERT_EQ(interacts.size(), 1u);
  ASSERT_EQ(api2.GetUserSendedInteracts(2, &interacts), kCgiCodeNoMoreData);

  ASSERT_EQ(api.ClearUserSendedInteracts(), kCgiCodeOk);
  interacts.clear();
  ASSERT_EQ(api.Comment("1", "comment1"), kCgiCodeOk);
  ASSERT_EQ(api.GetUserSendedInteracts(1, &interacts), kCgiCodeOk);
  ASSERT_EQ(interacts.size(), 1u);
  ExtInteract interact = interacts[0];
  EXPECT_EQ(interact.interact().type(),  INTER_TYPE_COMMENT);
  EXPECT_NEAR(interact.interact().time(), time(NULL), 1);
  EXPECT_EQ(interact.interact().text(), "comment1");
  EXPECT_EQ(interact.user().user(), "1");
  EXPECT_EQ(interact.user().nickname(), "nickname1");
  EXPECT_EQ(interact.user().head(), "head1");
  EXPECT_EQ(interact.record().id(), "1");
  EXPECT_EQ(interact.record().text(), "text");
  ASSERT_EQ(api.GetUserSendedInteracts(2, &interacts), kCgiCodeNoMoreData);
}

int main(int argc, char **argv) {  
  ::testing::InitGoogleTest(&argc, argv);  
  plog::init(plog::debug, "./InteractApi_test.log");
  return RUN_ALL_TESTS();  
}
