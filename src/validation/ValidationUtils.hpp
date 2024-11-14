#ifndef ValidationUtils_hpp
#define ValidationUtils_hpp

#include "dto/EncodingDTO.hpp"
#include "dto/EncodingTemplateDTO.hpp"
#include "dto/SettingsDTO.hpp"
#include <stdexcept>
#include <string>

namespace Validation {

 class ValidationException final : public std::runtime_error {
 public:
  explicit ValidationException(const std::string& message) : std::runtime_error(message) {}
 };

 void validateEncodingTemplate(const oatpp::Object<EncodingTemplateDTO>& templateDto);

} // namespace Validation

#endif /* ValidationUtils_hpp */