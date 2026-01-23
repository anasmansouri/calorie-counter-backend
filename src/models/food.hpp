#pragma once
#include <cstdint>
#include <string>
#include <optional>
#include <vector>
#include <chrono>

#include <magic_enum.hpp>
#include "models/nutrient.hpp"
#include <nlohmann/json.hpp>
namespace cc::models
{
    enum  class SOURCE:uint8_t
    {
        Manual,
        Online
    };
    // Represents a single food item
    class Food
    {
    public:
        Food() = default;
        Food(std::string id_,
             std::string name_,
             double caloriesPer100g_,
             std::vector<Nutrient> nutrient_,
             std::optional<double> servingSizeG_, // by default zero until we know how much the user want to consume
             std::optional<std::string> barcode_,
             std::optional<std::string> brand_,
             std::optional<std::string> imageUrl_);

        // IDs & identity
        const std::string &id() const;
        void setId(std::string id);

        const std::string &name() const;
        void setName(std::string n);

        const std::optional<std::string> &brand() const;
        void setBrand(std::optional<std::string> b);

        const std::optional<std::string> &barcode() const;
        void setBarcode(std::optional<std::string> b);

        // nutrition
        double caloriesPer100g() const;
        void setCaloriesPer100g(double kcal);

        double totalKcal() const ; // it is a getter and not const while i need to calculate totalKcal before i give it back
                                  // no need to implement setter

        const std::optional<double> &servingSizeG() const;
        void setServingSizeG(std::optional<double> g);

        const std::vector<Nutrient> &nutrients() const;
        // std::vector<Nutrient>& nutrients();              // mutable access (for building)
        void setNutrients(std::vector<Nutrient> ns);

        // media/source
        const std::optional<std::string> &imageUrl() const;
        void setImageUrl(std::optional<std::string> url);

        const SOURCE &source() const; // "off" | "manual" | ...
        void setSource(SOURCE s);

        std::string to_string()const; 


    private:
        std::string id_;
        std::string name_;
        double totalKcal_{0.0};
        double caloriesPer100g_{0.0};
        std::vector<Nutrient> nutrients_;
        std::optional<double> servingSizeG_;
        std::optional<std::string> barcode_;
        std::optional<std::string> brand_;
        std::optional<std::string> imageUrl_{"img_url_empty"};
        SOURCE source_{SOURCE::Online};
    }; // Food
    inline void to_json(nlohmann::json &j, const cc::models::Food &f)
    {

        nlohmann::json nutrient_array = nlohmann::json::array();

        for (cc::models::Nutrient i : f.nutrients())
        {
            nlohmann::json n = i;
            nutrient_array.push_back(n);
        }
        j = {
            {"id", f.id()},
            {"name", f.name()},
            {"nutrient", nutrient_array},
            {"caloriesPer100g", f.caloriesPer100g()},
            {"servingSizeG", f.servingSizeG()},
            {"barcode", f.barcode()},
            {"brand", f.brand()},
            {"imageUrl", f.imageUrl()},
            {"source", magic_enum::enum_name(f.source())}};
    }

    inline void from_json(const nlohmann::json &j, cc::models::Food &f)
    {
        f.setId(j.at("id").get<std::string>());
        f.setName(j.at("name").get<std::string>());
        f.setCaloriesPer100g(j.at("caloriesPer100g").get<double>());
        // nutrient :
        std::vector<cc::models::Nutrient> vector_of_nutrients;
        if(j.contains("nutrient") && !j.at("nutrient").is_null()){
            for (nlohmann::json i : j["nutrient"])
            {
                 Nutrient n = i;
                 vector_of_nutrients.push_back(n);
            }
        }
        f.setNutrients(vector_of_nutrients);
        if (j.contains("servingSizeG") && !j.at("servingSizeG").is_null())
        {
            f.setServingSizeG(j.at("servingSizeG").get<std::optional<double>>().value());
        }else{
            f.setServingSizeG(0);
        }
        f.setBarcode(j.at("barcode").get<std::optional<std::string>>().value());
        f.setBrand(j.at("brand").get<std::optional<std::string>>().value());
        f.setImageUrl(j.at("imageUrl").get<std::string>());
        auto source_value = magic_enum::enum_cast<SOURCE>(j.at("source").get<std::string>());
        if(source_value){
            f.setSource(source_value.value());
        }else{
            f.setSource(SOURCE::Online);
        }
    }

} // namespace cc::models
