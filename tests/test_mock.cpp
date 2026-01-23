#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <expected>
#include <string>

struct IRepository {
  virtual ~IRepository() = default;
  virtual std::expected<void,std::string> save(std::string_view data) = 0;
};

struct MockRepo : IRepository {
  MOCK_METHOD((std::expected<void,std::string>), save, (std::string_view), (override));
};

TEST(Repo, SaveOk) {
  MockRepo repo;
  EXPECT_CALL(repo, save("hello"))
    .WillOnce(::testing::Return(std::expected<void,std::string>{}));
  auto r = repo.save("hello");
  EXPECT_TRUE(r.has_value());
}

TEST(Repo, SaveError) {
  MockRepo repo;
  EXPECT_CALL(repo, save("bad"))
    .WillOnce(::testing::Return(std::unexpected("disk full")));
  auto r = repo.save("bad");
  ASSERT_TRUE(!r.has_value());
  EXPECT_EQ(r.error(), "disk full");
}

