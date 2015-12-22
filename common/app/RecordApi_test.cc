#include "thirdparty/gtest/gtest.h"
#include <string>
#include "common/app/FollowApi.h"
#include "common/app/RecordApi.h"
#include "common/app/CgiCode.h"
#include "common/utils/StringUtils.h"
#include "thirdparty/plog/Log.h"


using namespace std;

TEST(RecordApi, CreateRecordIdTest) {
  RecordApi api;
  string id1, id2;
  ASSERT_EQ(api.CreateRecordId(&id1), kCgiCodeOk);
  ASSERT_EQ(api.CreateRecordId(&id2), kCgiCodeOk);
  int64_t id;
  string_to_value(id1, id);
  EXPECT_EQ(id2, value_to_string(id + 1));
}

TEST(RecordApi, GetSetDelTest) {
  RecordApi api;
  string id;
  ASSERT_EQ(api.CreateRecordId(&id), kCgiCodeOk);
  RoughRecord record;
  record.set_id(id);
  record.set_user("1");
  record.set_time(time(NULL));
  record.set_weather("http://weather.jpg");
  record.set_mood("http://mood.jpg");
  record.set_status("http://status.jpg");
  record.set_text("hello patientsclub");
  record.set_address("中国");
  record.set_is_public(true);
  record.set_is_delete(false);
  record.set_picture_count(6);
  *(record.add_pictures()) = "http://pic1.jpg";
  *(record.add_pictures()) = "http://pic2.jpg";
  *(record.add_pictures()) = "http://pic3.jpg";

  // int Get(const std::string &id, RoughRecord *record);
  // int Set(const RoughRecord &record);
  ASSERT_EQ(api.Set(record), kCgiCodeOk);
  RoughRecord ret_record;
  ASSERT_EQ(api.Get(id,&ret_record), kCgiCodeOk);
  EXPECT_EQ(record.id(), ret_record.id());
  EXPECT_EQ(record.user(), ret_record.user());
  EXPECT_EQ(record.time(), ret_record.time());
  EXPECT_EQ(record.weather(), ret_record.weather());
  EXPECT_EQ(record.mood(), ret_record.mood());
  EXPECT_EQ(record.status(), ret_record.status());
  EXPECT_EQ(record.text(), ret_record.text());
  EXPECT_EQ(record.address(), ret_record.address());
  EXPECT_EQ(record.is_public(), ret_record.is_public());
  EXPECT_EQ(record.is_delete(), ret_record.is_delete());
  EXPECT_EQ(record.picture_count(), ret_record.picture_count());
  EXPECT_EQ(3u, ret_record.pictures_size());
  EXPECT_EQ("http://pic1.jpg", ret_record.pictures(0));
  EXPECT_EQ("http://pic2.jpg", ret_record.pictures(1));
  EXPECT_EQ("http://pic3.jpg", ret_record.pictures(2));

  // int Del(const std::string &id);
  ASSERT_EQ(api.Del(id), kCgiCodeOk);
  ASSERT_EQ(api.Get(id, &ret_record), kCgiCodeOk);
  EXPECT_EQ(true, ret_record.is_delete());

  // int SetRecordPublic(const std::string &id, bool is_public);
  ASSERT_EQ(api.SetRecordPublic(id, false), kCgiCodeOk);
  ASSERT_EQ(api.Get(id, &ret_record), kCgiCodeOk);
  EXPECT_EQ(ret_record.is_public(), false);
  ASSERT_EQ(api.SetRecordPublic(id, true), kCgiCodeOk);
  ASSERT_EQ(api.Get(id, &ret_record), kCgiCodeOk);
  EXPECT_EQ(true, ret_record.is_public());

  // int GetRecordOwner(const std::string &id, std::string *user);
  string user;
  ASSERT_EQ(api.GetRecordOwner(id, &user), kCgiCodeOk);
  EXPECT_EQ("1", user);

  // int Get(const std::string &id, StripRecord *record);
  StripRecord strip_record;
  ASSERT_EQ(api.Get(id, &strip_record), kCgiCodeOk);
  EXPECT_EQ(id, strip_record.id());
  EXPECT_EQ(record.pictures(0), strip_record.picture());
  EXPECT_EQ(record.text(), strip_record.text());
}

TEST(RecordApi, HomeRecordTest) {
  RecordApi api;
  map<string, RoughRecord> records;

  // clear data
  int page = 1;
  while(true) {
    if (api.GetHomeRecord("1", page, &records) == kCgiCodeNoMoreData) {
      break;
    }
    page++;
  }
  for (map<string, RoughRecord>::iterator iter = records.begin();
      iter != records.end(); ++iter) {
    ASSERT_EQ(api.UnlinkRecordToUserHome(iter->first, "1"), kCgiCodeOk);
  }

  string id;
  ASSERT_EQ(api.CreateRecordId(&id), kCgiCodeOk);
  RoughRecord record;
  record.set_id(id);
  record.set_user("1");
  record.set_time(time(NULL));
  ASSERT_EQ(api.Set(record), kCgiCodeOk);
  ASSERT_EQ(api.LinkRecordToUserHome(id, "1"), kCgiCodeOk);
  records.clear();
  ASSERT_EQ(api.GetHomeRecord("1", 1, &records), kCgiCodeNoMoreData);
  EXPECT_EQ(records.size(), 1u);
  EXPECT_EQ(records[id].id(), record.id());
  EXPECT_EQ(records[id].user(), record.user());
  EXPECT_EQ(records[id].time(), record.time());
  ASSERT_EQ(api.UnlinkRecordToUserHome(id, "1"), kCgiCodeOk);
}

TEST(RecordApi, HomeRecordPageTest) {
  RecordApi api;
  map<string, RoughRecord> records;
  ASSERT_EQ(api.GetHomeRecord("1", 1, &records), kCgiCodeNoMoreData);
  for (map<string, RoughRecord>::iterator iter = records.begin();
      iter != records.end(); ++iter) {
    ASSERT_EQ(api.UnlinkRecordToUserHome(iter->first, "1"), kCgiCodeOk);
  }

  RoughRecord record;
  record.set_user("1");
  record.set_time(time(NULL));
  for (int i = 0; i < 20; i++) {
    string record_id = value_to_string(i);
    record.set_id(record_id);
    ASSERT_EQ(api.Set(record), kCgiCodeOk);
    ASSERT_EQ(api.LinkRecordToUserHome(record_id, "1"), kCgiCodeOk);
  }

  records.clear();
  ASSERT_EQ(api.GetHomeRecord("1", 1, &records), kCgiCodeMoreData);
  EXPECT_EQ(records.size(), 12u);
  ASSERT_EQ(api.GetHomeRecord("1", 2, &records), kCgiCodeNoMoreData);
  EXPECT_EQ(records.size(), 20u);
  for (map<string, RoughRecord>::iterator iter = records.begin();
      iter != records.end(); ++iter) {
    ASSERT_EQ(api.UnlinkRecordToUserHome(iter->first, "1"), kCgiCodeOk);
  }
}

TEST(RecordApi, ActiveRecordTest) {
  RecordApi api;
  map<string, RoughRecord> records;
  ASSERT_EQ(api.GetActiveRecord("1", 1, &records), kCgiCodeNoMoreData);
  for (map<string, RoughRecord>::iterator iter = records.begin();
      iter != records.end(); ++iter) {
    ASSERT_EQ(api.UnlinkRecordToUserActive(iter->first, "1"), kCgiCodeOk);
  }

  string id;
  ASSERT_EQ(api.CreateRecordId(&id), kCgiCodeOk);
  RoughRecord record;
  record.set_id(id);
  record.set_user("1");
  record.set_time(time(NULL));
  ASSERT_EQ(api.Set(record), kCgiCodeOk);
  ASSERT_EQ(api.LinkRecordToUserActive(id, "1"), kCgiCodeOk);
  records.clear();
  ASSERT_EQ(api.GetActiveRecord("1", 1, &records), kCgiCodeNoMoreData);
  EXPECT_EQ(records.size(), 1u);
  EXPECT_EQ(records[id].id(), record.id());
  EXPECT_EQ(records[id].user(), record.user());
  EXPECT_EQ(records[id].time(), record.time());
  ASSERT_EQ(api.UnlinkRecordToUserActive(id, "1"), kCgiCodeOk);
}

TEST(RecordApi, ActiveRecordPageTest) {
  RecordApi api;
  map<string, RoughRecord> records;
  ASSERT_EQ(api.GetActiveRecord("1", 1, &records), kCgiCodeNoMoreData);
  for (map<string, RoughRecord>::iterator iter = records.begin();
      iter != records.end(); ++iter) {
    ASSERT_EQ(api.UnlinkRecordToUserActive(iter->first, "1"), kCgiCodeOk);
  }

  RoughRecord record;
  record.set_user("1");
  record.set_time(time(NULL));
  for (int i = 0; i < 20; i++) {
    string record_id = value_to_string(i);
    record.set_id(record_id);
    ASSERT_EQ(api.Set(record), kCgiCodeOk);
    ASSERT_EQ(api.LinkRecordToUserActive(record_id, "1"), kCgiCodeOk);
  }

  records.clear();
  ASSERT_EQ(api.GetActiveRecord("1", 1, &records), kCgiCodeMoreData);
  EXPECT_EQ(records.size(), 12u);
  ASSERT_EQ(api.GetActiveRecord("1", 2, &records), kCgiCodeNoMoreData);
  EXPECT_EQ(records.size(), 20u);
  for (map<string, RoughRecord>::iterator iter = records.begin();
      iter != records.end(); ++iter) {
    ASSERT_EQ(api.UnlinkRecordToUserActive(iter->first, "1"), kCgiCodeOk);
  }
}

int main(int argc, char **argv) {  
  ::testing::InitGoogleTest(&argc, argv);  
  plog::init(plog::debug, "./RecordApi_test.log");
  return RUN_ALL_TESTS();  
}
