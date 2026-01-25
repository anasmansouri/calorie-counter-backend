#pragma once
#include "models/food.hpp"
#include "utils/Result.hpp"
#include <memory>
#include <string>
#include <vector>

namespace cc::clients {

class OpenFoodFactsClient {
  public:
    explicit OpenFoodFactsClient(std::string baseUrl = "https://world.openfoodfacts.org",
                                 const std::string userAgent =
                                     "CalorieCounter/1.0 (+https://example.com; anas@example.com)");
    cc::utils::Result<cc::models::Food> getByBarcode(const std::string& barcode);
    cc::utils::Result<cc::models::Food>
    parseFoodFromOffJson_barcode(cc::utils::Result<std::string> food_in_off_json_format);

    void updateLanguageCountry(const std::string& locale);
    void setUserAgent(const std::string& ua);

  private:
    std::string baseUrl_;
    std::string locale;
    std::string userAgent_ =
        "CalorieCounter/1.0 (+https://example.com; anas@example.com)"; // update it later
};

} // namespace cc::clients
